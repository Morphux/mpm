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

static const char      *g_commands[] = {
    "install"
};

commands_t      *parse_cmd(mlist_t *args) {
    commands_t  *ret = NULL;
    mlist_t     *tmp;
    char        *str;

    ret = malloc(sizeof(*ret));
    if (ret == NULL)
        return NULL;

    list_for_each(args, tmp, str)
    {
        for (size_t i = 0; i < sizeof(g_commands) / sizeof(g_commands[0]); i++)
        {
            if (strncmp(str, g_commands[i], strlen(g_commands[i])) == 0)
            {
                printf("Command: %s\n", str);
            }
        }
    }
    return ret;
}
