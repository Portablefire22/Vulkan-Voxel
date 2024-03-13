use std::{
    env::var_os,
    fs::{self, read, read_dir, File},
    io,
    path::{Path, PathBuf},
};

use zip::ZipArchive;

pub fn rip_minecraft_textures(args: &Vec<String>) {
    let version: &String;
    if args.len() >= 2 {
        version = &args[2];
    } else {
        println!("No minecraft version specified!\nARGS: --extract-textures <MINECRAFT_VERSION>");
        return;
    }
    // TODO Add these
    let path = root_path(version);
    match fs::metadata(&path) {
        Err(_) => {
            eprintln!("Jar not found!: {:#?} does not exist!", &path);
            return;
        }
        Ok(_) => println!("Located: {}.jar", version),
    }

    println!("Opening: {:?}", &path);
    let jar_file = File::open(&path).unwrap();
    let mut archive = ZipArchive::new(jar_file).unwrap();
    let mut count = 0;

    for i in 0..archive.len() {
        let mut file = archive.by_index(i).unwrap();

        let path = PathBuf::from(match file.name().find('\0') {
            Some(index) => &file.name()[0..index],
            None => file.name(),
        });

        if file.name().starts_with("assets/minecraft") {
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            let mut out_file = File::create(&path).unwrap();
            io::copy(&mut file, &mut out_file).unwrap();
            count += 1;
        }
    }

    println!("Extracted {} files", count);
}

#[cfg(target_os = "linux")]
fn root_path(version: &str) -> PathBuf {
    let home = std::env::var_os("HOME").unwrap();
    let mut buf = PathBuf::from(home);
    buf.push(".minecraft/versions/");
    buf.push(version);
    buf.push(format!("{}.jar", version));
    buf
}
#[cfg(target_os = "windows")]
fn root_path(version: &str) -> PathBuf {}
#[cfg(target_os = "macos")]
fn root_path(version: &str) -> PathBuf {}
