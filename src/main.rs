use std::env;
use std::io::{self, Write};
use regex::Regex;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 || args.len() > 4 {
        eprintln!("Usage: {} <username> <optional=[langs_count], [hide]>", args[0]);
        std::process::exit(1);
    }

    let username = &args[1];
    let langs_count = args.get(2).and_then(|s| s.parse().ok()).unwrap_or(5);
    let hide = args.get(3).map_or("", |s| s.as_str());

    let url = format!("https://github-readme-stats.vercel.app/api/top-langs/?username={}&langs_count={}&hide={}", username, langs_count, hide);
    let body = ureq::get(&url).call()?.into_string()?;
    
    // Use regex to find the language names
    let re = Regex::new(r#"<text.*?>(.*?)</text>"#)?;
    let mut index = 1;

    println!("{} Most Used Languages", username);

    for cap in re.captures_iter(&body) {
        let formatted_name = &cap[1];
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
