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

#include <extract_pkg.h>

void extract_pkg(mlist_t *args) {
    packer_t    *ptr = NULL;
    bool        ret;

    if (list_size(args) == 0)
    {
        m_warning("The make-pkg command need a path\n");
        return ;
    }
    ptr = packer_init_archive(args->member);
    ret = packer_extract_archive(ptr,
        config_get_output() ? config_get_output() : "./");

    if (ret != true)
        m_warning("Error: %s\n", GET_ERR_STR());
    else
    {
        m_info("Archive '%s' has been sucessfully extracted to '%s'\n",
            args->member, (config_get_output() ? config_get_output() : "./"));
    }
    packer_free(ptr);
}
