use std::path::PathBuf;

use bevy::DefaultPlugins;
use bevy::app::Plugin;
use bevy::color::palettes::css::{DARK_GRAY, WHITE};
use bevy::prelude::*;
use bevy::tasks::futures_lite::future;
use bevy::tasks::{AsyncComputeTaskPool, Task, TaskPool, block_on};
use bevy::window::{Window, WindowPlugin, WindowResolution};

use crate::Algorithm as AlgoSelector;
use crate::algorithms::astar::AStar;
use crate::algorithms::bfs::BFS;
use crate::algorithms::dfs::DFS;
use crate::algorithms::greedy::Greedy;
use crate::algorithms::weighted_astar::WeightedAStar;
use crate::algorithms::{Algorithm, Benchmark};
use crate::gui::{EmbeddedAssetPlugin, GameOver, exit_on_esc, handle_game_over, is_game_over};
use crate::model::Movement;
use crate::{SaveFile, model::state::State};

use super::{
    BatchPiece, BatchUpdate, OutputPath, X, Y, batch_refresh_event, setup_batch, setup_batch_area,
    setup_board, setup_camera, setup_grid, setup_score_board, update_board, update_score,
};

/// Plugin for playing an AI powered game.
pub struct AIWoodBlocks {
    pub file: SaveFile,
    pub curr_state: State,
    pub algo: AlgoSelector,
    pub output_path: Option<PathBuf>,
}

/// Describes what moves an AI can play.
#[derive(Resource)]
struct AIMoves(Option<Vec<Movement>>);

/// Represents an async task of calculating an AI result
#[derive(Resource)]
struct LoadedAi(AlgoSelector, Option<Task<(Benchmark, Vec<Movement>)>>);

impl Plugin for AIWoodBlocks {
    fn build(&self, app: &mut bevy::app::App) {
        let file = self.file.clone();
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
        .insert_resource(file)
        .insert_resource(self.curr_state.clone())
        .insert_resource(LoadedAi(self.algo.clone(), None))
        .insert_resource(AIMoves(None))
        .insert_resource(OutputPath(self.output_path.clone()))
        .add_systems(Startup, setup_camera)
        .add_systems(Startup, setup_board)
        .add_systems(Startup, setup_score_board)
        .add_systems(Startup, setup_batch_area)
        .add_systems(Startup, setup_grid)
        .add_systems(Startup, setup_batch)
        .add_systems(Startup, load_loading_screen)
        .add_systems(Startup, setup_ai_calculation)
        .add_systems(Update, setup_next_button)
        .add_systems(Update, batch_refresh_event)
        .add_systems(Update, update_board)
        .add_systems(Update, update_score)
        .add_systems(Update, exit_on_esc)
        .add_systems(Update, next_move_button)
        .add_systems(Update, next_move_key)
        .add_systems(Update, handle_next_move_event)
        .add_systems(Update, is_game_over)
        .add_systems(Update, is_calculation_finished)
        .add_systems(Update, handle_game_over)
        .add_systems(Update, display_loading_screen)
        .add_event::<BatchUpdate>()
        .add_event::<GameOver>()
        .add_event::<NextMoveEvent>()
        .add_event::<SpawnButton>();
    }
}

/// Event to spawn the "next" button.
#[derive(Event)]
struct SpawnButton;

/// Whether the "next" button is pressed
#[derive(Event)]
struct NextMoveEvent;

/// Show a button on the bottom right corner of the screen
fn setup_next_button(mut commands: Commands, mut event: EventReader<SpawnButton>) {
    if event.is_empty() {
        return;
    }
    event.clear();
    commands
        .spawn(Node {
            width: Val::Percent(100.0),
            height: Val::Percent(100.0),
            align_items: AlignItems::End,
            justify_content: JustifyContent::End,
            ..default()
        })
        .with_children(|parent| {
            parent
                .spawn((
                    Button,
                    Node {
                        width: Val::Px(50.0),
                        height: Val::Px(50.0),
                        justify_content: JustifyContent::Center,
                        align_items: AlignItems::Center,
                        margin: UiRect::all(Val::Px(10.0)),
                        ..default()
                    },
                    BackgroundColor(Color::WHITE),
                    BorderRadius::all(Val::Px(10.0)),
                ))
                .with_child((
                    Text::new(">"),
                    TextFont {
                        font_size: 33.0,
                        ..default()
                    },
                    TextColor(Color::BLACK),
                ));
        });
}

