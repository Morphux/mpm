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

#include <mpm.h>

static const mopts_t    g_args[] = {
    {
        .opt = 'v',
        .s_opt = "verbose",
        .desc = "Use verbose mode",
        .callback = config_inc_verbose
    },
    {
        .opt = 'd',
        .s_opt = "directory",
        .desc = "Set a directory",
        .take_arg = true,
        .usage = "directory",
        .callback = config_set_directory
    },
    {
        .opt = 'y',
        .s_opt = "yes",
        .desc = "Skip any user input, and choose the default option",
        .callback = config_set_yes
    },
    {
        .opt = 'c',
        .s_opt = "load-config",
        .desc = "Load configuration from a file",
        .take_arg = true,
        .usage = "file",
        .callback = config_set_load_config
    },
    {
        .opt = 'o',
        .s_opt = "output",
        .desc = "Output to a file",
        .take_arg = true,
        .usage = "file",
        .callback = config_set_output
    },
    {
        .opt = 'l',
        .s_opt = "list",
        .desc = "List everything (Depends on command passed)",
        .callback = config_set_list
    },
    {
        .s_opt = "dry-run",
        .desc = "Do not write anything",
        .callback = config_set_dry_run
    },
    {
        .s_opt = "thread-install",
        .desc = "Install package concurrently, if possible",
        .callback = config_set_thread_install
    },
    {
        .s_opt = "thread-dl",
        .desc = "Download multiple packages at the same time",
        .callback = config_set_thread_dl
    },
    ARGS_EOL
};

int main(int ac, char **av) {
    mlist_t     *args = NULL;

    set_program_name(NAME);
    set_version("0.1");
    set_maintainer(AUTH);
    read_opt(ac, av, g_args, &args);

    init_config();
    mpm_apply_options();
    parse_cmd(args);

    mpm_config_free();
    return 0;
}
