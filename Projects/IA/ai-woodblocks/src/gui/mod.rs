use crate::Piece;
use crate::Pos;
use crate::SaveFile;
use std::cell::LazyCell;
use std::fs::File;
use std::io::Write;
use std::path::PathBuf;

use bevy::color::palettes::css::SEA_GREEN;
use bevy::color::palettes::tailwind::RED_600;
use bevy::{
    color::{LinearRgba, palettes::css::BROWN},
    math::primitives::Rectangle,
    prelude::*,
    sprite::ColorMaterial,
};

use crate::model::board::BOARD_SIDE;
use crate::model::state::State;

mod ai;
pub use ai::AIWoodBlocks;

mod human;
pub use human::HumanWoodBlocks;

mod embedded;
pub use embedded::EmbeddedAssetPlugin;

#[derive(Resource)]
pub struct OutputPath(pub Option<PathBuf>);

// ============================== //
/// Windows width
const X: f32 = 600.;
/// Windows height
const Y: f32 = 900.;
const _: () = assert!((X - Y).abs() > 150.);
const _: () = assert!(((X / Y) >= 0.5 && (X / Y) <= 2.) || ((Y / X) >= 0.5 && (Y / X) <= 2.));
// ============================== //
/// Padding size
const PADDING: f32 = (X - Y).abs() / 20.;
// ============================== //
/// Board side lenght
const BOARD_PX: f32 = X.min(Y) - PADDING;
/// Piece side lenght
const PIECE_PX: f32 = BOARD_PX / BOARD_SIDE as f32;
/// Piece mesh
const PIECE_SQUARE: LazyCell<Rectangle> = LazyCell::new(|| Rectangle::new(PIECE_PX, PIECE_PX));
// const PIECE_COLOR: LazyCell<ColorMaterial> = LazyCell::new(|| {
//     let rgba = "654321FF";
//     let color = u32::from_str_radix(rgba, 16).unwrap();
//     let bytes = color.to_be_bytes();
//     ColorMaterial {
//         color: LinearRgba::from_u8_array(bytes).into(),
//         ..Default::default()
//     }
// });
/// Optimal piece scale when on batch
const PIECE_SCALE: f32 = ((BATCH_AREA_Y) / BOARD_PX).min((BATCH_AREA_X) / BOARD_PX) * 1.5;
// ============================== //
const X_OFFSET: f32 = if Y > X {
    0.
} else {
    -(X - BOARD_PX) / 2. + PADDING / 2.
};
const Y_OFFSET: f32 = if Y > X { (Y - BOARD_PX) / 6. } else { 0. };
// ============================== //
const SCORE_X: f32 = if Y > X {
    X - PADDING
} else {
    X - BOARD_PX - PADDING * 1.5
};
const SCORE_Y: f32 = if Y > X {
    (Y - BOARD_PX) / 3. - PADDING
} else {
    Y / 4. - PADDING
};
// ============================== //
const SCORE_X_OFFS: f32 = if Y > X {
    0.
} else {
    BOARD_PX / 2. + X_OFFSET + SCORE_X / 2. + PADDING / 2.
};
const SCORE_Y_OFFS: f32 = if Y > X {
    BOARD_PX / 2. + Y_OFFSET + SCORE_Y / 2. + PADDING / 2.
} else {
    Y / 2. - SCORE_Y / 2. - PADDING / 2.
};
// ============================== //
const SCORE_FONTSIZE: f32 = if Y > X {
    (SCORE_Y - PADDING * 2.).min(SCORE_X * 0.1)
} else {
    (SCORE_X * 0.1).min(SCORE_Y * 0.9)
};
// ============================== //
const BATCH_AREA_X: f32 = if Y > X {
    X - PADDING
} else {
    X - BOARD_PX - PADDING * 1.5
};
const BATCH_AREA_Y: f32 = if Y > X {
    2. * ((Y - BOARD_PX) / 3.) - PADDING
} else {
    3. * Y / 4. - PADDING
};
// ============================== //
const BATCH_AREA_X_OFFS: f32 = if Y > X {
    0.
} else {
    BOARD_PX / 2. + X_OFFSET + BATCH_AREA_X / 2. + PADDING / 2.
};
const BATCH_AREA_Y_OFFS: f32 = if Y > X {
    -(BOARD_PX / 2. - Y_OFFSET + BATCH_AREA_Y / 2. + PADDING / 2.)
} else {
    -(Y / 2. - BATCH_AREA_Y / 2.) + PADDING / 2.
};
// ============================== //

