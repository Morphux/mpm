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

#include "mpm_config.h"

static const char *g_default_paths[] = {
    "~/.mpm",
    "~/.config/mpm",
    "/etc/mpm",
};

void init_config(void) {
    config_t    *ptr;
    char                *path;

    for (size_t i = 0; i < sizeof(g_default_paths) / sizeof(g_default_paths[0]); i++)
    {
        if (g_default_paths[i][0] == '~')
        {
            asprintf(&path, "%s/%s/" CONFIG_DEF_FN, secure_getenv("HOME"),
                &(g_default_paths[i][1]));
        }
        else
        {
            asprintf(&path, "%s/" CONFIG_DEF_FN, g_default_paths[i]);
        }
        if (file_exist(path) == true)
        {
            u8_t        ret;

            ptr = parse_config(path, &ret);
            if (ptr == NULL)
                printf("Error !\n");
            free(path);
            return ;
        }
        free(path);
    }

    m_warning("Cannot find a configuration file in the system.\n");
    m_warning("Places mpm search:\n");
    for (size_t i = 0; i < sizeof(g_default_paths) / sizeof(g_default_paths[0]); i++)
        m_info("%s\n", g_default_paths[i]);
    m_panic("You can specify a custom config file via the -c option.");
    return ;
}
