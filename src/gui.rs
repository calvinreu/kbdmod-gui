use crate::keyboard::{VirtualKeyboard, KeyMapping, empty_vk};
use crate::storage::{Entry, self};
use std::collections::HashMap as Map;
use std::collections::LinkedList as List;
use std::ptr::null_mut;
use iced::{executor, theme, Sandbox};

#[derive(Debug, Clone)]
pub enum Message {
    //config selection overview
    ConfigSelected(Entry),
    ConfigNameChanged(Entry, String),
    ConfigAdded,
    ConfigRemoved(Entry),
    ConfigCopied(Entry, Entry),//old, new
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
enum Page {
    ConfigOverview(ConfigSelectionType),
    Confirmation(String),
    ConfigEditing,
    LayerEditing,
    InputDeviceEditing,
    PropertiesEditing,
    MappingEditing(String),
}

#[derive(Debug, Clone)]
enum ConfigSelectionType {
    Edit,
    Copy,
    Delete,
}

pub struct Gui {
    current: VirtualKeyboard,
    selected_config: Entry,
    selected_layer: String,
    page: Page,
}

impl Sandbox for Gui {
    type Executor = executor::Default;
    type Message = Message;
    type Theme = theme::Theme;
    type Flags = String;
    fn new(path: String) -> (Gui, iced::Command<Message>) {
        let entry = Entry::new();
        entry.update_config(&empty_vk());
        (Gui{
            current: empty_vk(),
            selected_config: entry,
            selected_layer: "".to_string(),
            page: Page::ConfigOverview(ConfigSelectionType::Edit),
        }, iced::Command::none())
    }
    fn title(&self) -> String {
        match self.page {
            Page::ConfigOverview(_) => "kbdmod-gui".to_string(),
            Page::Confirmation(message) => "confirm ".to_string() + message.as_str(),
            Page::ConfigEditing => "editing config:".to_string() + self.selected_config.name.as_str(),
            Page::LayerEditing => "editing layer:".to_string() + self.selected_layer.as_str(),
            Page::InputDeviceEditing => "editing input devices of ".to_string() + self.selected_config.name.as_str(),
            Page::PropertiesEditing => "editing output properties of ".to_string() + self.selected_config.name.as_str(),
            Page::MappingEditing(mapping) => "editing mapping:".to_string() + mapping.as_str(),
        }
    }
    fn update(&mut self, message: Message) -> iced::Command<Message> {
        match message {
            Message::ConfigSelected(entry) => {
                self.selected_config = entry.clone();
                self.page = Page::ConfigEditing;
                self.current = entry.load_config().unwrap();
                return iced::Command::none();
            },
            Message::ConfigNameChanged(mut entry, name) => {
                entry.name = name;
                entry.commit();
                return iced::Command::none();
            },
            Message::ConfigAdded => {
                let entry = Entry::new();
                return iced::Command::none();
            },
            _ => {
                return iced::Command::none();
            },
        }

    }
    fn run(settings: iced::Settings<()>) -> Result<(), iced::Error>
        where
            Self: 'static + Sized, {
        
    }
}