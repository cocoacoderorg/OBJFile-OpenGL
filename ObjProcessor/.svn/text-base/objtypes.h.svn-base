//
//  objtypes.h
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#ifndef ObjProcessor_objtypes_h
#define ObjProcessor_objtypes_h

#define MAX_FACE_VERTS  16

typedef struct
{
    int vert[MAX_FACE_VERTS];
    int texcoord[MAX_FACE_VERTS];
    int normal[MAX_FACE_VERTS];
} FaceType;

#define MAX_NAME 4096

typedef struct
{
    char name[MAX_NAME];
} NameType;

typedef struct
{
    char object[MAX_NAME];
    char group[MAX_NAME];
    char material[MAX_NAME];
    int start;
} MeshType;

typedef struct
{
    char name[MAX_NAME];
    float   ambient[3];
    float   diffuse[3];
    float   specular_color[3];
    float   specular_coeff;
    float   transparency;
    char    ambient_texture[MAX_NAME];
    char    diffuse_texture[MAX_NAME];
    char    specular_color_texture[MAX_NAME];
    char    specular_coeff_texture[MAX_NAME];
    char    alpha_texture[MAX_NAME];
    char    bump_texture[MAX_NAME];
    char    displace_texture[MAX_NAME];
    char    decal_texture[MAX_NAME];
} MaterialType;

typedef struct
{
    char    name[MAX_NAME];
    void*   file;
    int     size;
} TextureType;

#endif
