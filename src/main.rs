use std::env;
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
    let values: Vec<String> = re
        .captures_iter(&body)
        .map(|cap| cap[1].to_string())
        .collect();

    let rows: Vec<(String, String)> = values
        .chunks(2)
        .filter_map(|chunk| {
            if chunk.len() == 2 {
                Some((chunk[0].clone(), chunk[1].clone()))
            } else {
                None
            }
        })
        .collect();

    let rank_width: usize = rows.len().to_string().len().max(2);
    let name_width: usize = rows
        .iter()
        .map(|(name, _)| name.chars().count())
        .max()
        .unwrap_or(0)
        .max(11);
    let pct_width: usize = rows
        .iter()
        .map(|(_, pct)| pct.chars().count())
        .max()
        .unwrap_or(0)
        .max(7);

    println!("{} Most Used Languages", username);
    for (index, (name, pct)) in rows.iter().enumerate() {
        println!(
            "{:>rank_width$} - {:<name_width$} [ {:>pct_width$} ]",
            index + 1,
            name,
            pct,
            rank_width = rank_width,
            name_width = name_width,
            pct_width = pct_width
        );
    }

    Ok(())
}
