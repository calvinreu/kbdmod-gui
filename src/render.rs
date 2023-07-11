use iced::widget::{Column, Container, Text, Button, text_input::TextInput, Row};
use crate::gui::{Message, Gui};


impl Gui {
    
pub fn render_config_overview<'a>(&self) -> iced::Element<'a, <Gui as iced::Sandbox>::Message> {
    let mut column = Column::new();
    for entry in self.entries.iter() {
        column = column.push(
        Container::new(
            Column::new()
            .push(
                TextInput::new(
                entry.name.as_str(), entry.name.as_str())
            )
            .push(
                TextInput::new(
                entry.description.as_str(), entry.description.as_str())
            )
            .push(
                Row::new()
                .push(
                    Button::new(
                        Text::new("Edit")
                    )
                    .on_press(Message::ConfigSelected(entry.clone()))
                )
                .push(
                    Button::new(
                        Text::new("Copy")
                    )
                    .on_press(Message::ConfigCopied(entry.clone()))
                )
                .push(
                    Button::new(
                        Text::new("Delete")
                    )
                    .on_press(Message::ConfigRemoved(entry.clone()))
                )
            )
        )
        .center_x()
        );
    }
    let button = Button::new(
        Text::new("Add new config")
    )
    .on_press(Message::ConfigAdded);
    column = column.push(button);
    column.into()
}

}
