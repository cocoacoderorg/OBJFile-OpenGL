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

void check_opengl_error(const char* function, const int line)
{
	GLenum err = glGetError();
	
	switch (err)
	{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
            NSLog(@"GL_INVALID_ENUM - %s, %d", function, line);
			break;
		case GL_INVALID_VALUE:
            NSLog(@"GL_INVALID_VALUE - %s, %d", function, line);
			break;
		case GL_INVALID_OPERATION:
            NSLog(@"GL_INVALID_OPERATION - %s, %d", function, line);
			break;
		case GL_STACK_OVERFLOW:
            NSLog(@"GL_STACK_OVERFLOW - %s, %d", function, line);
			break;
		case GL_STACK_UNDERFLOW:
            NSLog(@"GL_STACK_UNDERFLOW - %s, %d", function, line);
			break;
		case GL_OUT_OF_MEMORY:
            NSLog(@"GL_OUT_OF_MEMORY - %s, %d", function, line);
			break;
		default:
            NSLog(@"Unknown Error - %s, %d", function, line);
			break;
	}
}

#define CHECK_OGL() check_opengl_error(__FUNCTION__, __LINE__)

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
        
        materials = [[NSMutableArray alloc] init];
        textures = [[NSMutableArray alloc] init];
        
        OpenGL_OBJ* obj = (OpenGL_OBJ*)[data bytes];
        
        // create vertex buffers
        numMesh = obj->numMeshes;
        vertArray = malloc(sizeof(GLuint) * numMesh);
        vertCount = malloc(sizeof(int) * numMesh);
        vertBuffer = malloc(sizeof(GLuint) * numMesh);
        materialIndex = malloc(sizeof(int) * numMesh);
        char* start = (char*)obj;
        
        glGenVertexArraysOES(numMesh, vertArray);
        CHECK_OGL();
        glGenBuffers(numMesh, vertBuffer);
        CHECK_OGL();
        
        OpenGL_Mesh* mesh = (OpenGL_Mesh*)(start + obj->meshes);
        
        // register verticies
        for(int v = 0; v < numMesh; ++v)
        {
            ObjFullVert* verts = (ObjFullVert*)(start + mesh[v].verts);
            
            glBindVertexArrayOES(vertArray[v]);
            CHECK_OGL();
            glBindBuffer(GL_ARRAY_BUFFER, vertBuffer[v]);
            CHECK_OGL();
            vertCount[v] = mesh[v].numVerts;
            materialIndex[v] = mesh[v].material;
            
            glBufferData(GL_ARRAY_BUFFER, mesh[v].numVerts * sizeof(ObjFullVert), (void*)verts, GL_STATIC_DRAW);
            CHECK_OGL();
            
            glEnableVertexAttribArray(GLKVertexAttribPosition);
            CHECK_OGL();
            glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(ObjFullVert), 0);
            CHECK_OGL();
            
            glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
            CHECK_OGL();
            glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(ObjFullVert), (void*)(3*sizeof(float)));
            CHECK_OGL();
            
            glEnableVertexAttribArray(GLKVertexAttribNormal);
            CHECK_OGL();
            glVertexAttribPointer(GLKVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(ObjFullVert), (void*)(5*sizeof(float)));
            CHECK_OGL();
            
            glBindVertexArrayOES(0);
            CHECK_OGL();
        }
        
        // Create textures
        OpenGL_TextureFile* tex = (OpenGL_TextureFile*)(start + obj->textures);
        for(int t = 0; t < obj->numTextures; ++t)
        {
            void* texPtr = (void*)(start + tex[t].data);
            NSData* texData = [NSData dataWithBytesNoCopy:texPtr length:tex[t].size freeWhenDone:NO];
            NSError *error;
            // This check is here to make sure the OGL errors are cleared because if an OGL error has occured
            //  before [GLKTextureLoader textureWithContentsOfData], it will fail even if it didn't cause the
            //  error.
            CHECK_OGL();
            NSDictionary* opt = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithBool:YES], GLKTextureLoaderOriginBottomLeft,
                                 [NSNumber numberWithBool:NO], GLKTextureLoaderGrayscaleAsAlpha,
                                 [NSNumber numberWithBool:YES], GLKTextureLoaderApplyPremultiplication,
                                 [NSNumber numberWithBool:NO], GLKTextureLoaderGenerateMipmaps, nil];
            
            GLKTextureInfo* newTex = [GLKTextureLoader textureWithContentsOfData:texData options:opt error:&error];
            
            if(error)
            {
                NSLog(@"Error loading texture from image: %@", error);
            }
            
            [textures addObject:newTex];
        }
        
        // store materials
        OpenGL_Material* mat = (OpenGL_Material*)(start + obj->materials);
        for(int m = 0; m < obj->numMaterials; ++m)
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
            
                effect.texture2d0.envMode = GLKTextureEnvModeModulate;
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
	// Enable and load the vertex array
    for(int v = 0; v < numMesh; ++v)
    {
        GLKBaseEffect* mat = [materials objectAtIndex:materialIndex[v]];
        
        mat.transform.projectionMatrix = projection;
        
        GLKMatrix4 modelView = GLKMatrix4MakeTranslation(currentPosition.x, currentPosition.y, currentPosition.z);
        modelView = GLKMatrix4Rotate(modelView, currentRotation.x, 1.0f, 0.0f, 0.0f);
        modelView = GLKMatrix4Rotate(modelView, currentRotation.y, 0.0f, 1.0f, 0.0f);
        mat.transform.modelviewMatrix = GLKMatrix4Rotate(modelView, currentRotation.z, 0.0f, 0.0f, 1.0f);
        
        mat.lightingType = GLKLightingTypePerVertex;
        mat.lightModelAmbientColor = GLKVector4Make(0.0f, 0.0f, 0.0f, 0.0f);
        mat.colorMaterialEnabled = GL_TRUE;
        
        glDisable(GL_CULL_FACE);
//        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        mat.light0.enabled = GL_TRUE;
        mat.light0.diffuseColor = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
        
        [mat prepareToDraw];
        
		// Set color and materials based on group's material
        glBindVertexArrayOES(vertArray[v]);
        CHECK_OGL();
        glDrawArrays(GL_TRIANGLES, 0, vertCount[v]);
        CHECK_OGL();
    }
    
	// Restore the current transformation by popping it off
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
