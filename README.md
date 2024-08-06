# github-readme-stats-cli

Rust CLI tool that takes the tools from the [github-readme-stats](https://github.com/anuraghazra/github-readme-stats) project and repurposes them for anyone who loves the command line interface.

I decided to create this project because every time I wanted to know the technologies most used by the most competent software engineers in the sector I had to go to [this link](https://github-readme-stats.vercel.app/api/top-langs/?username=AntonioBerna) and manually enter the various data into the URL. Furthermore, this project was a good excuse to get my hands on the Rust programming language which is becoming more and more famous lately.

## mini docs

Thanks to the Rust language, the steps to use my code on your computer are very few. First of all, open the terminal and write:

```
git clone https://github.com/AntonioBerna/github-readme-stats-cli.git
```

now use the following command to access the project folder:

```
cd github-readme-stats-cli
```

then just use the following command to build the project in your operating system in order to generate the executable file:

```
cargo build
```

Once this procedure is finished you can use the program using the following command:

```
cargo run
```

In this way you will receive the following message which will help you understand how you should use the program, in particular:

```
Usage: target/debug/github-readme-stats-cli <username> <optional=[langs_count], [hide]>
```

so we can use the program for example on [my profile](https://github.com/AntonioBerna) to see which are the 3 programming languages ​​that I use the most, perhaps excluding for example `HTML`, `CSS`, `JavaScript`, `C#` and `Processing` as follows:

```
cargo run AntonioBerna 3 "html,css,javascript,c%23,processing"
```

> [!NOTE]
> The special character `#` is encoded in ASCII hexadecimal code (click [here](https://upload.wikimedia.org/wikipedia/commons/1/1b/ASCII-Table-wide.svg) for more detailed information).

So the final output is the following (at least until you start writing everything in Rust 🧑🏻‍💻):

```
AntonioBerna Most Used Languages
 1 - Python    [ 51.3%  ]
 2 - C         [ 25.73% ]
 3 - C++       [ 22.96% ]
```

finally if you want to delete the executable in a simple way you can use the following command:

```
cargo clean
```

## How to benefit from optimizations

To benefit from the optimizations, introduced with the latest updates, you need to generate the executable file with the following command:

```
cargo build --release
```

once this procedure is finished you can use the program using the following command:

```
./target/release/github-readme-stats-cli
```

so as to obtain:

```
Usage: ./target/release/github-readme-stats-cli <username> <optional=[langs_count], [hide]>
```

from now on everything is similar to before with the difference that the size of the executable has been drastically reduced. Finally if you want to delete the executable in a simple way you can use the following command:

```
cargo clean
```