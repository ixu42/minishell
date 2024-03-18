#include "../minishell.h"

void	update_termios(int set_echoctl)
{
	struct termios	info;

	if (set_echoctl == SET_ECHOCTL)
	{
		tcgetattr(0, &info);
		info.c_lflag |= ECHOCTL;
		tcsetattr(0, TCSANOW, &info);
	}
	else
	{
		tcgetattr(0, &info);
		info.c_lflag &= ~ECHOCTL;
		tcsetattr(0, TCSANOW, &info);
	}
}

int	set_signals(t_data *data)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	update_termios(UNSET_ECHOCTL);
	last_sig = 0;
	sa_int.sa_handler = &display_pmt_on_nl;
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

int	parent_signal_handler(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	update_termios(SET_ECHOCTL);
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

int	child_signal_handler(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	update_termios(SET_ECHOCTL);
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
	return (0);
}

int	heredoc_signal_handler(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	update_termios(UNSET_ECHOCTL);
	sa_int.sa_handler = &move_to_nl;
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}
