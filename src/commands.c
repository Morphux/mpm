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
    cmd_list_t  flag;
} command_decl_t;

static const command_decl_t     g_commands[] = {
    {"install", CMD_INSTALL},
    {"update", CMD_UPDATE},
    {"upgrade", CMD_UPGRADE},
    {"config", CMD_CONFIG},
    {"news", CMD_NEWS},
    {"remove", CMD_REMOVE},
    {"purge", CMD_PURGE},
    {"doctor", CMD_DOCTOR}
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
    exit(1);
}

#endif

commands_t      *parse_cmd(mlist_t *args) {
    commands_t  *ret = NULL;
    mlist_t     *tmp;
    char        *str;

    ret = malloc(sizeof(*ret));
    if (ret == NULL)
        return NULL;

    ret->cmd = CMD_NONE;

    list_for_each(args, tmp, str)
    {
        for (size_t i = 0; i < sizeof(g_commands) / sizeof(g_commands[0]); i++)
        {
            if (strncmp(str, g_commands[i].str, strlen(g_commands[i].str)) == 0)
            {
                ret->cmd = g_commands[i].flag;
                printf("Command: %s\n", str);
                break ;
            }
        }
        if (ret->cmd == CMD_NONE)
        {
            free(ret);
#ifndef MPM_SUGG
            m_error("Unknow command: %s\n", str);
#else
            m_warning("Unknown command: %s\n", str);
            commands_suggestion(str);
#endif
        }
    }
    return ret;
}
