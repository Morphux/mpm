#include "main.h"

TEST(config_inc_verbose) {
    TEST_ASSERT(config_get_verbose() == 0, "Default value is wrong");
    TEST_ASSERT(config_inc_verbose(NULL), "The function did not return true");
    TEST_ASSERT(config_get_verbose() == 1, "Value is wrong");
    TEST_ASSERT(config_inc_verbose(NULL), "The function did not return true");
    TEST_ASSERT(config_inc_verbose(NULL), "The function did not return true");
    TEST_ASSERT(config_inc_verbose(NULL), "The function did not return true");
    TEST_ASSERT(config_get_verbose() == 3, "Max value is wrong");
    return TEST_SUCCESS;
}

TEST(config_set_directory) {
    TEST_ASSERT(config_set_directory("Yup."), "Unexpected return");
    TEST_ASSERT(strcmp(config_get_directory(), "Yup.") == 0, "Values are not the same");
    mpm_config_free();
    return TEST_SUCCESS;
}

TEST(config_set_yes) {
    TEST_ASSERT(config_get_yes() == false, "Wrong default value");
    TEST_ASSERT(config_set_yes(NULL), "Unexpected return");
    TEST_ASSERT(config_get_yes() == true, "Wrong default value");
    return TEST_SUCCESS;
}

TEST(config_set_load_config) {
    TEST_ASSERT(config_set_load_config("/etc/"), "Unexpected return");
    TEST_ASSERT(strcmp(config_get_load_config(), "/etc/") == 0, "Values are not the same");
    mpm_config_free();
    return TEST_SUCCESS;
}

TEST(config_set_output) {
    TEST_ASSERT(config_set_output("some_file"), "Unexpected return");
    TEST_ASSERT(strcmp(config_get_output(), "some_file") == 0, "Values are not the same");
    mpm_config_free();
    return TEST_SUCCESS;
}

TEST(config_set_list) {
    TEST_ASSERT(config_get_list() == false, "Wrong default value");
    TEST_ASSERT(config_set_list(NULL), "Unexpected return");
    TEST_ASSERT(config_get_list() == true, "Wrong value");
    return TEST_SUCCESS;
}

TEST(config_set_dry_run) {
    TEST_ASSERT(config_get_dry_run() == false, "Wrong default value");
    TEST_ASSERT(config_set_dry_run(NULL), "Unexpected return");
    TEST_ASSERT(config_get_dry_run() == true, "Wrong value");
    return TEST_SUCCESS;
}

TEST(config_set_thread_install) {
    TEST_ASSERT(config_get_thread_install() == false, "Wrong default value");
    TEST_ASSERT(config_set_thread_install(NULL), "Unexpected return");
    TEST_ASSERT(config_get_thread_install() == true, "Wrong value");
    return TEST_SUCCESS;

}

TEST(config_set_thread_dl) {
    TEST_ASSERT(config_get_thread_dl() == false, "Wrong default value");
    TEST_ASSERT(config_set_thread_dl(NULL), "Unexpected return");
    TEST_ASSERT(config_get_thread_dl() == true, "Wrong value");
    return TEST_SUCCESS;

}

void register_options_tests(void) {
    reg_test("options", config_inc_verbose);
    reg_test("options", config_set_directory);
    reg_test("options", config_set_yes);
    reg_test("options", config_set_load_config);
    reg_test("options", config_set_output);
    reg_test("options", config_set_list);
    reg_test("options", config_set_dry_run);
    reg_test("options", config_set_thread_install);
    reg_test("options", config_set_thread_dl);
}
