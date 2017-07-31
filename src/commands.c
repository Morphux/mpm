/*********************************** LICENSE **********************************\
*                            Copyright 2017 Morphux                            *
*                                                                              *
*        Licensed under the Apache License, Version 2.0 (the "License");       *
*        you may not use this file except in compliance with the License.      *
*                  You may obtain a copy of the License at                     *
*                                                                              *
*                 http://www.apache.org/licenses/LICENSE-2.0                   *
*                                                                              *
*      Unless required by applicable law or agreed to in writing, software     *
*       distributed under the License is distributed on an "AS IS" BASIS,      *
*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*        See the License for the specific language governing permissions and   *
*                       limitations under the License.                         *
\******************************************************************************/

#include <commands.h>

typedef struct {
    char        *str;
    char        *help;
    void        (*fn)(mlist_t *);
} command_decl_t;

static const command_decl_t     g_commands[] = {
    {
        .str = "install",
        .help = "Install a package. You can either install a local .mpx archive\n\
or request one from the package server, with a name\n\
Examples:\n\
    - sudo mpm install system/ls\n\
    - sudo mpm install /tmp/ls-2.4.mpx"
    },
    {
        .str = "update",
        .help = "Update local database with new packages.\n\
This commands doest not update any package, see upgrade for that"
    },
    {
        .str = "upgrade",
        .help = "Upgrade local packages when possible\n\
By default, this command will upgrade all the packages on the system\n\
    - sudo mpm upgrade\n\
You can specify a package to upgrade:\n\
    - sudo mpm upgrade some_package\n\
Or via globbing:\n\
    - sudo mpm upgrade \"all_pkg_*\"\n\
Note that the double quotes '\"' are important"
    },
    {
        .str = "config",
        .help = "List, create change global configuration values",
        .fn = config_cmd
    },
    {
        .str = "news",
        .help = "List news on installed packages\n\
You can specify a package after the command:\n\
    - sudo mpm news pkg"
    },
    {
        .str = "remove",
        .help = "Remove a package from the system\n\
    - sudo mpm remove package\n\
This command will only remove binaries and libraries, configuration will be keeped\n\
See purge for more information"
    },
    {
        .str = "purge",
        .help = "Purge a package from the system\n\
    - sudo mpm purge package\n\
This command will remove binaries, libraries and every file created by the package\n\
See remove for more information"
    },
    {
        .str = "doctor",
        .help = "Fix the system\n\
This command will try to fix the system in certain ways:\n\
    - Fix file rights on known system files\n\
    - Check for orphaned packages, and fix them"
    },
    {
        .str = "make-pkg",
        .help = "Create a Morphux Package Archive (MPX)",
        .fn = make_pkg
    }
};

#ifdef MPM_SUGG

void    commands_suggestion(const char *str) {
    int         highest_score = 0;
    int         score_tmp = 0;

    for (size_t i = 0; i < sizeof(g_commands) / sizeof(g_commands[0]); i++)
    {
        int     current_score = 0;

        for (size_t h = 0; g_commands[i].str[h] != '\0'; h++)
        {
            for (size_t j = 0; str[j] != '\0'; j++)
            {
                if (g_commands[i].str[h] == str[j])
                {
                    if (i == h)
                        current_score += 2;
                    else
                        current_score++;

                    break;
                }
            }
        }
        if (score_tmp < current_score)
        {
            highest_score = i;
            score_tmp = current_score;
        }

    }
    m_info("Did you mean: 'mpm %s' ?\n", g_commands[highest_score].str);
}

#endif

void command_help(void) {
    size_t j;

    printf("\n");
    for (size_t i = 0; i < sizeof(g_commands) / sizeof(g_commands[0]); i++)
    {
        m_info("\e[1m%s\e[0m", g_commands[i].str);
        for (j = 0; j + strlen(g_commands[i].str) < 15; j++, printf(" "))
            ;

        printf(": ");
        for (size_t h = 0, k = 0; g_commands[i].help[h] != '\0'; h++, k++)
        {
            if (k + j > 80 || g_commands[i].help[h] == '\n')
            {
                printf("\n  ");
                for (k = 0; k < 15; k++, printf(" "))
                    ;
                k = 0;
                if (g_commands[i].help[h] == '\n')
                {
                    if (h + 1 == strlen(g_commands[i].help))
                        break ;
                    h++;
                }
                printf(": ");
            }
            printf("%c", g_commands[i].help[h]);
        }

        if (i + 1 < sizeof(g_commands) / sizeof(g_commands[0]))
            printf("\n\n");
        else
            printf("\n");

        fflush(stdout);
    }
    exit(1);
}

void parse_cmd(mlist_t *args) {
    mlist_t     *tmp;
    char        *str;

    if (args == NULL)
    {
        m_warning("Mpm need at least one action\n");
        command_help();
    }
    list_for_each(args, tmp, str)
    {
        for (size_t i = 0; i < sizeof(g_commands) / sizeof(g_commands[0]); i++)
        {
            if (strncmp(str, g_commands[i].str, strlen(g_commands[i].str)) == 0)
            {
                list_del(args, str, strlen(str), NULL);
                assert(g_commands[i].fn != NULL);
                g_commands[i].fn(args);
                list_free(args, NULL);
                return ;
            }
        }
        m_warning("Unknow command: %s\n", str);
#ifdef MPM_SUGG
        commands_suggestion(str);
#endif
        command_help();
    }
}
