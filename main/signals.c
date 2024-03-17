#include "../minishell.h"

int	set_signals(t_data *data)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = &set_sigint;
	// sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	sa_quit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}

// static void	set_last_sig(int signum)
// {
// 	dprintf(2, "in parent: %d\n", signum);
// 	write(2, "debug3\n", 7);
// 	if (signum == SIGINT)
// 		last_sig = SIGINT;
// 	else if (signum == SIGQUIT)
// 		last_sig = SIGQUIT;
// }

int	parent_signal_handler(t_data *data)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// data->act_int.sa_handler = &set_last_sig;
	sa_int.sa_handler = SIG_IGN;
	// sigemptyset(&sigact.sa_mask);
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	sa_quit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}

int	child_signal_handler(t_data * data)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct termios		info;

	// data->act_int.sa_handler = &handle_sigint;
	sa_int.sa_handler = SIG_DFL;
	// sigemptyset(&sa_int.sa_mask);
	// dprintf(2, "debug\n");
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	// dprintf(2, "debug1\n");
	sa_quit.sa_handler = SIG_DFL;
	// sa_quit.sa_handler = &handle_sigquit;
	// sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	tcgetattr(0, &info);
	info.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &info);
	return (0);
}
