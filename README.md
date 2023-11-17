# github-readme-stats-cli
Rust CLI tool that takes the tools from the [@github-readme-stats](https://github.com/anuraghazra/github-readme-stats) project and repurposes them for anyone who loves the command line interface.

I decided to create this project because every time I wanted to know the technologies most used by the most competent software engineers in the sector I had to go to [this link](https://github-readme-stats.vercel.app/api/top-langs/?username=AntonioBerna) and manually enter the various data into the URL. Furthermore, this project was a good excuse to get my hands on the Rust programming language which is becoming more and more famous lately.

## mini docs
Thanks to the Rust language, the steps to use my code on your computer are very few. First of all, open the terminal and write:

```bash
git clone https://github.com/AntonioBerna/github-readme-stats-cli.git
```

now use the following command to access the project folder:

```bash
cd github-readme-stats-cli
```

then just use the following command to build the project in your operating system in order to generate the executable file:

```bash
cargo build
```

Once this procedure is finished you can use the program using the following command:

```bash
cargo run
```

In this way you will receive the following message which will help you understand how you should use the program, in particular:

```bash
Usage: target/debug/github-readme-stats-cli <username> <langs_count> <hide>
```

so we can use the program for example on [my profile](https://github.com/AntonioBerna) to see which are the 3 programming languages ​​that I use the most, perhaps excluding for example `HTML`, `CSS`, `JavaScript`, `C#` and `Processing` as follows:

```bash
cargo run AntonioBerna 3 "html,css,javascript,c%23,processing"
```
> **NOTE:** The special character `#` is encoded in ASCII hexadecimal code (click [here](https://ascii.cl/) for more detailed information).

So the final output is the following (at least until you start writing everything in Rust 🧑🏻‍💻):

```bash
AntonioBerna Most Used Languages
 1 - Python    [ 55.37% ]
 2 - C         [ 23.78% ]
 3 - C++       [ 20.85% ]
```

finally if you want to delete the executable in a simple way you can use the following command:

```bash
cargo clean
```
