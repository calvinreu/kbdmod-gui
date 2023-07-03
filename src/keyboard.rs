use std::{collections::BTreeMap as Map};
use serde_derive::{Serialize, Deserialize};

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq)]
pub enum OutputType {
    Standard(Vec<String>),
    Command(Vec<String>),
    Osm(Vec<String>),
    Empty,
}

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq)]
pub struct KeyMapping{
    pub name: String,
    pub code: u16,
    pub tap      : OutputType,
    pub hold     : OutputType,
    pub doubletap: OutputType,
    pub taphold  : OutputType,
}

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq)]
pub struct VirtualKeyboard{
    pub device_name: String,
    pub physical_devices: Vec<String>,
    pub output_properties: Vec<String>,
    pub keymap: Map<String, u16>,
    pub mappings: Map<String, KeyMapping>,
}

#[allow(dead_code)]
pub fn empty_vk() -> VirtualKeyboard{
    VirtualKeyboard{
        device_name: "".to_string(),
        physical_devices: Vec::new(),
        output_properties: Vec::new(),
        keymap: Map::new(),
        mappings: Map::new(),
    }
}
