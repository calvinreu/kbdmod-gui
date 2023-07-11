use crate::keyboard::{VirtualKeyboard, KeyMapping};
use crate::storage::Entry;

#[derive(Debug, Clone)]
pub enum Message {
    //config selection overview
    ConfigSelected(Entry),
    ConfigNameChanged(Entry, String),
    ConfigAdded,
    ConfigRemoved(Entry),
    ConfigCopied(Entry),//old, new
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
    ConfigOverview(ConfigSelectionType),
    Confirmation(String),
    ConfigEditing,
    LayerEditing,
    InputDeviceEditing,
    PropertiesEditing,
    MappingEditing(String),
}

#[derive(Debug, Clone)]
pub enum ConfigSelectionType {
    Edit,
    Copy,
    Delete,
}

pub struct Gui {
    pub entries: Vec<Entry>,
    pub current: VirtualKeyboard,
    pub selected_config: Entry,
    pub selected_layer: String,
    pub page: Page,
}