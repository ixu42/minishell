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

int	parent_signal_handler(void)
{
	struct sigaction	sigact;

	sigact.sa_handler = SIG_IGN;
	// sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	if (sigaction(SIGINT, &sigact, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	if (sigaction(SIGQUIT, &sigact, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}

static void	exit_child(int signum)
{
	printf("received sig: %d\n", signum);
	if (signum == SIGINT)
	{
		write(2, "debug2\n", 7);
		last_sig = SIGINT;
		exit(signum);
	}
}

int	child_signal_handler(void)
{
	struct sigaction	sigact;

	sigact.sa_handler = &exit_child;
	// sigemptyset(&sigact.sa_mask);
	dprintf(2, "debug\n");
	sigact.sa_flags = 0;
	if (sigaction(SIGINT, &sigact, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	dprintf(2, "debug1\n");
	if (sigaction(SIGQUIT, &sigact, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}
