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
- signal handling (Ivy)
- lexical analysis/lexing/tokenization: convert the string to more meaningful tokens (Alex)
- syntax analysis/parsing: parse a command line and build AST (Alex)
  - funcs: e.g. parse_cmd, parse_pipe, parse_redir
  - each func will: 
    - scan a bunch of tokens
    - return a ptr to the tree it builds
### backend
- execution of commands by traversing AST recursively (Ivy)
- builtins (Ivy)