/// Event describing if we reached game over.
#[derive(Event)]
struct GameOver;

/// Sends game over if end condition detected.
fn is_game_over(
    mut event: EventWriter<GameOver>,
    q: Query<Entity, With<GameOver>>,
    state: Res<State>,
) {
    if q.is_empty() {
        if state.is_game_over() | state.is_won() {
            event.send(GameOver);
        }
    }
}

/// Handles game over.
fn handle_game_over(
    mut event: EventReader<GameOver>,
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
) {
    if !event.is_empty() {
        event.clear();
        commands
            .spawn((
                GameOver,
                Mesh2d(meshes.add(Rectangle::new(X - PADDING, Y / 4.))),
                MeshMaterial2d(materials.add(ColorMaterial {
                    color: LinearRgba::from_u8_array([0xA1, 0xA1, 0xA1, 200]).into(),
                    ..Default::default()
                })),
                Transform::from_xyz(0., 0., 4.),
            ))
            .with_children(|parent| {
                parent.spawn((
                    Text2d("GAME OVER".to_string()),
                    TextFont {
                        font_size: Y / 8. - PADDING,
                        ..Default::default()
                    },
                    TextColor::from(SEA_GREEN),
                    Transform::from_xyz(0., Y / 16., 5.),
                ));
                parent.spawn((
                    Text2d("Press <Esc> to exit.".to_string()),
                    TextColor::from(RED_600),
                    Transform::from_xyz(0., -Y / 16., 5.),
                ));
            });
    }
}

/// Adds ability to exit on ESC
fn exit_on_esc(
    save: Res<OutputPath>,
    file: Res<SaveFile>,
    keys: Res<ButtonInput<KeyCode>>,
    mut exit: EventWriter<AppExit>,
) {
    if keys.just_released(KeyCode::Escape) {
        if let Some(p) = &save.0 {
            println!("Saving to '{}'...", p.display());
            let f = File::create(p);
            match f {
                Ok(mut f) => {
                    let state_bytes = ron::to_string(file.into_inner())
                        .expect("This is supposed to be serializable.");
                    f.write_all(state_bytes.as_bytes())
                        .expect("Should've been able to write to file, but couldn't.")
                }
                Err(e) => println!("Failed opening save file: {e}."),
            }
        }

        println!("Exiting...");
        exit.send(AppExit::Success);
    }
}

#[allow(dead_code)]
fn save_on_s(save: Res<OutputPath>, file: Res<SaveFile>, keys: Res<ButtonInput<KeyCode>>) {
    if keys.just_released(KeyCode::KeyS) {
        if let Some(p) = &save.0 {
            println!("Saving to '{}'...", p.display());
            let f = File::create(p);
            match f {
                Ok(mut f) => {
                    let state_bytes = ron::to_string(file.into_inner())
                        .expect("This is supposed to be serializable.");
                    f.write_all(state_bytes.as_bytes())
                        .expect("Should've been able to write to file, but couldn't.")
                }
                Err(e) => println!("Failed opening save file: {e}."),
            }
        }
    }
}

