//
//  test_redhammer.c
//  FBTests
//
//  Created by Ben Torkington on 8/01/23.
//

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "test_redhammer.h"

#include "redhammer.h"

char testRom[] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0xff, 0xf8, 0xff, 0xfa, 0x00, 0x08, 0x00, 0x0a,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
};

void test_RHReadWord(void **state) {
    assert_int_equal(0x304, RHReadWord(2));
}

void test_RHReadLong(void **state) {
    assert_int_equal(0x5060708, RHReadLong(4));
}

void test_RHReadWord16_negative_offset(void **state) {
    assert_ptr_equal(&testRom[0], RHOffsetLookup16(RHCODE(8), 0));
    assert_ptr_equal(&testRom[2], RHOffsetLookup16(RHCODE(8), 1));
}

void test_RHReadWord16_positive_offset(void **state) {
    assert_ptr_equal(&testRom[16], RHOffsetLookup16(RHCODE(8), 2));
    assert_ptr_equal(&testRom[18], RHOffsetLookup16(RHCODE(8), 3));
}

void test_RHCODE(void **state) {
    assert_ptr_equal(&testRom[0], RHCODE(0));
    assert_ptr_equal(&testRom[8], RHCODE(8));
    assert_ptr_equal(&testRom[-8], RHCODE(-8));
}