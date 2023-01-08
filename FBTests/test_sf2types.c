//
//  test_sf2types.c
//  FBTests
//
//  Created by Ben Torkington on 8/01/23.
//

#include "test_sf2types.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <cmocka.h>

#include "sf2types.h"

void test_FIXED16_16_carry(void **state) {
    FIXED16_16 sut = {
        .part.integer = 0x0000,
        .part.fraction = 0xffff,
    };

    sut.full += 1;

    assert_int_equal(0x10000, sut.full);
}

void test_FIXED8_8_carry(void **state) {
    FIXED8_8 sut = {
        .part.integer = 0x00,
        .part.fraction = 0xff,
    };

    sut.full += 1;

    assert_int_equal(0x100, sut.full);
}

void test_MAKE_VECTFP16(void **state) {
    struct VectFP16 sut = MAKE_VECTFP16(1, -1);
    assert_int_equal(4, sizeof(int));

    assert_int_equal(1, sut.x.part.integer);
    assert_int_equal(-1, sut.y.part.integer);
}

void test_SET_VECTFP16(void **state) {
    struct VectFP16 sut = {
        0x12345678,
        0x87654321,
    };

    assert_int_equal(4, sizeof(sut.x.full));

    SET_VECTFP16(sut, 0x1111, 0xffff);

    assert_int_equal(0x1111, sut.x.part.integer);
    assert_int_equal(0x5678, sut.x.part.fraction);
    assert_int_equal(-1, sut.y.part.integer);
    assert_int_equal(0x4321, sut.y.part.fraction);
}
