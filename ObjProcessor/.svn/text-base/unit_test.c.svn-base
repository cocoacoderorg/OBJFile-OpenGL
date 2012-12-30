//
//  unit_test.c
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#include "unit_test.h"

#include "vector.h"
#include "expanding_list.h"

#include <stdio.h>

#define CHECK(assertion, message)   \
    if(!(assertion))    \
    {   \
        test_result = false;    \
        printf("%s : %d - ", __FILE__, __LINE__);  \
        printf(message);    \
    }

bool test_vector();
bool test_list();

bool unit_test()
{
    bool test_result = true; // true == passed

    CHECK(test_vector(), "Unit test test_vector failed\n");
    CHECK(test_list(), "Unit test test_list failed\n");
    
    return test_result;
}

bool test_vector()
{
    Vector init;
    bool test_result = true; // true == passed
    
    init.v[0] = 0.0f;
    
    VEC_INIT(init);
    
    CHECK(init.v[0] == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.v[1] == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.v[2] == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.v[3] == 0.0f, "test_vector: INIT failed\n");

    CHECK(init.vec.x == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.vec.y == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.vec.z == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.vec.w == 0.0f, "test_vector: INIT failed\n");
    
    CHECK(init.col.r == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.col.g == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.col.b == 0.0f, "test_vector: INIT failed\n");
    CHECK(init.col.a == 0.0f, "test_vector: INIT failed\n");
    
    init.v[0] = 1.0f;
    init.v[1] = 2.0f;
    init.v[2] = 3.0f;
    init.v[3] = 4.0f;
    
    CHECK(init.vec.x == 1.0f, "test_vector: union test failed\n");
    CHECK(init.vec.y == 2.0f, "test_vector: union test failed\n");
    CHECK(init.vec.z == 3.0f, "test_vector: union test failed\n");
    CHECK(init.vec.w == 4.0f, "test_vector: union test failed\n");
    
    CHECK(init.col.r == 1.0f, "test_vector: union test failed\n");
    CHECK(init.col.g == 2.0f, "test_vector: union test failed\n");
    CHECK(init.col.b == 3.0f, "test_vector: union test failed\n");
    CHECK(init.col.a == 4.0f, "test_vector: union test failed\n");
    
    VEC_ASSIGN(init, 5.0f, 6.0f, 7.0f, 8.0f);
    
    CHECK(init.v[0] == 5.0f, "test_vector: VEC_ASSIGN failed\n");
    CHECK(init.v[1] == 6.0f, "test_vector: VEC_ASSIGN failed\n");
    CHECK(init.v[2] == 7.0f, "test_vector: VEC_ASSIGN failed\n");
    CHECK(init.v[3] == 8.0f, "test_vector: VEC_ASSIGN failed\n");
    
    Vector second;
    VEC_ASSIGN(second, 9.0f, 10.0f, 11.0f, 12.0f);
    
    Vector result;
    VEC_ADD(result, init, second);
    
    CHECK(result.v[0] == 14.0f, "test_vector: VEC_ADD failed\n");
    CHECK(result.v[1] == 16.0f, "test_vector: VEC_ADD failed\n");
    CHECK(result.v[2] == 18.0f, "test_vector: VEC_ADD failed\n");
    CHECK(result.v[3] == 20.0f, "test_vector: VEC_ADD failed\n");
    
    return test_result;
}

bool test_list()
{
    bool test_result = true; // true == passed
    
    DEFINE_LIST(int, int_list);
    DEFINE_LIST(Vector, vec_list);
    
    int_list test_int;
    vec_list test_vec;
    
    CREATE_LIST(test_int, int, 5);
    CREATE_LIST(test_vec, Vector, 5);
    
    ADD_LIST(test_int, 5);
    
    CHECK(test_int.l[0] == 5, "test_int vector ADD_LIST failed");
    CHECK(test_int.cur_size = 1, "test_int vector ADD_LIST failed");

    ADD_LIST(test_int, 6);
    ADD_LIST(test_int, 7);
    ADD_LIST(test_int, 8);
    ADD_LIST(test_int, 9);
    ADD_LIST(test_int, 10);
    ADD_LIST(test_int, 11);

    CHECK(test_int.l[0] == 5, "test_int vector ADD_LIST failed");
    CHECK(test_int.l[1] == 6, "test_int vector ADD_LIST failed");
    CHECK(test_int.l[2] == 7, "test_int vector ADD_LIST failed");
    CHECK(test_int.l[3] == 8, "test_int vector ADD_LIST failed");
    CHECK(test_int.l[4] == 9, "test_int vector ADD_LIST failed");
    CHECK(test_int.l[5] == 10, "test_int vector ADD_LIST failed");
    CHECK(test_int.l[6] == 11, "test_int vector ADD_LIST failed");
    CHECK(test_int.cur_size == 7, "test_int vector ADD_LIST failed");
    CHECK(test_int.max_size == 10, "test_int vector ADD_LIST failed");

    FREE_LIST(test_int);
    
    Vector val;
    val.v[0] = val.v[1] = val.v[2] = val.v[3] = 10.0f;
    ADD_LIST(test_vec, val);
    
    CHECK(test_vec.l[0].v[0] == 10.0f, "test_vec vector ADD_LIST failed");
    CHECK(test_vec.l[0].v[1] == 10.0f, "test_vec vector ADD_LIST failed");
    CHECK(test_vec.l[0].v[2] == 10.0f, "test_vec vector ADD_LIST failed");
    CHECK(test_vec.l[0].v[3] == 10.0f, "test_vec vector ADD_LIST failed");
    
    Vector inc;
    VEC_ASSIGN(inc, 1.0f, 1.0f, 1.0f, 1.0f);
    
    VEC_ADD(val, val, inc);
    ADD_LIST(test_vec, val);
    VEC_ADD(val, val, inc);
    ADD_LIST(test_vec, val);
    VEC_ADD(val, val, inc);
    ADD_LIST(test_vec, val);
    VEC_ADD(val, val, inc);
    ADD_LIST(test_vec, val);
    VEC_ADD(val, val, inc);
    ADD_LIST(test_vec, val);
    VEC_ADD(val, val, inc);
    ADD_LIST(test_vec, val);
    
    CHECK(test_vec.l[6].v[0] == 16.0f, "test_vec vector ADD_LIST failed");
    CHECK(test_vec.l[6].v[1] == 16.0f, "test_vec vector ADD_LIST failed");
    CHECK(test_vec.l[6].v[2] == 16.0f, "test_vec vector ADD_LIST failed");
    CHECK(test_vec.l[6].v[3] == 16.0f, "test_vec vector ADD_LIST failed");
    CHECK(test_int.cur_size == 7, "test_int vector ADD_LIST failed");
    CHECK(test_int.max_size == 10, "test_int vector ADD_LIST failed");
    
    return test_result;
}