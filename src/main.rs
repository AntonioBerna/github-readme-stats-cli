use std::env;
use std::io::{self, Write};
use scraper::Selector;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 || args.len() > 4 {
        eprintln!("Usage: {} <username> <optional=[langs_count], [hide]>", args[0]);
        std::process::exit(1);
    }

    let username: &String = args.get(1).expect("Username not provided");
    let mut langs_count: usize = 5;
    let mut hide: &str = "";

    if args.len() > 2 {
        langs_count = args.get(2).and_then(|s| s.parse().ok()).unwrap_or(5);
    }

    if args.len() > 3 {
        hide = args.get(3).map(|s| s.as_str()).unwrap_or("");
    }

    let url: String = format!("https://github-readme-stats.vercel.app/api/top-langs/?username={}&langs_count={}&hide={}", username, langs_count, hide);
    let body: String = reqwest::blocking::get(&url)?.text()?;
    let document: scraper::Html = scraper::Html::parse_document(&body);
    let mut index: i32 = 1;

    println!("{} Most Used Languages", username);

    let selector_str: &str = ".lang-name";
    let selector: Selector = Selector::parse(selector_str)
        .map_err(|e| format!("Failed to parse selector '{}': {:?}", selector_str, e))?;

    for node in document.select(&selector) {
        let formatted_name: String = node.text().collect::<String>();
        if index % 2 != 0 {
            print!("{:2} - {:11}", (index + 1) / 2, formatted_name);
        } else {
            print!("[ {:7}]\n", formatted_name);
        }
        io::stdout().flush()?;
        index += 1;
    }

    Ok(())
}
