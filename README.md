# 42_minishell
Project at Hive

## installing readline package on MacOS
```
brew install readline
```

## installing readline package on Ubuntu/Debian
```
sudo apt-get update
sudo apt-get install libreadline-dev
```

## Shell repeats the following in a loop
- print a prompt
- read a line
- parse it and build a tree representation
- execute

## Summary and division of work
### frontend
- convert user input to a string (Ivy)
  - display a prompt when waiting for a new command
  - have a working history
- signal handling (Ivy)
  - handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash
  - in interactive mode:
    - ctrl-C displays a new prompt on a new line
    - ctrl-D exits the shell
    - ctrl-\ does nothing
- lexical analysis/lexing/tokesnization: convert the string to more meaningful tokens (Alex)
- syntax analysis/parsing: parse a command line and build AST (Alex)
  - funcs: e.g. parse_cmd, parse_pipe, parse_redir
  - each func will:
    - scan a bunch of tokens
    - return a ptr to the tree it builds
### backend
- execution of commands by traversing AST recursively (Ivy)
- builtins (Ivy)
  - `echo` with option `-n`
  - `cd` with only a relative or absolute path
  - `pwd` with no options
  - `export` with no options
  - `unset` with no options
  - `env` with no options or arguments
  - `exit` with no options

## Test cases
```
echo hello world\n
echo 'hello world\n'
echo -e 'hello world\n'
echo "hello world\n"
echo -e "hello world\n"

touch tmp.txt | ls *.txt

cat
cat | ls
cat | cat | ls
```

## checklist (can be handled after we have a functional program)
- are functions protected
  - e.g. in case of failure of malloc, write, pipe, fork, waitpid, etc., (1) print error message properly, (2) free all heap allocated memory, and (3) exit with correct exit code
- error handling to be agreed and improved
- norminette compliance, incl. 42 header
- if scope of function is limited to the file, define it as static function
