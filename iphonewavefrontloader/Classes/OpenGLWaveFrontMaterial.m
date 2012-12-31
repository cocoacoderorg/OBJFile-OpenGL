//
//  OpenGLWaveFrontMaterial.m
//  Wavefront OBJ Loader
//
//  Created by Jeff LaMarche on 12/18/08.
//  Copyright 2008 Jeff LaMarche. All rights reserved.
//

#import "OpenGLWaveFrontMaterial.h"
#import "OpenGLTexture3D.h"

@implementation OpenGLWaveFrontMaterial
@synthesize   ambient;
@synthesize   diffuse;
@synthesize   specular_color;
@synthesize   specular_coeff;
@synthesize   transparency;
@synthesize   ambient_texture;
@synthesize   diffuse_texture;
@synthesize   specular_color_texture;
@synthesize   specular_coeff_texture;
@synthesize   alpha_texture;
@synthesize   bump_texture;
@synthesize   displace_texture;
@synthesize   decal_texture;


- (void)dealloc
{
    [ambient_texture release];
    [diffuse_texture release];
    [specular_color_texture release];
    [specular_coeff_texture release];
    [alpha_texture release];
    [bump_texture release];
    [displace_texture release];
    [decal_texture release];
     
	[super dealloc];
}
@end
