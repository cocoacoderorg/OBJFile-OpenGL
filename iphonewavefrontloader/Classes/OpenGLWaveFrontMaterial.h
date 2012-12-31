//
//  OpenGLWaveFrontMaterial.h
//  Wavefront OBJ Loader
//
//  Created by Jeff LaMarche on 12/18/08.
//  Copyright 2008 Jeff LaMarche. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "OpengLWaveFrontCommon.h"
@class OpenGLTexture3D;

@interface OpenGLWaveFrontMaterial : NSObject 
{
    Color3D   ambient;
    Color3D   diffuse;
    Color3D   specular_color;
    GLfloat   specular_coeff;
    GLfloat   transparency;
    OpenGLTexture3D*    ambient_texture;
    OpenGLTexture3D*    diffuse_texture;
    OpenGLTexture3D*    specular_color_texture;
    OpenGLTexture3D*    specular_coeff_texture;
    OpenGLTexture3D*    alpha_texture;
    OpenGLTexture3D*    bump_texture;
    OpenGLTexture3D*    displace_texture;
    OpenGLTexture3D*    decal_texture;
}
@property Color3D   ambient;
@property Color3D   diffuse;
@property Color3D   specular_color;
@property GLfloat   specular_coeff;
@property GLfloat   transparency;
@property (nonatomic, retain) OpenGLTexture3D*    ambient_texture;
@property (nonatomic, retain) OpenGLTexture3D*    diffuse_texture;
@property (nonatomic, retain) OpenGLTexture3D*    specular_color_texture;
@property (nonatomic, retain) OpenGLTexture3D*    specular_coeff_texture;
@property (nonatomic, retain) OpenGLTexture3D*    alpha_texture;
@property (nonatomic, retain) OpenGLTexture3D*    bump_texture;
@property (nonatomic, retain) OpenGLTexture3D*    displace_texture;
@property (nonatomic, retain) OpenGLTexture3D*    decal_texture;

@end
