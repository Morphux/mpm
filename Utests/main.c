#include "main.h"

int     main(void) {
    int         ret;

    register_options_tests();

    m_info("Beginning tests...\n");
    ret = test_all();
    test_free();

    return ret;
}
