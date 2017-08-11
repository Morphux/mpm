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

#include <make_pkg.h>

void make_pkg(mlist_t *args) {
    packer_t    *pkg = NULL;
    mlist_t     *tmp;
    char        *str, *path = "./", *temp;

    if (list_size(args) == 0)
    {
        m_warning("The make-pkg command needs a directory\n");
        return ;
    }

    list_for_each(args, tmp, str)
    {
        pkg = packer_init_dir(str);

        if (pkg == NULL || packer_read_dir(pkg) == false)
        {
            packer_free(pkg);
            goto error;
        }

        if (config_get_directory() != NULL)
            path = config_get_directory();

        if (config_get_output() != NULL)
        {
            asprintf(&temp, "%s/%s", path, config_get_output());
            path = temp;
        }
        else
            asprintf(&path, "%s/%s-%s" PACKER_DEF_EXT, path,
                        pkg->__pkg_name, pkg->__pkg_version);

        if (packer_create_archive(pkg, path) == false)
        {
            packer_free(pkg);
            goto error;
        }
        m_info("Archive has been created: %s\n", path);
        packer_free(pkg);
    }

    return ;

error:
    m_warning("Error: %s\n", GET_ERR_STR());
}
