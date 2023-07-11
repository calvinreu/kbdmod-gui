use crate::keyboard::empty_vk;
use crate::storage::{Entry, self};
use crate::gui::{Message, Gui, Page, ConfigSelectionType};
use iced::Sandbox;



impl Sandbox for Gui {
    type Message = Message;
    fn new() -> Gui {
        let entry = Entry::new_null();
        Gui{
            entries: storage::load_entries().unwrap(),
            current: empty_vk(),
            selected_config: entry,
            selected_layer: "".to_string(),
            page: Page::ConfigOverview(ConfigSelectionType::Edit),
        }
    }
    fn title(&self) -> String {
        match &self.page {
            Page::ConfigOverview(_) => "kbdmod-gui".to_string(),
            Page::Confirmation(message) => "confirm ".to_string() + message.as_str(),
            Page::ConfigEditing => "editing config:".to_string() + self.selected_config.name.as_str(),
            Page::LayerEditing => "editing layer:".to_string() + self.selected_layer.as_str(),
            Page::InputDeviceEditing => "editing input devices of ".to_string() + self.selected_config.name.as_str(),
            Page::PropertiesEditing => "editing output properties of ".to_string() + self.selected_config.name.as_str(),
            Page::MappingEditing(mapping) => "editing mapping:".to_string() + mapping.as_str(),
        }
    }
    fn update(&mut self, message: Message) {
        match message {
            Message::ConfigSelected(entry) => {
                self.selected_config = entry.clone();
                self.page = Page::ConfigEditing;
                self.current = entry.load_config().unwrap();
            },
            Message::ConfigNameChanged(mut entry, name) => {
                entry.name = name;
                entry.commit(&mut self.entries);
            },
            Message::ConfigAdded => {
                let entry = Entry::new(&mut self.entries);
            },
            _ => {
            },
        }

    }
    fn view(&self) -> iced::Element<'_, Self::Message> {
        match self.page.clone() {
            Page::ConfigOverview(_) => self.render_config_overview(),
            _ => self.render_config_overview(),
        }
    }
}