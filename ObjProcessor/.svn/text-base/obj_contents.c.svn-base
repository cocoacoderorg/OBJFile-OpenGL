//
//  obj_contents.c
//  ObjProcessor
//
//  Created by Ian Bullard on 12/29/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#include "obj_contents.h"
#include "stringutil.h"

#include <stdio.h>

#define UNSUPPORTED(feature, line)  \
    static bool feature = false;   \
    if(!feature)    \
    {   \
        printf("WARNING line %d: Feature %s is unsupported, this warning is only displayed once.\n", line + 1, #feature);  \
        feature = true; \
    }

void InitMaterial(MaterialType* mat)
{
    memset(mat->name, 0, MAX_NAME);
    mat->ambient_texture[0] = 0.2f;
    mat->ambient_texture[1] = 0.2f;
    mat->ambient_texture[2] = 0.2f;
    mat->diffuse[0] = 0.8f;
    mat->diffuse[1] = 0.8f;
    mat->diffuse[2] = 0.8f;
    mat->specular_color[0] = 1.0f;
    mat->specular_color[1] = 1.0f;
    mat->specular_color[2] = 1.0f;
    mat->specular_coeff = 0.0f;
    mat->transparency = 1.0f;
    
    memset(mat->ambient_texture, 0, MAX_NAME);
    memset(mat->diffuse_texture, 0, MAX_NAME);
    memset(mat->specular_color_texture, 0, MAX_NAME);
    memset(mat->specular_coeff_texture, 0, MAX_NAME);
    memset(mat->alpha_texture, 0, MAX_NAME);
    memset(mat->bump_texture, 0, MAX_NAME);
    memset(mat->displace_texture, 0, MAX_NAME);
    memset(mat->decal_texture, 0, MAX_NAME);
}

bool IsNewTexture(ObjFileContents* obj, char* name)
{
    for(int i = 0; i < obj->texture.cur_size; ++i)
    {
        if( strcmp(name, obj->texture.l[i].name) == 0 )
        {
            return false;
        }
    }
    
    return true;
}

void AddIfNewTexture(ObjFileContents* obj, char* name)
{
    if(IsNewTexture(obj, name))
    {
        TextureType t;
        memset(t.name, 0, MAX_NAME);
        strcpy(t.name, name);
        
        FILE* fp = fopen(name, "rb");
        
        if(fp == NULL)
        {
            printf("Failed to open texture file: %s\n", name);
            return;
        }
        
        fseek(fp, 0, SEEK_END);
        t.size = (int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        t.file = malloc(t.size);
        int readAmount = (int)fread(t.file, 1, t.size, fp);
        
        if(readAmount != t.size)
        {
            printf("Failed to read texture file: %s\n", name);
            free(t.file);
            return;
        }
        
        ADD_LIST(obj->texture, t);
    }
}

void InitObjFileContents(ObjFileContents* file, int maxVertCount)
{
    CREATE_LIST(file->vertex, Vector, 500);
    CREATE_LIST(file->texture_coord, Vector, 500);
    CREATE_LIST(file->normal, Vector, 500);

    CREATE_LIST(file->face, FaceType, 500);

    CREATE_LIST(file->material_lib, NameType, 5);
    CREATE_LIST(file->mesh, MeshType, 5);
    
    CREATE_LIST(file->material, MaterialType, 5);
    file->first_material = true;
    
    CREATE_LIST(file->texture, TextureType, 5);
    
    // state machine to track groups
    file->last_line = UNKNOWN;

    memset(file->cur_mesh.group, 0, MAX_NAME);
    memset(file->cur_mesh.object, 0, MAX_NAME);
    memset(file->cur_mesh.group, 0, MAX_NAME);
    memset(file->cur_mesh.material, 0, MAX_NAME);
    file->cur_mesh.start = 0;
    
    InitMaterial(&file->cur_material);
    
    // options
    file->max_verts = maxVertCount;
}

void FinishObjFile(ObjFileContents* file)
{
    // Do we need to save the current material?
    if(!file->first_material)
    {
        // we do
        ADD_LIST(file->material, file->cur_material);
        InitMaterial(&file->cur_material);
    }
}

void EmitUnknown(ObjFileContents* file, int line_num, char* line)
{
    UNSUPPORTED(UNKNOWN, line_num);
}

void EmitEmpty(ObjFileContents* file, int line_num, char* line)
{
    // Empty line, nothing to do.
    //  Only here in case of debugging opportunities
}

void EmitComment(ObjFileContents* file, int line_num, char* line)
{
    // Nothing to do.
    //  Only here in case of debugging opportunities
}

void EmitVertex(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("v", s) != 0)
    {
        printf("ERROR line %d: Invalid vertex found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);
    
    ADD_LIST(file->vertex, v);
    file->last_line = VERTEX;
}

void EmitTexCoord(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("vt", s) != 0)
    {
        printf("ERROR line %d: Invalid vertex texture coord found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);
    
    ADD_LIST(file->texture_coord, v);
    file->last_line = TEXTURE_COORD;
}

void EmitNormal(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("vn", s) != 0)
    {
        printf("ERROR line %d: Invalid vertex texture coord found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);
    
    ADD_LIST(file->normal, v);
    file->last_line = NORMAL;
}

void EmitVertParam(ObjFileContents* file, int line_num, char* line)
{
    // not supported
    UNSUPPORTED(vertex_param, line_num);
}

void EmitFace(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("f", s) != 0)
    {
        printf("ERROR line %d: Invalid face found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    FaceType f = parse_face(num);
    
    // check for new mesh
    if(file->last_line != FACE)
    {
        file->cur_mesh.start = file->face.cur_size;
        
        ADD_LIST(file->mesh, file->cur_mesh);
    }
    
    int vert_count = 0;
    for(int i = 0; i < MAX_FACE_VERTS; ++i)
    {
        if( f.vert < 0 )
            break;
        vert_count++;
    }
    
    if( vert_count <= file->max_verts )
    {
        ADD_LIST(file->face, f);
    }
    else
    {
        printf("ERROR line %d: Polygon with more than allowed verts found (due to -f or -q)\n", line_num + 1);
    }
    
    file->last_line = FACE;
}

void EmitMaterialLib(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("mtllib", s) != 0)
    {
        printf("ERROR line %d: Invalid material lib reference found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    NameType mtl;
    strcpy(mtl.name, name);
    
    ADD_LIST(file->material_lib, mtl);
}

void EmitUseMaterial(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("usemtl", s) != 0)
    {
        printf("ERROR line %d: Invalid material found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_mesh.material, name);
    
    // need to do this because a change in material
    //  forces a change in a mesh
    file->last_line = USE_MATERIAL;
}

void EmitObject(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("o", s) != 0)
    {
        printf("ERROR line %d: Invalid object found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_mesh.object, name);
}

void EmitGroup(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("g", s) != 0)
    {
        printf("ERROR line %d: Invalid group found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_mesh.group, name);
}

void EmitSmoothingGroup(ObjFileContents* file, int line_num, char* line)
{
    // not supported, but no warning because we're not an editor
    //  so we don't care
    // UNSUPPORTED(smoothing_group, line_num);
}


// Material
void EmitMatNew(ObjFileContents* file, int line_num, char* line)
{
    // Do we need to save the current material?
    if(!file->first_material)
    {
        // we do
        ADD_LIST(file->material, file->cur_material);
        InitMaterial(&file->cur_material);
    }
    file->first_material = false;

    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("newmtl", s) != 0)
    {
        printf("ERROR line %d: Invalid new material found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.name, name);
}

void EmitMatAmbient(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("Ka", s) != 0)
    {
        printf("ERROR line %d: Invalid ambient found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);

    file->cur_material.ambient[0] = v.v[0];
    file->cur_material.ambient[1] = v.v[1];
    file->cur_material.ambient[2] = v.v[2];
}

void EmitMatDiffuse(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("Kd", s) != 0)
    {
        printf("ERROR line %d: Invalid diffuse found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);
    
    file->cur_material.diffuse[0] = v.v[0];
    file->cur_material.diffuse[1] = v.v[1];
    file->cur_material.diffuse[2] = v.v[2];
}

void EmitMatSpecularColor(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("Ks", s) != 0)
    {
        printf("ERROR line %d: Invalid specular found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);
    
    file->cur_material.specular_color[0] = v.v[0];
    file->cur_material.specular_color[1] = v.v[1];
    file->cur_material.specular_color[2] = v.v[2];
}

void EmitMatSpecularCoeff(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("Ns", s) != 0)
    {
        printf("ERROR line %d: Invalid specular coeff found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);
    
    file->cur_material.specular_coeff = v.v[0];
}

void EmitMatTransparent(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("d", s) != 0 &&
       strcmp("Tr", s) != 0 )
    {
        printf("ERROR line %d: Invalid transparency found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* num = line;
    
    while(*(num++) != '\0');
    
    Vector v = parse_vector(num);
    
    file->cur_material.transparency = v.v[0];
}

void EmitMatIllumination(ObjFileContents* file, int line_num, char* line)
{
    // not supported
    UNSUPPORTED(material_illumination, line_num);
}

void EmitMatRefraction(ObjFileContents* file, int line_num, char* line)
{
    // not supported
    UNSUPPORTED(material_refraction, line_num);
}

void EmitMatTransmissionFilter(ObjFileContents* file, int line_num, char* line)
{
    // not supported
    UNSUPPORTED(material_transmission_filter, line_num);
}

void EmitMatTexAmbient(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("map_Ka", s) != 0)
    {
        printf("ERROR line %d: Invalid texture ambient found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.ambient_texture, name);
    
    
    AddIfNewTexture(file, name);
}

void EmitMatTexDiffuse(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("map_Kd", s) != 0)
    {
        printf("ERROR line %d: Invalid texture diffuse found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.diffuse_texture, name);
    
    AddIfNewTexture(file, name);
}

void EmitMatTexSpecularColor(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("map_Ks", s) != 0)
    {
        printf("ERROR line %d: Invalid texture specular color found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.specular_color_texture, name);
    
    AddIfNewTexture(file, name);
}

void EmitMatTexSpecularCoeff(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("map_Ns", s) != 0)
    {
        printf("ERROR line %d: Invalid texture specular coeff found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.specular_coeff_texture, name);
    
    AddIfNewTexture(file, name);
}

void EmitMatTexTransparent(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("map_d", s) != 0)
    {
        printf("ERROR line %d: Invalid texture alpha found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.alpha_texture, name);
    
    AddIfNewTexture(file, name);
}

void EmitMatTexIllumination(ObjFileContents* file, int line_num, char* line)
{
    // not supported
    UNSUPPORTED(material_texture_illumination, line_num);
}

void EmitMatTexBump(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("map_bump", s) != 0)
    {
        printf("ERROR line %d: Invalid bump map found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.bump_texture, name);
    
    AddIfNewTexture(file, name);
}

void EmitMatTexDisplace(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("disp", s) != 0)
    {
        printf("ERROR line %d: Invalid displacement map found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.displace_texture, name);
    
    AddIfNewTexture(file, name);
}

void EmitMatTexDecal(ObjFileContents* file, int line_num, char* line)
{
    // verify line
    char* s = strtok(line, " ");
    
    if(strcmp("decal", s) != 0)
    {
        printf("ERROR line %d: Invalid decal found\n", line_num + 1);
        return;
    }
    
    // since strtok changed the string, find the part after the null
    char* name = line;
    
    while(*(name++) != '\0');
    
    strcpy(file->cur_material.decal_texture, name);
    
    AddIfNewTexture(file, name);
}

void EmitMatTexReflection(ObjFileContents* file, int line_num, char* line)
{
    // not supported
    UNSUPPORTED(material_texture_reflection, line_num);
}

