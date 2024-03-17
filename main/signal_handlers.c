#include "../minishell.h"

/* SIGINT signal handler when waiting 
for user inputting command line */

void	display_pmt_on_nl(int signum)
{
	last_sig = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// void	handle_sigint(int signum)
// {
// 	(void)signum;
// 	printf("received sig: %d\n", signum);
// 	write(2, "debug2\n", 7);
// 	ast_sig = SIGINT;
// 	exit(130);
// }

// static void	handle_sigquit(int signum)
// {
// 	printf("received sig: %d\n", signum);
// 	write(2, "debug3\n", 7);
// 	last_sig = SIGQUIT;
// 	write(1, "Quit: 3\n", 8); // fd?
// 	exit(131);
// }

// SIGINT signal handler for here documents

void	move_to_nl(int signum)
{
	last_sig = SIGINT;
	write(1, "\n", 1);
}
