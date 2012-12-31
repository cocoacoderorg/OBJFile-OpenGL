//
//  BOBJObject.m
//  OpenGL BOBJ Viewer
//
//  Created by Ian Bullard on 12/31/12.
//  Copyright (c) 2012 Ian Bullard. All rights reserved.
//

#import "BOBJObject.h"
#import "obj_opengl.h"
#import <GLKit/GLKit.h>

@implementation BOBJObject

@synthesize currentPosition;
@synthesize currentRotation;

- (id)initWithPath:(NSString *)path
{
	
	if ((self = [super init]))
	{
        NSData *data = [NSData dataWithContentsOfFile:path];
        
        if (data == NULL)
        {
            NSLog(@"Failed to load file: %s", [path cStringUsingEncoding:NSUTF8StringEncoding]);
            return nil;
        }
        
        OpenGL_OBJ* obj = (OpenGL_OBJ*)[data bytes];
        
        // create vertex buffers
        numMesh = obj->numMeshes;
        vertArray = malloc(sizeof(GLuint) * numMesh);
        vertCount = malloc(sizeof(int) * numMesh);
        vertBuffer = malloc(sizeof(GLuint) * numMesh);
        materialIndex = malloc(sizeof(int) * numMesh);
        char* start = (char*)obj;
        
        glGenVertexArraysOES(numMesh, vertArray);
        glGenBuffers(numMesh, vertBuffer);
        
        OpenGL_Mesh* mesh = (OpenGL_Mesh*)(start + obj->meshes);
        
        // register verticies
        for(int v = 0; v < numMesh; ++v)
        {
            ObjFullVert* verts = (ObjFullVert*)(start + mesh[v].verts);
            
            glBindVertexArrayOES(vertArray[v]);
            glBindBuffer(GL_ARRAY_BUFFER, vertBuffer[v]);
            vertCount[v] = mesh[v].numVerts;
            materialIndex[v] = mesh[v].material;
            
            glBufferData(GL_ARRAY_BUFFER, mesh[v].numVerts * sizeof(ObjFullVert), (void*)verts, GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(GLKVertexAttribPosition);
            glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(ObjFullVert), 0);
            
            glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
            glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(ObjFullVert), (void*)(3*sizeof(float)));
            
            glEnableVertexAttribArray(GLKVertexAttribNormal);
            glVertexAttribPointer(GLKVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(ObjFullVert), (void*)(5*sizeof(float)));
            
            glBindVertexArrayOES(0);
        }
        
        // Create textures
        OpenGL_TextureFile* tex = (OpenGL_TextureFile*)(start + obj->textures);
        for(int t = 0; t < obj->numTextures; ++t)
        {
            void* texPtr = (void*)(start + tex[t].data);
            NSData* texData = [NSData dataWithBytesNoCopy:texPtr length:tex[t].size];
            NSError *error;
            GLKTextureInfo* newTex = [GLKTextureLoader textureWithContentsOfData:texData options:nil error:&error];
            
            if(error)
            {
                NSLog(@"Error loading texture from image: %@", error);
            }
            
            [textures addObject:newTex];
        }
        
        // store materials
        OpenGL_Material* mat = (OpenGL_Material*)(start + obj->materials);
        for(int m = 0; m < obj->numMeshes; ++m)
        {
            GLKBaseEffect* effect = [[GLKBaseEffect alloc] init];
            
            effect.material.ambientColor = GLKVector4Make(mat[m].ambient[0], mat[m].ambient[1], mat[m].ambient[2], 1.0f);
            effect.material.diffuseColor = GLKVector4Make(mat[m].diffuse[0], mat[m].diffuse[1], mat[m].diffuse[2], mat[m].transparency);
            effect.material.specularColor = GLKVector4Make(mat[m].specular_color[0], mat[m].specular_color[1], mat[m].specular_color[2], 1.0f);
            effect.material.shininess =  mat[m].specular_coeff;
            
            if( mat[m].diffuse_texture >= 0)
            {
                GLKTextureInfo* tex;
                
                tex = [textures objectAtIndex:mat[m].diffuse_texture];
            
                effect.texture2d0.envMode = GLKTextureEnvModeReplace;
                effect.texture2d0.target = GLKTextureTarget2D;
                effect.texture2d0.name = tex.name;
                effect.texture2d0.enabled = TRUE;
            }
            
            [materials addObject:effect];
        }
        
	}
	return self;
}

- (void)drawSelf:(GLKMatrix4)projection
{
	// Save the current transformation by pushing it on the stack
	glPushMatrix();
	
	// Load the identity matrix to restore to origin
	glLoadIdentity();
	
	// Translate to the current position
	glTranslatef(currentPosition.x, currentPosition.y, currentPosition.z);
	
	// Rotate to the current rotation
	glRotatef(currentRotation.x, 1.0, 0.0, 0.0);
	glRotatef(currentRotation.y, 0.0, 1.0, 0.0);
	glRotatef(currentPosition.z, 0.0, 0.0, 1.0);
	
	// Enable and load the vertex array
    for(int v = 0; v < numMesh; ++v)
    {
        GLKBaseEffect* mat = [materials objectAtIndex:v];
        
        mat.transform.projectionMatrix = projection;
        
        [mat prepareToDraw];
        
		// Set color and materials based on group's material
        glBindVertexArrayOES(vertArray[v]);
        glDrawArrays(GL_TRIANGLES, 0, vertCount[v]/3);
    }
    
	// Restore the current transformation by popping it off
	glPopMatrix();
}

- (void)dealloc
{
	if (vertArray)
		free(vertArray);
	if (vertCount)
		free(vertCount);
	if (vertBuffer)
		free(vertBuffer);
	if (materialIndex)
		free(materialIndex);
}

@end
