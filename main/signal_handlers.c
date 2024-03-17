#include "../minishell.h"

void	set_sigint(int signum)
{
	// printf("Received signal %d\n", signum);
	if (signum == SIGINT)
	{
		// last_sig = SIGINT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// static void	handle_sigquit(int signum)
// {
// 	printf("received sig: %d\n", signum);
// 	write(2, "debug3\n", 7);
// 	last_sig = SIGQUIT;
// 	write(1, "Quit: 3\n", 8); // fd?
// 	exit(131);
// }

void	handle_sigint(int signum)
{
	(void)signum;
	// printf("received sig: %d\n", signum);
	write(2, "debug2\n", 7);
	last_sig = SIGINT;
	exit(130);
}