/// Sets up the camera.
fn setup_camera(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
    asset_server: Res<AssetServer>,
) {
    commands.spawn((Camera2d,));

    let img = asset_server.load("embedded://wood.png");
    commands.spawn((
        Mesh2d(meshes.add(Rectangle::new(X, Y))),
        MeshMaterial2d(materials.add(ColorMaterial {
            texture: Some(img.clone()),
            ..Default::default()
        })),
        Transform::from_xyz(0., 0., -1.),
    ));
}

/// Sets up the grid
fn setup_grid(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
) {
    let grid_vert = Rectangle::new(2., BOARD_PX);
    let grid_hori = Rectangle::new(BOARD_PX, 2.);
    for i in 0..=BOARD_SIDE {
        commands.spawn((
            Mesh2d(meshes.add(grid_vert)),
            MeshMaterial2d(materials.add(ColorMaterial {
                color: BROWN.into(),
                ..Default::default()
            })),
            Transform::from_xyz(-BOARD_PX / 2. + i as f32 * PIECE_PX, Y_OFFSET, 1.),
        ));
        commands.spawn((
            Mesh2d(meshes.add(grid_hori)),
            MeshMaterial2d(materials.add(ColorMaterial {
                color: BROWN.into(),
                ..Default::default()
            })),
            Transform::from_xyz(0., Y_OFFSET + BOARD_PX / 2. - i as f32 * PIECE_PX, 1.),
        ));
    }
}

/// Whether the batch changed.
#[derive(Event)]
struct BatchUpdate;

/// Update UI with new batch.
fn batch_refresh_event(
    mut event: EventReader<BatchUpdate>,
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
    assets: Res<AssetServer>,
    state: Res<State>,
) {
    for _ in event.read() {
        for (i, p) in state.batch.0.iter().enumerate() {
            if let Some(piece) = p {
                let x = -((BATCH_AREA_X - PADDING) / 2.)
                    + PIECE_PX / 2.
                    + (PIECE_PX * 4. + PADDING) * PIECE_SCALE * i as f32;
                let y = (BATCH_AREA_Y / 2.) + BATCH_AREA_Y_OFFS - PADDING * 1.75;
                spawn_piece(
                    &mut commands,
                    &mut meshes,
                    &mut materials,
                    &assets,
                    piece,
                    x,
                    y,
                    BatchPiece(i as u8),
                );
            }
        }
    }
}

/// Sets up initial batch
fn setup_batch(mut event: EventWriter<BatchUpdate>) {
    event.send(BatchUpdate);
}

/// Sets up the batch area
fn setup_batch_area(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
) {
    commands.spawn((
        Mesh2d(meshes.add(Rectangle::new(BATCH_AREA_X, BATCH_AREA_Y))),
        MeshMaterial2d(materials.add(ColorMaterial {
            color: LinearRgba::from_u8_array([0x36, 0x37, 0x37, 100]).into(),
            ..Default::default()
        })),
        Transform::from_xyz(BATCH_AREA_X_OFFS, BATCH_AREA_Y_OFFS, 0.5),
    ));
}

/// Whether an entity is a board tile.
#[derive(Component)]
struct BoardTile(bool, Pos);

/// Sets up the board
fn setup_board(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
    assets: Res<AssetServer>,
) {
    // Draw board
    commands.spawn((
        Mesh2d(meshes.add(Rectangle::new(BOARD_PX, BOARD_PX))),
        MeshMaterial2d(materials.add(ColorMaterial {
            color: LinearRgba::from_u8_array([0xFD, 0xF4, 0xDC, 255]).into(),
            ..Default::default()
        })),
        Transform::from_xyz(X_OFFSET, Y_OFFSET, 0.),
    ));

    let wood_planks = assets.load("embedded://minecraft_wood_plank.png");
    let piece_color = ColorMaterial {
        texture: Some(wood_planks),
        ..Default::default()
    };
    // Draw Tiles
    let x_start = -(BOARD_PX / 2.) + PIECE_PX / 2.;
    let y_start = BOARD_PX / 2. + Y_OFFSET - PIECE_PX / 2.;
    for x in 0..BOARD_SIDE {
        for y in 0..BOARD_SIDE {
            commands.spawn((
                BoardTile(false, Pos { x, y }),
                Transform::from_xyz(
                    x_start + PIECE_PX * x as f32,
                    y_start - PIECE_PX * y as f32,
                    1.1,
                )
                .with_scale(Vec3::splat(0.)),
                Mesh2d(meshes.add(*PIECE_SQUARE)),
                MeshMaterial2d(materials.add(piece_color.clone())),
            ));
        }
    }
}

