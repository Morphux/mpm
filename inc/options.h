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

#ifndef OPTIONS_H
# define OPTIONS_H

# include <mpm.h>

# define CONF_MAX_VERBOSE 3

/*!
 * \brief Free the options allocations
 */
void mpm_config_free(void);

/*!
 * \brief Increment verbose level
 * \note The parameter is ignored
 */
bool config_inc_verbose(const char *s);

/*!
 * \brief Set the directory option
 */
bool config_set_directory(const char *s);

/*!
 * \brief Set the 'yes' (skip input) to true
 * \note The parameter is ignored
 */
bool config_set_yes(const char *s);

/*!
 * \brief Set the load config file path
 */
bool config_set_load_config(const char *s);

/*!
 * \brief Get the option 'directory'
 */
char *config_get_directory(void);

/*!
 * \brief Get the option 'load-config'
 */
char *config_get_load_config(void);

/*!
 * \brief Get verbose level
 */
int config_get_verbose(void);

/*!
 * \brief Get the value of the 'yes' option
 */
bool config_get_yes(void);

/*!
 * \brief Set output option
 */
bool config_set_output(const char *s);

/*!
 * \brief Get output option
 */
char *config_get_output(void);

/*!
 * \brief Set list option
 */
bool config_set_list(const char *s);

/*!
 * \brief Get list option
 */
bool config_get_list(void);

#endif /* OPTIONS_H */
