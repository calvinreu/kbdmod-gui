mod gui;
mod keyboard;
mod storage;
mod render;
mod iced_main;
use iced::Settings;
use gui::Gui;
use iced::Sandbox;
//
fn main() {
    storage::init().unwrap();
    Gui::run(Settings::default()).unwrap();
}
