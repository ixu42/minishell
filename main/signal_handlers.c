#include "../minishell.h"

/* SIGINT signal handler when waiting 
for user inputting command line */

void	display_pmt_on_nl(int signum)
{
	last_sig = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line(); // signal safety?
	rl_replace_line("", 0);
	rl_redisplay();
}

// SIGINT signal handler for here documents

void	move_to_nl(int signum)
{
	last_sig = SIGINT;
	// dprintf(2, "0.11\n");
	write(1, "\n", 1);
	// dprintf(2, "last_sig - %d\n", last_sig);
}
