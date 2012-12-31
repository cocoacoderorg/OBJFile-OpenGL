//
//  postprocess.c
//  ObjProcessor
//
//  Created by Ian Bullard on 12/30/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#include "postprocess.h"
#include <stdio.h>

typedef struct
{
    char* mem;
    unsigned int top;
    unsigned int size;
} MemAllocBlock;

void init_memblock(MemAllocBlock* block, void* ptr, unsigned int size)
{
    block->mem = ptr;
    block->top = 0;
    block->size = size;
}

void* malloc_memblock(MemAllocBlock* block, unsigned int size)
{
    void* result = &(block->mem[block->top]);
    
    block->top += size;
    
    if(block->top > block->size)
    {
        printf("ERROR!  Tried to allocate more memory than available.\n");
        exit(255);
    }
    
    return result;
}

int find_material(ObjFileContents* obj, char* name)
{
    for(int i = 0; i < obj->material.cur_size; ++i)
    {
        if(strcmp(obj->material.l[i].name, name) == 0)
        {
            return i;
        }
    }
    return 0;
}

int find_texture(ObjFileContents* obj, char* name)
{
    for(int i = 0; i < obj->texture.cur_size; ++i)
    {
        if(strcmp(obj->texture.l[i].name, name) == 0)
        {
            return i;
        }
    }
    return 0;
}

DEFINE_LIST(ObjFullVert, oglvert_list);

void init_vert(ObjFullVert* v)
{
    v->pos[0] = 0.0f;
    v->pos[1] = 0.0f;
    v->pos[2] = 0.0f;

    v->tex[0] = 0.0f;
    v->tex[1] = 0.0f;

    v->norm[0] = 0.0f;
    v->norm[1] = 0.0f;
    v->norm[2] = 0.0f;
}

void set_vert(ObjFileContents* obj, ObjFullVert* vert, int faceIndex, int vertIndex)
{
    init_vert(vert);
    
    // Guaranteed valid face indices for positions (at least for valid files)
    vert->pos[0] = obj->vertex.l[obj->face.l[faceIndex].vert[vertIndex]].v[0];
    vert->pos[1] = obj->vertex.l[obj->face.l[faceIndex].vert[vertIndex]].v[1];
    vert->pos[2] = obj->vertex.l[obj->face.l[faceIndex].vert[vertIndex]].v[2];
    
    if(obj->face.l[faceIndex].normal[vertIndex] >= 0)
    {
        vert->norm[0] = obj->vertex.l[obj->face.l[faceIndex].normal[vertIndex]].v[0];
        vert->norm[1] = obj->vertex.l[obj->face.l[faceIndex].normal[vertIndex]].v[1];
        vert->norm[2] = obj->vertex.l[obj->face.l[faceIndex].normal[vertIndex]].v[2];
    }
    
    if(obj->face.l[faceIndex].texcoord[vertIndex] >= 0)
    {
        vert->tex[0] = obj->vertex.l[obj->face.l[faceIndex].texcoord[vertIndex]].v[0];
        vert->tex[1] = obj->vertex.l[obj->face.l[faceIndex].texcoord[vertIndex]].v[1];
    }
}

void create_mesh(int i, ObjFileContents* obj, oglvert_list* list)
{
    int start = obj->mesh.l[i].start;
    int end = obj->face.cur_size;
    
    if( i < obj->mesh.cur_size-1 )
    {
        end = obj->mesh.l[i+1].start;
    }
    
    ObjFullVert curVert;
    
    for(int face = start; face < end; ++face)
    {
        // Assumptions: vertex winding is consistant
        //              all polygons are planar
        //              all polygons are convex
        
        // How many tris make up the face?
        int numTris;
        
        for(numTris = 0; numTris < MAX_FACE_VERTS; ++numTris)
        {
            if( obj->face.l[face].vert[numTris] < 0 )
            {
                break;
            }
        }
        
        numTris = numTris - 2; // We're treating the face as if it's a triangle fan
        
        if( numTris < 1 )
            continue;
        
        // emit first triangle
        set_vert(obj, &curVert, face, 0);
        ADD_LIST((*list), curVert);
        
        set_vert(obj, &curVert, face, 1);
        ADD_LIST((*list), curVert);
        
        set_vert(obj, &curVert, face, 2);
        ADD_LIST((*list), curVert);
        
        int first = 0, second = 2, third = 3;
        for(int f = 1; f < numTris; ++f)
        {
            set_vert(obj, &curVert, face, first);
            ADD_LIST((*list), curVert);
            
            set_vert(obj, &curVert, face, second);
            ADD_LIST((*list), curVert);
            
            set_vert(obj, &curVert, face, third);
            ADD_LIST((*list), curVert);
            
            // fan verts go like this:
            //  0 1 2
            //  0 2 3
            //  0 3 4
            //  etc
            second++;
            third++;
        }
    }
}


