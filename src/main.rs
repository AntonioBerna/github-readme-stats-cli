use std::env;
use std::io::{self, Write};
use regex::Regex;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 || args.len() > 4 {
        eprintln!("Usage: {} <username> <optional=[langs_count], [hide]>", args[0]);
        std::process::exit(1);
    }

    let username: &String = &args[1];
    let langs_count: i32 = args.get(2).and_then(|s| s.parse().ok()).unwrap_or(5);
    let hide: &str = args.get(3).map_or("", |s| s.as_str());

    let url: String = format!("https://github-readme-stats.vercel.app/api/top-langs/?username={}&langs_count={}&hide={}", username, langs_count, hide);
    let body: String = ureq::get(&url).call()?.into_string()?;
    
    let re: Regex = Regex::new(r#"<text.*?>(.*?)</text>"#)?;
    let mut index: i32 = 1;

    println!("{} Most Used Languages", username);
    for cap in re.captures_iter(&body) {
        let formatted_name: &str = &cap[1];
        if index % 2 != 0 {
            print!("{:2} - {:11}", (index + 1) / 2, formatted_name);
        } else {
            println!("[ {:7}]", formatted_name);
        }
        io::stdout().flush()?;
        index += 1;
    }

    Ok(())
}
