//
//  test_redhammer.h
//  FBTests
//
//  Created by Ben Torkington on 8/01/23.
//

#ifndef test_redhammer_h
#define test_redhammer_h

extern char testRom[];

void test_RHReadWord(void **state);
void test_RHReadLong(void **state);
void test_RHReadWord16_negative_offset(void **state);
void test_RHReadWord16_positive_offset(void **state);
void test_RHCODE(void **state);

#endif /* test_redhammer_h */
