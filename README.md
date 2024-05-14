# 42_minishell
Project at Hive to build a simple bash-like shell.

## Overview
- The features:
	- History: Store command history and allow users to cycle through it using arrow keys.
	- Pipes and Redirection: Support pipes (|) and input/output redirection (<.>,<<,>>) incuding heredoc.
	- Lists: Support for `&&` and `||` lists. 
	- Signals Handling: Handle signals like Ctrl+C (SIGINT) and Ctrl+D (SIGQUIT).
	- Environment Variables: Allow setting, modifying, and using environment variables.
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
	- print a prompt and read a line from the user
	- parse the line and build a abstract syntax tree (AST)
	- run the command using build AST
	- expansion of the enviromental variables and wildcards in tje arguments and filenames of redirection. 
	- execution of command using execve or buildin function. 

## Installation Guide & Usage

Thank you for your interest in our program! Follow these steps to install it on your system.

### Step 1: Clone or Download Repository

- **Clone**: If you have Git installed, run the following command in your terminal:
  ```bash
  git clone https://github.com/dnapi/42_minishell.git
  ```
- **Download ZIP**: Alternatively, you can download the repository as a ZIP file from the "Code" dropdown menu on the GitHub page.

### Step 2: Navigate to Repository

Navigate to the directory where you cloned or extracted the repository using the `cd` command in your terminal.
```bash
cd 42_minishell
```

### Step 3: Install Dependencies
Before compiling the program, install readline package using `brew install readline` on MacOS, or `sudo apt-get update` and `sudo apt-get install libreadline-dev` on Ubuntu/Debian.

### Step 4: Run Installation Script
```bash
make
```

### Step 5: Run the Program
```bash
./minishell 
```