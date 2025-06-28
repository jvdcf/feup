use std::path::PathBuf;

use bevy::{
    app::{App, Plugin},
    asset::io::embedded::{self, EmbeddedAssetRegistry},
};

/// Plugin for including assets at compile time.
pub struct EmbeddedAssetPlugin;

impl Plugin for EmbeddedAssetPlugin {
    #[cfg(unix)]
    fn build(&self, app: &mut App) {
        let wood = include_bytes!("../../assets/wood.png");
        let plank = include_bytes!("../../assets/minecraft_wood_plank.png");
        let embedded = app.world_mut().resource_mut::<EmbeddedAssetRegistry>();
        let wood_watch = embedded::watched_path(file!(), "../../assets/wood.png");
        embedded.insert_asset(wood_watch, &PathBuf::from("wood.png"), wood);
        let plank_watch = embedded::watched_path(file!(), "../../assets/minecraft_wood_plank.png");
        embedded.insert_asset(
            plank_watch,
            &PathBuf::from("minecraft_wood_plank.png"),
            plank,
        );
    }
    #[cfg(windows)]
    fn build(&self, app: &mut App) {
        let wood = include_bytes!("..\\..\\assets\\wood.png");
        let plank = include_bytes!("..\\..\\assets\\minecraft_wood_plank.png");
        let embedded = app.world_mut().resource_mut::<EmbeddedAssetRegistry>();
        let wood_watch = embedded::watched_path(file!(), "..\\..\\assets\\wood.png");
        embedded.insert_asset(wood_watch, &PathBuf::from("wood.png"), wood);
        let plank_watch =
            embedded::watched_path(file!(), "..\\..\\assets\\minecraft_wood_plank.png");
        embedded.insert_asset(
            plank_watch,
            &PathBuf::from("minecraft_wood_plank.png"),
            plank,
        );
    }
}
