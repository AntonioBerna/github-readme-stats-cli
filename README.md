# github-readme-stats-cli

C CLI tool that repurposes [github-readme-stats](https://github.com/anuraghazra/github-readme-stats) for terminal usage.

The program fetches the SVG from:

```bash
https://github-readme-stats.vercel.app/api/top-langs/
```

then extracts and prints the language ranking in a readable table.

## Build And Run

Clone and enter the project:

```bash
git clone https://github.com/AntonioBerna/github-readme-stats-cli.git
cd github-readme-stats-cli
```

Build with clang:

```bash
make
```

Run:

```bash
./github-readme-stats-cli
```

Usage:

```text
Usage: ./github-readme-stats-cli <username> <optional=[langs_count], [hide]>
```

Example:

```bash
./github-readme-stats-cli AntonioBerna 3 "html,css,javascript,c%23,processing"
```

Expected output (example):

```text
AntonioBerna Most Used Languages
 1 - Python    [  51.3% ]
 2 - C         [ 25.73% ]
 3 - C++       [ 22.96% ]
```

## Memory Check

Run Valgrind via Makefile:

```bash
make valgrind
```

Clean artifacts:

```bash
make clean
```

> [!NOTE]
> If a hidden language contains `#`, encode it as `%23`.
> For example, [`C#` should be passed as `c%23`](https://upload.wikimedia.org/wikipedia/commons/1/1b/ASCII-Table-wide.svg) in the hide parameter.