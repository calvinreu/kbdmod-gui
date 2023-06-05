use crate::keyboard::{VirtualKeyboard, KeyMapping};
use crate::save::{SaveConfig, LoadConfig, DeleteConfig};
use std::collections::BTreeMap as Map;
use std::collections::LinkedList as List;
use iced::{Application, executor, theme};

#[derive(Debug, Clone)]
pub enum Message {
    //config selection overview
    ConfigSelected(String),
    ConfigNameChanged(String, String),
    ConfigAdded(String),
    ConfigRemoved(String),
    //keymap creation
    CopyFrom(String),
    NextKey,
    KeymapCreated,
    //config editing devices
    OutputDeviceChanged(String),
    InputDeviceAdded(String),
    InputDeviceRemoved(String),
    OutputPropertyAdded(String),
    OutputPropertyRemoved(String),
    //keymap editing
    MappingAdded(String),
    MappingRemoved(String),
    MappingChanged(String, KeyMapping),
    //switching pages
    PageChanged(Page),
}

#[derive(Debug, Clone)]
pub enum Page {
    ConfigOverview,
    KeymapCreation,
    VirtualKeyboard,
    Mapping,
}

pub struct Gui {
    config: Map<String, VirtualKeyboard>,
    current: VirtualKeyboard,
    selected_config: String,
    selected_mapping: String,
    page: Page,
    error: List<String>,
}

impl Application for Gui {
    type Executor = executor::Default;
    type Message = Message;
    type Theme = theme::Theme;
    type Flags = ();
    fn new(_: ()) -> (Gui, iced::Command<Message>) {
        (Gui{
            config: LoadConfig(),
            current: EmptyVK(),
            selected_config: "".to_string(),
            selected_mapping: "".to_string(),
            page: Page::ConfigOverview,
            error: List::new(),
        }, iced::Command::none())
    }
    fn title(&self) -> String {
        String::from("kbdmod-gui")
    }
    fn update(&mut self, message: Message) {
        match message {
            Message::ConfigSelected(name) => {
                selected_config = name;
                page = Page::Mapping;
            },
            Message::ConfigNameChanged(old_name, new_name) => {
                if config.contains_key(&new_name) {
                    error.push("Config name already exists".to_string());
                    return;
                }
                config.insert(new_name, config.remove(&old_name));
            },
            Message::ConfigAdded(name) => {
                config.insert(name, keyboard::EmptyVK());
                page = Page::KeymapCreation;
            },
            Message::ConfigRemoved(name) => {
                config.remove(name);
                DeleteConfig(name);
            },
            Message::KeymapCreated => {
                page = Page::VirtualKeyboard;
            },
            Message::CopyFrom(name) => {
                current = config.;                 
            },
        }
    }
}