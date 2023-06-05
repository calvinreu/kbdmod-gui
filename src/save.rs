use crate::keyboard::{VirtualKeyboard};
use std::fs::{File, remove_file, read_dir};
use std::collections::BTreeMap as Map;
use std::io::{BufReader, BufWriter, Write, Error};

pub fn save_config(path: String, config: &VirtualKeyboard)
-> Result<(),Error> {
    let mut buffer = BufWriter::new(File::create(path)?);
    serde_json::to_writer(&mut buffer, &config)?;
    buffer.flush()?;
    Ok(())
}

fn load_config(path: String) -> Result<Map<String, VirtualKeyboard>,Error> {
    let mut configs : Map<String, VirtualKeyboard> = Map::new();
    let directory = read_dir(path)?;
    for entry in directory {
        let entry = match entry {
            Ok(entry) => entry,
            Err(e) => {
                eprintln!("Error listing files: {0}", e);
                continue;
            },
        };
        let path = entry.path();
        let name = match path.file_name(){
            Some(name) => name,
            None => {
                continue;
            },
        };
        let extension = match path.extension() {
            Some(extension) => if extension == "json" {
                ".json"
            } else {
                continue;
            },
            None => {
                continue;
            },
        };
        let name = name.to_str().unwrap().trim_end_matches(extension);
        let file = match File::open(path.clone()) {
            Ok(file) => file,
            Err(e) => {
                eprintln!("Error opening file: {0} {1}", name, e);
                continue;
            },
        };
        let buffer = BufReader::new(file);
        let config: VirtualKeyboard = match serde_json::from_reader(buffer)
        {
            Ok(config) => config,
            Err(e) => {
                eprintln!("Error reading file: {0} {1}", name, e);
                continue;
            },
        }; 
        configs.insert(name.to_string(), config);
    }
    Ok(configs)
}

fn delete_config(path: String) -> Result<(),Error> {
    remove_file(path)?;
    Ok(())
}

#[cfg(test)]
mod tests {
    const FOLDER : &str = "/tmp/kbdmod-gui-test";
    use super::*;
    use std::fs::{create_dir, remove_dir_all};
    #[test]
    fn test_saveload() {
        _ = remove_dir_all(FOLDER);
        create_dir(FOLDER).unwrap();
        let path = format!("{}/test.json", FOLDER);
        File::create(path.clone()).unwrap();
        delete_config(path).unwrap();
        assert_eq!(read_dir(FOLDER).unwrap().count(), 0);
        _ = remove_dir_all(FOLDER);
        create_dir(FOLDER).unwrap();
        let path = format!("{}/test1.json", FOLDER);
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
        save_config(path, &config).unwrap();
        let path = format!("{}/test2.json", FOLDER);
        save_config(path, &config).unwrap();
        let config_loaded = load_config(FOLDER.to_string()).unwrap();
        assert_eq!(config_loaded["test1"], config);
        assert_eq!(config_loaded["test2"], config);
    }
}