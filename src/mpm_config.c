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

static void print_single_option(const char *name)
{
    cfg_opt_t       *opt = NULL;

    opt = get_opt_from_name(g_mpm_conf, name);
    if (opt == NULL)
    {
        m_warning(MPM_CONF_UNKN_TOKEN_FMT, name);
        return ;
    }

    m_info("%s = ", name);

    for (size_t i = 0; i < cfg_opt_size(opt); i++)
    {
        switch (opt->type)
        {
            case CFGT_STR:
                fprintf(stdout, "%s", cfg_opt_getnstr(opt, i));
                break;
            case CFGT_INT:
                fprintf(stdout, "%ld", cfg_opt_getnint(opt, i));
                break;
            default:
                assert(!"Unknow config type");
        }

        if (i + 1 < cfg_opt_size(opt))
            fprintf(stdout, ", ");
    }
    fprintf(stdout, "\n");
}

static void add_single_opt_val(const char *token, const char *val, const char *sign) {
    cfg_opt_t           *opt = NULL;
    unsigned long       tmp = 0;
    char                *end_ptr = NULL;
    u8_t                index = 0;

    assert(token != NULL);
    assert(val != NULL);

    opt = get_opt_from_name(g_mpm_conf, token);
    if (opt == NULL)
    {
        m_warning(MPM_CONF_UNKN_TOKEN_FMT, token);
        return ;
    }

    if (strcmp(sign, "+=") == 0)
        index = cfg_opt_size(opt);
    else if (strcmp(sign, "-=") == 0)
    {
        bool del = false;

        for (size_t i = 0; i < cfg_opt_size(opt); i++)
        {
            if (strcmp(cfg_opt_getnstr(opt, i), val) == 0)
            {
                del = true;
                for (size_t j = i; j + 1 < cfg_opt_size(opt); j++)
                {
                    cfg_opt_setnstr(opt, cfg_opt_getnstr(opt, j + 1), j);
                }
                free(opt->values[--opt->nvalues]);
                --i;
            }
        }

        if (del == false)
            m_warning("Could not find value '%s' in token '%s'\n", val, token);
        return ;
    }
    else
    {
        for (size_t j = 0; j < opt->nvalues; j++)
            free(opt->values[j]);

        opt->nvalues = 0;
        free(opt->values);
        opt->values = NULL;
    }

    switch (opt->type)
    {
        case CFGT_STR:
            /* Can't set the value */
            if (cfg_opt_setnstr(opt, val, index) != CFG_SUCCESS)
            {
                m_warning("Can't set the token %s\n", token);
                return ;
            }
            break;
        case CFGT_INT:
            tmp = strtoul(val, &end_ptr, 10);

            /* Value is not an integer */
            if (val == end_ptr)
            {
                m_warning("The following value is not an integer: %s\n", val);
                return ;
            }

            /* Can't set the value */
            if (cfg_opt_setnint(opt, tmp, index) != CFG_SUCCESS)
            {
                m_warning("Can't set the token %s\n", token);
                return ;
            }
            break;
        default:
            assert(!"Unknown config type");

    }
    g_mpm_conf->need_save = true;
}

static const char *g_sign_list[] = {
    "+=",
    "-=",
    "="
};

void config_cmd(mlist_t *ptr) {
    if (config_get_list())
    {
        config_list();
        return ;
    }

    if (ptr == NULL)
        m_warning("Config command need at least one parameter\n");

    /* Show the value of one configuration token */
    if (list_size(ptr) == 1)
    {
        print_single_option(ptr->member);
    }
    else if (list_size(ptr) == 2)
    {
        add_single_opt_val(ptr->member, ptr->next->member, NULL);
    }
    else if (list_size(ptr) == 3)
    {
        char    *sign = ptr->next->member;
        size_t  i;

        for (i = 0; i < COUNTOF(g_sign_list); i++)
        {
            if (strcmp(g_sign_list[i], sign) == 0)
                break ;
        }
        if (i == COUNTOF(g_sign_list))
        {
            m_warning("Unknown sign: %s\n", sign);
            return ;
        }

        add_single_opt_val(ptr->member, ptr->next->next->member, g_sign_list[i]);
    }
    else
    {
        m_warning("Too many parameters for the config command\n");
        return ;
    }

    /* Saving config, if needed */
    if (g_mpm_conf->need_save && config_get_dry_run() == false)
    {
        FILE *fp = fopen(g_mpm_conf->fn, "w+");

        if (fp == NULL)
        {
            m_warning("Cannot write configuration in the file: %s\n", g_mpm_conf->fn);
            return ;
        }
        cfg_print(g_mpm_conf->ptr, fp);
        fclose(fp);
        g_mpm_conf->need_save = false;
    }
}
