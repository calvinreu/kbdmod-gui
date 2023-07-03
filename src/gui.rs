use crate::keyboard::{VirtualKeyboard, KeyMapping, empty_vk};
use crate::storage::{save_config, load_config, delete_config, move_config};
use std::collections::HashMap as Map;
use std::collections::LinkedList as List;
use std::ptr::null_mut;
use iced::{Application, executor, theme};
use serde_json::map::Entry;

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
    config_list: Vec<storage::Entry>,
    current: VirtualKeyboard,
    selected_config: String,
    selected_mapping: String,
    page: Page,
}

impl Application for Gui {
    type Executor = executor::Default;
    type Message = Message;
    type Theme = theme::Theme;
    type Flags = String;
    fn new(path: String) -> (Gui, iced::Command<Message>) {
        (Gui{
            config: load_config(path).unwrap(),
            current: std::rc::Rc::,
            selected_config: "".to_string(),
            selected_mapping: "".to_string(),
            page: Page::ConfigOverview,
        }, iced::Command::none())
    }
    fn title(&self) -> String {
        String::from("kbdmod-config")
    }
    fn update(&mut self, message: Message) -> iced::Command<Message> {
        match message {
            Message::ConfigSelected(name) => {
                self.selected_config = name;
                self.page = Page::VirtualKeyboard;
                self.current = self.config.entry(name).and_modify();
                return iced::Command::none();
            },
            Message::ConfigNameChanged(old, new) => {
                if self.config.contains_key(&new) {
                    eprintln!("Error renaming config: config with name {0} already exists", new);
                    return iced::Command::none();
                }
                match move_config(old, new) {
                    Ok(()) => {
                        self.config.insert(new.clone(), self.config.remove(&old).unwrap());
                        self.selected_config = new;
                    },
                    Err(err) => {
                        eprintln!("Error renaming config: {0}", err);
                    },
                };
                return iced::Command::none();
            },

        }
    }
}