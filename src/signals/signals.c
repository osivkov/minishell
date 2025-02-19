





#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include <signal.h>


void    handle_sigint(int sig)
{
    (void)sig;

    g_signal_status = 1;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}