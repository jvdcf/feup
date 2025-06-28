use std::path::PathBuf;

use bevy::{
    DefaultPlugins,
    app::Plugin,
    math::bounding::{Aabb2d, BoundingVolume},
    prelude::*,
    window::{Window, WindowPlugin, WindowResolution},
};

use crate::{
    SaveFile,
    model::{board::BOARD_SIDE, pos::Pos, state::State},
};

use super::{
    BATCH_AREA_X, BATCH_AREA_Y, BATCH_AREA_Y_OFFS, BOARD_PX, BatchPiece, BatchUpdate,
    EmbeddedAssetPlugin, GameOver, Grabbable, OutputPath, PADDING, PIECE_PX, PIECE_SCALE, X, Y,
    Y_OFFSET, batch_refresh_event, exit_on_esc, handle_game_over, is_game_over, setup_batch,
    setup_batch_area, setup_board, setup_camera, setup_grid, setup_score_board, update_board,
    update_score,
};

/// Plugin that describes a uman playing the game.
pub struct HumanWoodBlocks {
    pub curr_state: State,
    pub file: SaveFile,
    pub output_path: Option<PathBuf>,
}

impl Plugin for HumanWoodBlocks {
    fn build(&self, app: &mut bevy::app::App) {
        app.add_plugins((
            DefaultPlugins.set(WindowPlugin {
                primary_window: Some(Window {
                    resolution: WindowResolution::new(X, Y),
                    title: "WoodBlocks!".to_string(),
                    name: Some("WoodBlocks!".to_string()),
                    resizable: false,
                    ..Default::default()
                }),
                ..Default::default()
            }),
            EmbeddedAssetPlugin,
        ))
        .insert_resource(self.file.clone())
        .insert_resource(self.curr_state.clone())
        .insert_resource(OutputPath(self.output_path.clone()))
        .add_systems(Startup, setup_camera)
        .add_systems(Startup, setup_board)
        .add_systems(Startup, setup_score_board)
        .add_systems(Startup, setup_batch_area)
        .add_systems(Startup, setup_grid)
        .add_systems(Startup, setup_batch)
        .add_systems(Update, batch_refresh_event)
        .add_systems(Update, grab_stuff)
        .add_systems(Update, update_board)
        .add_systems(Update, update_score)
        .add_systems(Update, exit_on_esc)
        .add_systems(Update, is_game_over)
        .add_systems(Update, handle_game_over)
        .add_event::<BatchUpdate>()
        .add_event::<GameOver>();
    }
}

/// Ability to click and drag.
fn grab_stuff(
    q_window: Query<&Window>,
    mut commands: Commands,
    mut state: ResMut<State>,
    mut file: ResMut<SaveFile>,
    mut grabbables: Query<(&mut Grabbable, &mut Transform, &mut BatchPiece, Entity)>,
    camera_query: Single<(&Camera, &GlobalTransform)>,
    mouse_button_input: Res<ButtonInput<MouseButton>>,
    mut event: EventWriter<BatchUpdate>,
) {
    let (camera, camera_transform) = *camera_query;
    let window = q_window.single();
    let Some(cursor) = window.cursor_position() else {
        return;
    };

    let Ok(world_pos) = camera.viewport_to_world_2d(camera_transform, cursor) else {
        return;
    };

    let Ok(viewport_check) = camera.world_to_viewport(camera_transform, world_pos.extend(0.0))
    else {
        return;
    };
    let Ok(_world_check) = camera.viewport_to_world_2d(camera_transform, viewport_check.xy())
    else {
        return;
    };

    if grabbables.iter().filter(|x| x.0.0).count() > 1 {
        let mut found = false;
        for (mut g, _, _, _) in grabbables.iter_mut() {
            if found {
                g.0 = false;
            } else {
                found = g.0;
            }
        }
    }

    if let Some(grabbed) = grabbables.iter_mut().filter(|x| x.0.0).next() {
        let (mut g, mut t, p, id) = grabbed;
        *t = Transform::from_translation(Vec3 {
            x: world_pos.x,
            y: world_pos.y,
            z: 3.,
        });
        if mouse_button_input.just_released(MouseButton::Left) {
            g.0 = false;
            t.scale = Vec3::splat(PIECE_SCALE);
            let x_start = -(BOARD_PX / 2.);
            let y_start = BOARD_PX / 2. + Y_OFFSET;
            let x_end = x_start + BOARD_PX;
            let y_end = y_start - BOARD_PX;

            let inside_board = (x_start < world_pos.x && world_pos.x < x_end)
                && (y_end < world_pos.y && world_pos.y < y_start);
            let refreshed_batch = state.batch.0.iter().filter(|x| x.is_some()).count() == 1;
            if inside_board {
                let pos = Pos {
                    x: ((world_pos.x - x_start) / PIECE_PX) as usize,
                    y: BOARD_SIDE - 1 - ((world_pos.y - y_end) / PIECE_PX) as usize,
                };
                if let Ok(m) = state.play(p.0 as usize, pos) {
                    file.insert_move(m);
                    commands.entity(id).despawn_recursive();
                    if refreshed_batch {
                        event.send(BatchUpdate);
                    }
                } else {
                    let x = -((BATCH_AREA_X - PADDING) / 2.)
                        + PIECE_PX / 2.
                        + (PIECE_PX * 4. + PADDING) * PIECE_SCALE * p.0 as f32;
                    let y = (BATCH_AREA_Y / 2.) + BATCH_AREA_Y_OFFS - PADDING * 1.75;
                    *t = Transform::from_translation(Vec3 { x, y, z: 3. });
                    t.scale = Vec3::splat(PIECE_SCALE);
                }
            } else {
                let x = -((BATCH_AREA_X - PADDING) / 2.)
                    + PIECE_PX / 2.
                    + (PIECE_PX * 4. + PADDING) * PIECE_SCALE * p.0 as f32;
                let y = (BATCH_AREA_Y / 2.) + BATCH_AREA_Y_OFFS - PADDING * 1.75;
                *t = Transform::from_translation(Vec3 { x, y, z: 3. });
                t.scale = Vec3::splat(PIECE_SCALE);
            }
        }
        return;
    }

    if mouse_button_input.just_pressed(MouseButton::Left) {
        for (mut g, t, _, _) in grabbables.iter_mut() {
            let cursor_aabb = Aabb2d::new(world_pos, Vec2::new(2., 2.));
            let obj_box = Aabb2d::new(
                Vec2::new(
                    t.translation.x + PIECE_PX * 1.5 * PIECE_SCALE,
                    t.translation.y - PIECE_PX * 1.5 * PIECE_SCALE,
                ),
                Vec2::splat(PIECE_PX * 4. * PIECE_SCALE / 2.),
            );
            if obj_box.contains(&cursor_aabb) {
                g.0 = true;
                return;
            }
        }
    }
}
