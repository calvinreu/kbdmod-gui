use crate::keyboard::{VirtualKeyboard};
use std::fs::{File, remove_file};
use std::io::{BufReader, BufWriter, Write, Error};
use serde_derive::{Serialize, Deserialize};

#[cfg(not(test))]
const LOOKUP_FILE : &str = "~/.local/share/kbdmod-gui/lookup.json";
#[cfg(not(test))]
const CONFIG_FOLDER : &str = "~/.local/share/kbdmod-gui/configs/";

#[cfg(test)]
const LOOKUP_FILE : &str = "/tmp/kbdmod-gui-test/lookup.json";
#[cfg(test)]
const CONFIG_FOLDER : &str = "/tmp/kbdmod-gui-test/";


const FIRST_VALID_ID : u32 = 1;
const DELETED_ENTRY_ID : u32 = 0;
const LAST_VALID_ID : u32 = std::u32::MAX;

#[derive(Serialize, Deserialize, Clone, Debug, PartialEq)]
pub struct Entry{
    pub name: String,
    pub path: String,
    pub description: String, 
    pub id: u32,
}

pub static mut ENTRIES: Vec<Entry> = Vec::new();

fn get_free_filename() -> String {
    let mut id = 0 as u32;
    while std::path::Path::new(&(CONFIG_FOLDER.to_string() + &id.to_string() + ".json")).exists() {
        id += 1;
    } 
    
    id.to_string() + ".json"
}

impl Entry {
    pub fn new() -> Entry {
        let mut id: u32 = FIRST_VALID_ID;

        unsafe{
        for entry in ENTRIES.iter() {
            if entry.id > id {
                id = entry.id;
            }
        }
        }
        if id == LAST_VALID_ID {
            panic!("No valid id found, restarting could resolve this issue. if not please report this issue and include the {} file.", LOOKUP_FILE);
        }
        id += 1;
        let path = get_free_filename();

        let entry = 
        Entry{
            name: "".to_string(),
            path: path,
            description: "".to_string(),
            id: id,
        };
        unsafe{
        ENTRIES.push(entry.clone());
        }
        entry
    }
    pub fn load_config(&self) -> Result<VirtualKeyboard,Error> {
        let file = File::open(self.path.clone())?; 
        let buffer = BufReader::new(file);
        let config: VirtualKeyboard = serde_json::from_reader(buffer)?;
        Ok(config)
    }
    pub fn update_config(&self, config: &VirtualKeyboard) -> Result<(),Error> {
        let mut buffer = BufWriter::new(File::create(self.path.clone())?);
        serde_json::to_writer(&mut buffer, &config)?;
        buffer.flush()?;
        Ok(())
    }
    pub fn commit(&self) -> Result<(),Error> {
        unsafe{
        for entry in ENTRIES.iter_mut() {
            if entry.id == self.id {
                *entry = self.clone();
                return Ok(());
            }
        }
        }
        Err(Error::new(std::io::ErrorKind::NotFound, "Entry not found"))
    }
    pub fn remove(& mut self) -> Result<(),Error> {
        self.id = DELETED_ENTRY_ID;
        remove_file(self.path.clone())?;
        Ok(())
    }

}

pub fn save_entries() -> Result<(),Error> {
    let mut buffer = BufWriter::new(File::create(LOOKUP_FILE)?);
    let mut save_vec : Vec<Entry> = Vec::new();
    unsafe{
    save_vec.reserve_exact(ENTRIES.len());
    for i in ENTRIES.iter() {
        if i.id != DELETED_ENTRY_ID {
            let mut entry = i.clone();
            entry.id = save_vec.len() as u32;
            save_vec.push(entry);
        }
    }
    }
    serde_json::to_writer(&mut buffer, &save_vec)?;
    buffer.flush()?;
    unsafe{
    ENTRIES = save_vec;
    }
    Ok(())
}

pub fn load_entries() -> Result<(),Error> {
    let file = File::open(LOOKUP_FILE)?; 
    let buffer = BufReader::new(file);
    unsafe{
    ENTRIES = serde_json::from_reader(buffer)?;
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs::{create_dir, remove_dir_all};
    #[test]
    fn test_saveload() {
        _ = remove_dir_all(CONFIG_FOLDER);
        create_dir(CONFIG_FOLDER).unwrap();
        let config = VirtualKeyboard{
            device_name: "test".to_string(),
            physical_devices: ["a".to_string(), "b".to_string(), "c".to_string()].to_vec(),
            output_properties: ["d".to_string(), "e".to_string(), "f".to_string()].to_vec(),
            keymap: [("a".to_string(), 1), ("b".to_string(), 2), ("c".to_string(), 3)].iter().cloned().collect(),
            mappings: [("a".to_string(), crate::keyboard::KeyMapping{
                name: "a".to_string(),
                code: 1,
                tap: crate::keyboard::OutputType::Standard(["a".to_string(), "b".to_string(), "c".to_string()].to_vec()),
                hold: crate::keyboard::OutputType::Standard(["d".to_string(), "e".to_string(), "f".to_string()].to_vec()),
                doubletap: crate::keyboard::OutputType::Standard(["g".to_string(), "h".to_string(), "i".to_string()].to_vec()),
                taphold: crate::keyboard::OutputType::Standard(["j".to_string(), "k".to_string(), "l".to_string()].to_vec()),
            }), ("b".to_string(), crate::keyboard::KeyMapping{
                name: "b".to_string(),
                code: 2,
                tap: crate::keyboard::OutputType::Standard(["m".to_string(), "n".to_string(), "o".to_string()].to_vec()),
                hold: crate::keyboard::OutputType::Standard(["p".to_string(), "q".to_string(), "r".to_string()].to_vec()),
                doubletap: crate::keyboard::OutputType::Standard(["s".to_string(), "t".to_string(), "u".to_string()].to_vec()),
                taphold: crate::keyboard::OutputType::Standard(["v".to_string(), "w".to_string(), "x".to_string()].to_vec()),
            }), ("c".to_string(), crate::keyboard::KeyMapping{
                name: "c".to_string(),
                code: 3,
                tap: crate::keyboard::OutputType::Standard(["y".to_string(), "z".to_string(), "1".to_string()].to_vec()),
                hold: crate::keyboard::OutputType::Standard(["2".to_string(), "3".to_string(), "4".to_string()].to_vec()),
                doubletap: crate::keyboard::OutputType::Standard(["5".to_string(), "6".to_string(), "7".to_string()].to_vec()),
                taphold: crate::keyboard::OutputType::Standard(["8".to_string(), "9".to_string(), "0".to_string()].to_vec()),
            })].iter().cloned().collect(),
        };
        let mut entry1 = Entry::new();
        entry1.name = "test".to_string();
        entry1.description = "testing".to_string();
        let mut entry2 = Entry::new();
        entry2.name = "test2".to_string();
        entry2.description = "testing2".to_string();
        entry1.commit().unwrap();
        entry2.commit().unwrap();

        entry1.update_config(&config).unwrap();
        entry2.update_config(&config).unwrap();
        
        save_entries().unwrap();

        unsafe{
        let entries_current = ENTRIES.clone();
        
        load_entries().unwrap();

        assert_eq!(entries_current, ENTRIES);

        let conf1 = ENTRIES[0].load_config().unwrap();
        let conf2 = ENTRIES[1].load_config().unwrap();

        assert_eq!(conf1, config);
        assert_eq!(conf2, config);

        ENTRIES[0].remove().unwrap();
        save_entries().unwrap();
        load_entries().unwrap();
        assert_eq!(ENTRIES.len(), 1);
        
        }
    }
}