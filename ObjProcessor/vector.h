//
//  vector.h
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//
#ifndef ObjProcessor_vector_h
#define ObjProcessor_vector_h

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} FullVector;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} ColorVector;

typedef struct
{
    union
    {
        float v[4];
        FullVector vec;
        ColorVector col;
    };
} Vector;

#define VEC_INIT(vec) vec.v[0] = vec.v[1] = vec.v[2] = vec.v[3] = 0.0f;

#define VEC_ASSIGN(vec, x, y, z, w) vec.v[0] = x; vec.v[1] = y; vec.v[2] = z; vec.v[3] = w;

#define VEC_ADD(res, left, right) \
    res.v[0] = left.v[0] + right.v[0];  \
    res.v[1] = left.v[1] + right.v[1];  \
    res.v[2] = left.v[2] + right.v[2];  \
    res.v[3] = left.v[3] + right.v[3];

#endif

