#include "../minishell.h"

static void	signal_handler(int signum)
{
	// printf("Received signal %d\n", signum);
	if (signum == SIGINT)
	{
		last_sig = SIGINT;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	set_signals(void)
{
	struct sigaction	sigact;

	sigact.sa_handler = &signal_handler;
	// sigemptyset(&sigact.sa_mask);
	// sigact.sa_flags = SA_RESTART;
	sigact.sa_flags = 0;
	if (sigaction(SIGINT, &sigact, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	sigact.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sigact, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}
