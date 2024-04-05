# 42_minishell
Project at Hive to build a simple bash-like shell.

## Overview
- The minishell program repeats the following in a loop
	- print a prompt and read a line from the user
	- parse the line and build a tree representation
		- lexical analysis/lexing/tokesnization: convert the string to more meaningful tokens
		- syntax analysis/parsing: parse a command line and build the abstract syntax tree (AST)
			- functions: e.g. parse_cmd, parse_pipe, parse_redir
			- each func will:
				- scan a bunch of tokens
				- return a ptr to the tree it builds
	- execute commands by traversing the tree recursively

- The following builtins are implemented
	- `echo` with option `-n`
	- `cd` with only a relative or absolute path
	- `pwd` with no options
	- `export` with no options
	- `unset` with no options
	- `env` with no options or arguments
	- `exit` with no options

## Usage
Before compiling the program, install readline package using `brew install readline` on MacOS, or `sudo apt-get update` and `sudo apt-get install libreadline-dev` on Ubuntu/Debian.