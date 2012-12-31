//
//  obj_opengl.h
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#ifndef ObjProcessor_obj_opengl_h
#define ObjProcessor_obj_opengl_h

// Currently all OBJ files are converted into one type of mesh,
//  regardless of the input data.  All data is loaded into
//  the tool and could output optimal structures but for now,
//  this shortcut is being made.

// Define an offset pointer.  We do this because pointers can
//  be 32 or 64 bits and we need to be consistant
typedef int off_ptr;

typedef struct
{
    float   pos[3];
    float   tex[2];
    float   norm[3];
} ObjFullVert;

typedef struct
{
    int material;
    int numVerts;
    off_ptr verts;     // ObjFullVert
} OpenGL_Mesh;

#define MAX_TEXTURE_FILENAME 4096

typedef struct
{
    char name[MAX_TEXTURE_FILENAME];
    off_ptr data;
    int size;
} OpenGL_TextureFile;

// Limited support for materials.  Specifically:
//  No texture options, no reflection map
typedef struct
{
    float   ambient[3];
    float   diffuse[3];
    float   specular_color[3];
    float   specular_coeff;
    float   transparency;
    int     ambient_texture;
    int     diffuse_texture;
    int     specular_color_texture;
    int     specular_coeff_texture;
    int     alpha_texture;
    int     bump_texture;
    int     displace_texture;
    int     decal_texture;
} OpenGL_Material;

typedef struct
{
    int numMeshes;
    int numMaterials;
    int numTextures;
    
    off_ptr meshes;     // OpenGL_Mesh
    off_ptr materials;  // OpenGL_Material
    off_ptr textures;   // OpenGL_TextureFile
} OpenGL_OBJ;

#endif