/// Call the NextMoveEvent when the button is pressed
fn next_move_button(
    mut interaction_query: Query<
        (&Interaction, &mut BackgroundColor),
        (Changed<Interaction>, With<Button>),
    >,
    mut event: EventWriter<NextMoveEvent>,
) {
    for (interaction, mut color) in &mut interaction_query {
        match *interaction {
            Interaction::Pressed => {
                *color = BackgroundColor(DARK_GRAY.into());
                event.send(NextMoveEvent);
            }
            Interaction::None => {
                *color = BackgroundColor(WHITE.into());
            }
            _ => {}
        }
    }
}

/// Call the NextMoveEvent when the right arrow key is pressed
fn next_move_key(keys: Res<ButtonInput<KeyCode>>, mut event: EventWriter<NextMoveEvent>) {
    if keys.just_released(KeyCode::ArrowRight) {
        event.send(NextMoveEvent);
    }
}

/// Apply the next move to the game state when the NextMoveEvent is dispatched
fn handle_next_move_event(
    mut nm_event: EventReader<NextMoveEvent>,
    mut state: ResMut<State>,
    mut moves: ResMut<AIMoves>,
    mut bu_event: EventWriter<BatchUpdate>,
    batch_pieces: Query<(&mut BatchPiece, Entity)>,
    mut commands: Commands,
    button: Query<Entity, With<Button>>,
    mut file: ResMut<SaveFile>,
) {
    if moves.0.is_none() {
        return;
    }

    for _ in nm_event.read() {
        if let Some(Some(movement)) = moves.0.as_ref().map(|x| x.first()) {
            let _ = state
                .play(movement.piece_idx, movement.pos)
                .expect("Moves from the SaveFile must be valid.");

            // Remove piece from batch
            for (batch_piece, entity) in batch_pieces.iter() {
                if batch_piece.0 as usize == movement.piece_idx {
                    commands.entity(entity).despawn_recursive();
                }
            }

            file.insert_move(movement.clone());
            bu_event.send(BatchUpdate);
            moves.0.as_mut().map(|x| x.remove(0));

            // Remove button if no more moves are left
            if moves.0.as_ref().is_some_and(|x| x.is_empty()) {
                for entity in button.iter() {
                    commands.entity(entity).despawn_recursive();
                }
            }
        }
    }
}

/// ZST Component for marking a loading screen.
#[derive(Component)]
struct LoadingScreen;

/// Spawns the necessary components for the loading screen.
fn load_loading_screen(mut commands: Commands) {
    let text_style = TextFont {
        font_size: 40.0,
        ..default()
    };

    // Spawn the UI and Loading screen camera.
    commands.spawn((
        Camera2d,
        Camera {
            order: 1,
            ..default()
        },
        LoadingScreen,
    ));

    // Spawn the UI that will make up the loading screen.
    commands
        .spawn((
            Node {
                height: Val::Percent(100.0),
                width: Val::Percent(100.0),
                justify_content: JustifyContent::Center,
                align_items: AlignItems::Center,
                ..default()
            },
            BackgroundColor(Color::BLACK),
            LoadingScreen,
        ))
        .with_child((Text::new("AI is thinking..."), text_style.clone()));
}

/// Determines when to show the loading screen
fn display_loading_screen(
    mut loading_screen: Single<&mut Visibility, (With<LoadingScreen>, With<Node>)>,
    moves: Res<AIMoves>,
) {
    **loading_screen = if let None = moves.0 {
        Visibility::Visible
    } else {
        Visibility::Hidden
    };
}

/// Starts an AI caculation.
fn setup_ai_calculation(mut loaded_ai: ResMut<LoadedAi>, state: Res<State>) {
    let tasks_pool = AsyncComputeTaskPool::get_or_init(TaskPool::new);
    let func = match loaded_ai.0 {
        AlgoSelector::Dfs => DFS::run,
        AlgoSelector::Bfs => BFS::run,
        AlgoSelector::AStar => AStar::run,
        AlgoSelector::WAStar => WeightedAStar::run,
        AlgoSelector::Greedy => Greedy::run,
    };
    let st = state.clone();
    let task = tasks_pool.spawn(async move { func(st) });
    loaded_ai.1 = Some(task);
}

/// Determines if the calculation has finnished.
fn is_calculation_finished(
    mut loaded_ai: ResMut<LoadedAi>,
    mut ai_moves: ResMut<AIMoves>,
    mut file: ResMut<SaveFile>,
    mut event: EventWriter<SpawnButton>,
) {
    if ai_moves.0.is_some() {
        return;
    }
    if let Some(task) = loaded_ai.1.take() {
        if task.is_finished() {
            let Some((bench, moves)) = block_on(future::poll_once(task)) else {
                return;
            };
            println!("");
            println!("FINISHED BENCHMARKING:");
            println!("{bench}");
            file.bench = Some(bench);
            ai_moves.0 = Some(moves);
            event.send(SpawnButton);
            return;
        }
        loaded_ai.1 = Some(task);
    }
}
