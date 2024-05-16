# 42_minishell
First group project at Hive to build a simple bash-like shell, collaborated with [dnapi](https://github.com/dnapi).

## Overview
- Features:
	- History: Store command history and allow users to cycle through it using arrow keys.
	- Pipes and Redirection: Support pipes `|` and input/output redirection (`<`, `>`, `<<`, `>>`) including heredoc.
	- Lists: Support `&&` and `||` list operators.
	- Signal Handling: Handle signals such as Ctrl+C (SIGINT) and Ctrl+D (SIGQUIT).
	- Environment Variables: Enable setting, modifying, and using environment variables.
	- Wildcard: Expanding * for files in the current directory.

- The following builtins are implemented
	- `echo` with option `-n`
	- `cd` with only a relative or absolute path
	- `pwd` with no options
	- `export` with no options
	- `unset` with no options
	- `env` with no options or arguments
	- `exit` with no options

- The minishell program repeats the following in a loop
	- Print a prompt and read a line from the user.
	- Parse the line and build an abstract syntax tree (AST).
	- Run the command using the AST.
		- Expansion of the environment variables and wildcards in the arguments and filenames of redirection.
		- Execution of command using execve or functions implementing builtins.

## Installation Guide & Usage

Thank you for your interest in our program! Follow these steps to install it on your system.

### Step 1: Clone or Download Repository
- **Clone**: If you have Git installed, run the following command in your terminal:
  ```bash
  git clone https://github.com/ixu42/minishell.git
  ```
- **Download ZIP**: Alternatively, you can download the repository as a ZIP file from the "Code" dropdown menu on the GitHub page.

### Step 2: Navigate to Repository
Navigate to the directory where you cloned or extracted the repository using the `cd` command in your terminal.
```bash
cd minishell
```

### Step 3: Install Dependencies
Before compiling the program, install readline package using `brew install readline` on MacOS, or `sudo apt-get update` and `sudo apt-get install libreadline-dev` on Ubuntu/Debian.

### Step 4: Compile the Program
```bash
make
```

### Step 5: Run the Program
```bash
./minishell
```