use select::document::Document;
use select::predicate::Class;
use std::env;
use std::io::{self, Write};
use reqwest;

#[tokio::main]
async fn main() -> Result<(), reqwest::Error> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 || args.len() > 4 {
        eprintln!("Usage: {} <username> <optional=[langs_count], [hide]>", args[0]);
        std::process::exit(1);
    }

    let username: &String = args.get(1).expect("Username not provided");
    let mut langs_count: usize = 5;
    let mut hide: &str = "";
    
    if args.len() > 2 {
        langs_count = args.get(2).and_then(|s: &String| s.parse().ok()).unwrap_or(5);
    }

    if args.len() > 3 {
        hide = args.get(3).map(|s: &String| s.as_str()).unwrap_or("");
    }

    let url: String = format!("https://github-readme-stats.vercel.app/api/top-langs/?username={username}&langs_count={langs_count}&hide={hide}");
    let body: String = reqwest::get(&url).await?.text().await?;
    let document: Document = Document::from(body.as_str());
    let mut index: i32 = 1;

    println!("{username} Most Used Languages");
    for node in document.find(Class("lang-name")) {
        let formatted_name: String = node.text();
        if index % 2 != 0 {
            print!("{:2} - {:10}", (index + 1) / 2, formatted_name);
        } else {
            print!("[ {:7}]\n", formatted_name);
        }
        io::stdout().flush().unwrap();
        index += 1;
    }

    Ok(())
}