void postprocess(ObjFileContents* obj, OpenGL_OBJ** final, int* size)
{
    // Create meshes from polygons
    oglvert_list* meshes = malloc(sizeof(oglvert_list) * obj->mesh.cur_size);

    for(int i = 0; i < obj->mesh.cur_size; ++i)
    {
        CREATE_LIST(meshes[i], ObjFullVert, 16);
        create_mesh(i, obj, &(meshes[i]));
    }
    
    // We now have everything we need to determine the final size of the output
    
    // Calculate memory block size, start with base structure
    int finalSize = sizeof(OpenGL_OBJ);
    
    // add the size of the meshes
    for(int m = 0; m < obj->mesh.cur_size; ++m)
    {
        finalSize += sizeof(OpenGL_Mesh);
        finalSize += sizeof(ObjFullVert) * meshes[m].cur_size;
    }
    
    // add textures
    for(int t = 0; t < obj->texture.cur_size; ++t)
    {
        finalSize += sizeof(OpenGL_TextureFile);
        finalSize += obj->texture.l[t].size;
    }
    
    // add materials
    for(int mat = 0; mat < obj->material.cur_size; ++mat)
    {
        finalSize += sizeof(OpenGL_Material);
    }
    
    // allocate final memory block
    void* memblock = malloc(finalSize);
    MemAllocBlock blk;
    init_memblock(&blk, memblock, finalSize);
    
    // Start filling in final structures
    // Begin with OpenGL_OBJ
    OpenGL_OBJ* result = malloc_memblock(&blk, sizeof(OpenGL_OBJ));
    char* startPtr = (char*)result;
    
    // copy over meshes
    result->numMeshes = obj->mesh.cur_size;
    char* tempPtr = malloc_memblock(&blk, sizeof(OpenGL_Mesh) * result->numMeshes);
    OpenGL_Mesh* tmpMeshes = (OpenGL_Mesh*)tempPtr;
    result->meshes = (int)(tempPtr - startPtr);
    for(int i = 0; i < obj->mesh.cur_size; ++i)
    {
        tmpMeshes[i].material = find_material(obj, obj->mesh.l[i].material);
        tmpMeshes[i].numVerts = meshes[i].cur_size;
        tempPtr = malloc_memblock(&blk, sizeof(ObjFullVert) * meshes[i].cur_size);
        tmpMeshes[i].verts = (int)(tempPtr - startPtr);
        
        memcpy(tempPtr, meshes[i].l, sizeof(ObjFullVert) * meshes[i].cur_size);
        
        FREE_LIST(meshes[i]);
    }
    free(meshes);
    
    // copy over materials
    result->numMaterials = obj->material.cur_size;
    tempPtr = malloc_memblock(&blk, sizeof(OpenGL_Material) * result->numMaterials);
    OpenGL_Material* tmpMats = (OpenGL_Material*)tempPtr;
    result->materials = (int)(tempPtr - startPtr);
    for(int i = 0; i < obj->material.cur_size; ++i)
    {
        tmpMats[i].ambient[0] = obj->material.l[i].ambient[0];
        tmpMats[i].ambient[1] = obj->material.l[i].ambient[1];
        tmpMats[i].ambient[2] = obj->material.l[i].ambient[2];
        
        tmpMats[i].diffuse[0] = obj->material.l[i].diffuse[0];
        tmpMats[i].diffuse[1] = obj->material.l[i].diffuse[1];
        tmpMats[i].diffuse[2] = obj->material.l[i].diffuse[2];
        
        tmpMats[i].specular_color[0] = obj->material.l[i].specular_color[0];
        tmpMats[i].specular_color[1] = obj->material.l[i].specular_color[1];
        tmpMats[i].specular_color[2] = obj->material.l[i].specular_color[2];
        
        tmpMats[i].specular_coeff = obj->material.l[i].specular_coeff;
        
        tmpMats[i].transparency = obj->material.l[i].transparency;
        
        tmpMats[i].ambient_texture = find_texture(obj, obj->material.l[i].ambient_texture);
        tmpMats[i].diffuse_texture = find_texture(obj, obj->material.l[i].diffuse_texture);
        tmpMats[i].specular_color_texture = find_texture(obj, obj->material.l[i].specular_color_texture);
        tmpMats[i].specular_coeff_texture = find_texture(obj, obj->material.l[i].specular_coeff_texture);
        tmpMats[i].alpha_texture = find_texture(obj, obj->material.l[i].alpha_texture);
        tmpMats[i].bump_texture = find_texture(obj, obj->material.l[i].bump_texture);
        tmpMats[i].displace_texture = find_texture(obj, obj->material.l[i].displace_texture);
        tmpMats[i].decal_texture = find_texture(obj, obj->material.l[i].decal_texture);
    }
    
    // copy over textures
    result->numTextures = obj->texture.cur_size;
    tempPtr = malloc_memblock(&blk, sizeof(OpenGL_TextureFile) * result->numTextures);
    OpenGL_TextureFile* tmpTextures = (OpenGL_TextureFile*)tempPtr;
    result->textures = (int)(tempPtr - startPtr);
    for(int i = 0; i < obj->texture.cur_size; ++i)
    {
        tmpTextures[i].size = obj->texture.l[i].size;
        strcpy(tmpTextures[i].name, obj->texture.l[i].name);
        tempPtr = malloc_memblock(&blk, tmpTextures[i].size);
        tmpTextures[i].data = (int)(tempPtr - startPtr);
        memcpy(tempPtr, obj->texture.l[i].file, tmpTextures[i].size);
    }
    
    *size = blk.top;
    *final = result;
}