/// Updates the board
fn update_board(state: Res<State>, mut pieces: Query<(&mut BoardTile, &mut Transform)>) {
    for (mut bt, mut t) in pieces.iter_mut() {
        let pos = bt.1;
        let is_enabled = bt.0;
        let enabled_in_board = state
            .board
            .get(pos)
            .expect("Should be impossible to have an invalid tile.");
        if is_enabled != enabled_in_board {
            bt.0 = enabled_in_board;
            if enabled_in_board {
                t.scale = Vec3::splat(0.95);
            } else {
                t.scale = Vec3::splat(0.);
            }
        }
    }
}

/// Updates the score.
fn update_score(state: Res<State>, mut score: Query<&mut Text2d, With<ScoreText>>) {
    let curr_score = state.score;
    let score_str = format!("Score: {curr_score:06}");
    let mut text = score.single_mut();
    text.0 = score_str;
}

/// Whether a text is a ScoreText.
#[derive(Component)]
struct ScoreText;

/// Sets up the score board.
fn setup_score_board(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
    state: Res<State>,
) {
    commands.spawn((
        Mesh2d(meshes.add(Rectangle::new(SCORE_X, SCORE_Y))),
        MeshMaterial2d(materials.add(ColorMaterial {
            color: LinearRgba::from_u8_array([0x36, 0x37, 0x37, 200]).into(),
            ..Default::default()
        })),
        Transform::from_xyz(SCORE_X_OFFS, SCORE_Y_OFFS, 0.5),
    ));

    let curr_score = state.score;

    commands.spawn((
        ScoreText,
        Text2d::from(format!("Score: {curr_score:06}")),
        TextFont {
            font_size: SCORE_FONTSIZE,
            ..Default::default()
        },
        TextColor::from(LinearRgba::from_u8_array_no_alpha([0x98, 0xFF, 0x98])),
        Transform::from_xyz(SCORE_X_OFFS, SCORE_Y_OFFS, 1.),
    ));
}

/// Whether something is grabbable
#[derive(Component)]
struct Grabbable(bool);

/// Whether something is a piece of the batch.
#[derive(Debug, Component)]
struct BatchPiece(u8);

/// Spawn a grabbable piece.
#[inline]
fn spawn_piece(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    materials: &mut ResMut<Assets<ColorMaterial>>,
    assets: &Res<AssetServer>,
    piece: &Piece,
    x: f32,
    y: f32,
    piece_idx: BatchPiece,
) {
    let wood_planks = assets.load("embedded://minecraft_wood_plank.png");
    let piece_color = ColorMaterial {
        texture: Some(wood_planks),
        ..Default::default()
    };
    commands
        .spawn((
            piece_idx,
            Transform::from_xyz(x, y, 2.).with_scale(Vec3::splat(PIECE_SCALE)),
            Grabbable(false),
        ))
        .with_children(|parent| {
            for pos in &piece.0 {
                let coord_x = pos.x as f32 * PIECE_PX;
                let coord_y = -(pos.y as f32) * PIECE_PX;
                parent.spawn((
                    Mesh2d(meshes.add(*PIECE_SQUARE)),
                    MeshMaterial2d(materials.add(piece_color.clone())),
                    Transform::from_xyz(coord_x, coord_y, 2.),
                ));
            }
        });
}
