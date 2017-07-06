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

#ifndef COMMANDS_H
# define COMMANDS_H

#include <mpm.h>

typedef enum {
    CMD_NONE = 0,
    CMD_INSTALL,
    CMD_UPDATE,
    CMD_UPGRADE,
    CMD_CONFIG,
    CMD_NEWS
} cmd_list_t;

typedef struct {
    cmd_list_t          cmd;
    mlist_t             *args;
} commands_t;

/*!
 * \brief Parse mpm main commands
 *
 * \param args Chained list of arguments, after flag parsing
 *
 * \note This function, on error, can make some score testing for suggestion.
 * Mpm needs to be compiled with MPM_SUGG for that.
 *
 * \return A command_t pointer, with the rest of the arguments, and the
 * command type in the cmd member (cmd_list_t).
 */
commands_t      *parse_cmd(mlist_t *args);

#endif /* COMMANDS_H */
