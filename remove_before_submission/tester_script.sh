#!/bin/bash
make re
norminette main
norminette parser
norminette strings
norminette exec
norminette libft

cp minishell minishell_bonus
cd minishell_tester
./tester
./tester wildcards
./tester syntax
./tester os_specific
