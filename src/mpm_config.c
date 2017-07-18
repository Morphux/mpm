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

static config_t *g_mpm_conf = NULL;

void init_config(void) {
    char        *path;
    u8_t        ret;
    config_t    *ptr;

    if (config_get_load_config() != NULL)
    {
        path = strdup(config_get_load_config());
        goto read_config;
    }

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
            goto read_config;

        free(path);
    }

    m_warning("Cannot find a configuration file in the system.\n");
    m_warning("Places mpm search:\n");

    for (size_t i = 0; i < sizeof(g_default_paths) / sizeof(g_default_paths[0]); i++)
        m_info("%s\n", g_default_paths[i]);

    m_panic("You can specify a custom config file via the -c option.");

read_config:
    ptr = parse_config(path, &ret);

    free(path);
    if (ret != 0)
    {
        config_get_error_string(ptr);
        m_error("Error in the configuration: %s\n", ptr->err);
        config_free(&ptr);
        exit(1);
    }

    g_mpm_conf = ptr;

    return ;
}


static void print_main_conf(cfg_t *opt) {
    PRINT_CONF_STR(opt, CONFIG_MAIN_SEC, CONFIG_MAIN_INST_MODE);
    PRINT_CONF_INT(opt, CONFIG_MAIN_SEC, CONFIG_MAIN_THREAD_INST);
}

static void print_log_conf(cfg_t *opt) {
    PRINT_CONF_INT(opt, CONFIG_LOG_SEC, CONFIG_LOG_VERBOSE_LVL);
    PRINT_CONF_STR(opt, CONFIG_LOG_SEC, CONFIG_LOG_DIR);
}

static void print_dl_conf(cfg_t *opt) {
    PRINT_CONF_STR(opt, CONFIG_DL_SEC, CONFIG_DL_MAIN_SITE);
    PRINT_CONF_INT(opt, CONFIG_DL_SEC, CONFIG_DL_THREAD_DL);
    PRINT_CONF_STR(opt, CONFIG_DL_SEC, CONFIG_DL_DIR);
    PRINT_CONF_STR(opt, CONFIG_DL_SEC, CONFIG_DL_MIRROR_LIST);
}

static void print_comp_conf(cfg_t *opt) {
    PRINT_CONF_INT(opt, CONFIG_COMP_SEC, CONFIG_COMP_SBU);
    PRINT_CONF_LIST(opt, CONFIG_COMP_SEC, CONFIG_COMP_CONF_ARG);
    PRINT_CONF_LIST(opt, CONFIG_COMP_SEC, CONFIG_COMP_MAKE_ARG);
}

static void print_pkg_conf(cfg_t *opt) {
    PRINT_CONF_STR(opt, CONFIG_PKG_SEC, CONFIG_PKG_DB);
    PRINT_CONF_STR(opt, CONFIG_PKG_SEC, CONFIG_PKG_TREE);
}

static void print_kern_conf(cfg_t *opt) {
    PRINT_CONF_STR(opt, CONFIG_KERN_SEC, CONFIG_KERN_SRC);
}

static void print_boot_conf(cfg_t *opt) {
    PRINT_CONF_INT(opt, CONFIG_BOOT_SEC, CONFIG_BOOT_MNT_WHEN_NEEDED);
    PRINT_CONF_STR(opt, CONFIG_BOOT_SEC, CONFIG_BOOT_GRUB_DIR);
}

typedef struct {
    char        *str;
    void        (*fn)(cfg_t *);
} list_config_t;

static const list_config_t g_section_names[] = {
    {CONFIG_MAIN_SEC, print_main_conf},
    {CONFIG_LOG_SEC, print_log_conf},
    {CONFIG_DL_SEC, print_dl_conf},
    {CONFIG_COMP_SEC, print_comp_conf},
    {CONFIG_PKG_SEC, print_pkg_conf},
    {CONFIG_KERN_SEC, print_kern_conf},
    {CONFIG_BOOT_SEC, print_boot_conf}
};

static void config_list(void) {
    for (size_t i = 0; i < COUNTOF(g_section_names); i++)
    {
        assert(g_section_names[i].fn != NULL);
        g_section_names[i].fn(cfg_getsec(g_mpm_conf->ptr, g_section_names[i].str));
    }
}

void config_cmd(mlist_t *ptr) {
    if (config_get_list())
    {
        config_list();
        return ;
    }

    if (ptr == NULL)
        m_warning("Config command need at least one parameter\n");

    /* Show the value of a configuration token */
    if (list_size(ptr) == 1)
    {
        char *ret = get_conf_str_from_name(g_mpm_conf, ptr->member);

        /* Unknown token */
        if (ret == NULL)
        {
            m_warning("Unknow token: %s\n", ptr->member);
            return ;
        }
        m_info("%s = %s\n", ptr->member, ret);
    }
}
