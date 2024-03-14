#include "../minishell.h"

static void	signal_handler(int signum)
{
	// printf("Received signal %d\n", signum);
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signals(void)
{
	struct sigaction	sa;
	sa.sa_handler = &signal_handler;
	// sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	// signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}