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

#ifndef MPM_CONFIG_H
# define MPM_CONFIG_H

# include <mpm.h>

# define PRINT_CONF_STR(opt, m_sec, str_opt) m_info(m_sec "." str_opt " = %s\n", cfg_getstr(opt, str_opt));
# define PRINT_CONF_INT(opt, m_sec, str_opt) m_info(m_sec "." str_opt " = %ld\n", cfg_getint(opt, str_opt));
# define PRINT_CONF_LIST(opt, m_sec, str_opt) m_info(m_sec "." str_opt " = "); \
    for (size_t __i = 0; __i < cfg_size(opt, str_opt); __i++) \
    { \
        fprintf(stdout, "%s", cfg_getnstr(opt, str_opt, __i)); \
        if (__i + 1 < cfg_size(opt, str_opt)) \
            fprintf(stdout, ", "); \
    } \
    fprintf(stdout, "\n");

# define MPM_CONF_UNKN_TOKEN_FMT "Unknow token: %s\n"

/*!
 * \brief Init configuration
 *
 * \note This function will try to read a configuration from different
 * directories. If the option 'c' is passed, the function will read
 * from that path.
 */
void init_config(void);

/*!
 * \brief Config cmd
 *
 * \param ptr Takes a pointer to the head of a list (can be NULL)
 */
void config_cmd(mlist_t *ptr);

#endif /* CONFIG_H */
