README.txt for ObjProcessor

Usage:  ObjProcessor [options] -i input.obj -o output.bobj

Options:
 -t or --test
    Runs tests to check code validity (debug use only).

 -p or --path
    Sets the base path.  All files are loaded relative to
        this path.  If none is specified, the current
        directory is used.
        
 -i or --input <file.obj>
    Sets the input file to process, required.
    
 -o or --output <file.bobj>
    Sets the output file, required.

 -q or --force-quads
    Gives an error if the obj file has polygons with more
        than 4 points.  Will not write out polygons that
        fail the test but will still write out the bobj
        file.

 -f or --force-tris
    Gives an error if the obj file has polygons with more
        than 3 points.  Will not write out polygons that
        fail the test but will still write out the bobj
        file.

 -r or --report
    Reports information during processing.


Notes:

-- Material and Texture files

ObjProcessor isn't very robust when dealing with subdirectories. If
your obj or mtl has references to files in subdirectories and you're
getting errors, try these solutions:

1) Make sure path seperators in the obj or mtl file are the same as
    your platform ('/' for Unix/Mac or '\' for Windows).
2) Move the file into the same directory as the obj file and
    change the filename in the obj/mtl files to match.
   
-- Polygons

Although ObjProcessor supports polygons of any vertex count, it makes
some assumptions about the polygons:

1) All points of the polygons are coplanar (they are all on the same plane)

2) All polygons are convex.  

If you have an obj file that has polygons that don't satisfy these two
conditions, you'll need to use a 3D editor to fix them.

-- Texture Formats

I haven't gotten [GLKTextureLoader textureWithContentsOfData:texture] to work
with pvr4 files and there's no documentation online that explains why.  So
I don't recommend using that file format.  TGA files have been tested and
are verified to work.  Other file formats (PNG, BMP, etc) should also work
but aren't verified).

-- Backface Culling

There isn't a defined vertex order for OBJ files, in fact files can have mixed
winding order polygons.  The tool currently doesn't fix this so to properly 
draw objects output from this tool you need to turn backface culling off. The
example viewer does this in [BOBJObject drawSelf].