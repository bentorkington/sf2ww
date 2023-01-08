//
//  main.c
//  FBTests
//
//  Created by Ben Torkington on 8/01/23.
//

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <cmocka.h>

static void null_test_success(void **state) {
    (void) state; /* unused */
}


int main(int argc, const char * argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
