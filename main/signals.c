#include "../minishell.h"

static void	update_termios(int set_echoctl)
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

int	set_signals_interactive(t_data *data)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	update_termios(UNSET_ECHOCTL);
	last_sig = 0;
	sa_int.sa_handler = &display_pmt_on_nl;
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}

int	set_default_signals(t_data *data)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	update_termios(SET_ECHOCTL);
	last_sig = 0;
	sa_int.sa_handler = SIG_DFL;
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	sa_quit.sa_handler = SIG_DFL;
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) 
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}

int	ignore_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_IGN;
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
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror(ERR_SIGACTION);
		return (1);
	}
	return (0);
}
