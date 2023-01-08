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

#include "test_redhammer.h"
#include "test_sf2types.h"

#include "redhammer.h"

int main(int argc, const char * argv[]) {
    g_code_roms = testRom; // for RedHammer tests

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_RHCODE),
        cmocka_unit_test(test_RHReadWord),
        cmocka_unit_test(test_RHReadLong),
        cmocka_unit_test(test_RHReadWord16_positive_offset),
        cmocka_unit_test(test_RHReadWord16_negative_offset),
        cmocka_unit_test(test_FIXED16_16_carry),
        cmocka_unit_test(test_FIXED8_8_carry),
        cmocka_unit_test(test_SET_VECTFP16),
        cmocka_unit_test(test_MAKE_VECTFP16),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
