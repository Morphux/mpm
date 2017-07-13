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

#include "options.h"

typedef struct {
    int         verbose;
    char        *directory;
    bool        yes;
    char        *load_config;
    char        *output;
} mpm_options_t;

static mpm_options_t g_mpm_opt = {
    .verbose = 0,
    .directory = NULL,
    .yes = false,
    .load_config = NULL,
    .output = NULL
};

void mpm_config_free(void) {
    free(g_mpm_opt.directory);
    free(g_mpm_opt.load_config);
    free(g_mpm_opt.output);
}

bool config_inc_verbose(const char *s) {
    (void)s;

    if (g_mpm_opt.verbose < CONF_MAX_VERBOSE)
        g_mpm_opt.verbose++;
    return true;
}

int config_get_verbose(void) {
    return g_mpm_opt.verbose;
}

bool config_set_directory(const char *s) {
    if (s != NULL)
        g_mpm_opt.directory = strdup(s);
    return true;
}

char *config_get_directory(void) {
    return g_mpm_opt.directory;
}

bool config_set_yes(const char *s) {
    (void)s;

    g_mpm_opt.yes = true;
    return true;
}

bool config_get_yes(void) {
    return g_mpm_opt.yes;
}

bool config_set_load_config(const char *s) {
    if (s != NULL)
        g_mpm_opt.load_config = strdup(s);
    return true;
}

char *config_get_load_config(void) {
    return g_mpm_opt.load_config;
}

bool config_set_output(const char *s) {
    if (s != NULL)
        g_mpm_opt.output = strdup(s);
    return true;
}

char *config_get_output(void) {
    return g_mpm_opt.output;
}
