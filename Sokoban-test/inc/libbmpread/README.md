libbmpread
==========

libbmpread is a tiny, fast bitmap (.bmp) image file loader, written from
scratch in portable C (see below), with no dependencies.  Its default behavior
is compatible with OpenGL texture functions, making it ideal for use in simple
games.  It handles uncompressed monochrome, 16- and 256-color, and 24-bit
bitmap files of any size (no RLE support yet).

<https://github.com/chazomaticus/libbmpread>

Documentation
-------------

To use, simply copy `bmpread.c` and `bmpread.h` into your project and add them
to the build.

See `bmpread.h` for thorough documentation of the interface.  The main points
are:
* `bmpread_t` - struct that holds bitmap data
* `bmpread()` - read bitmap data from disk into a `bmpread_t`
* `bmpread_free()` - free memory allocated in `bmpread()`

Example
-------

Here's a code snippet showing how libbmpread might be used to create an OpenGL
texture from a bitmap file on disk:

```c
#include <stdio.h>
#include <GL/gl.h>
#include "bmpread.h"

/* Load the specified bitmap file from disk and copy it into an OpenGL texture.
 * Return the GLuint representing the texture.
 */
GLuint LoadTexture(const char * bitmap_file)
{
    GLuint texture = 0;
    bmpread_t bitmap;

    if(!bmpread(bitmap_file, 0, &bitmap))
    {
        fprintf(stderr, "%s: error loading bitmap file\n", bitmap_file);
        exit(1);
    }

    /* At this point, bitmap.width and .height hold the pixel dimensions of the
     * file, and bitmap.rgb_data holds the raw pixel data in RGB triplets.
     */

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.width, bitmap.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data);

    bmpread_free(&bitmap);

    return texture;
}

void SomeInitFunction(void)
{
    GLuint tex1 = LoadTexture("texture1.bmp");
    // ...
}
```

See also the `example` directory for a full, compiling example that displays a
bitmap named on the command line on an OpenGL quad using GLUT.  To build and
run:

    cd example
    make
    ./bmpread-example <bmpfile>

Some example bitmap files are included there as well.

Portability/Security Notes
--------------------------

libbmpread is written to be maximally ANSI C (C89/C90) portable, minimizing
undefined and implementation-defined behavior.  It's also written to be able to
be compiled as C++, so it can be used in C++ projects with a minimum of fuss.
It should work in most environments--I believe there are only two assumptions
the code makes that aren't guaranteed by the C standard: 1) `CHAR_BIT == 8`
(note: if this is violated, compilation will fail), and 2) two's complement
integer storage; deviations from these assumptions are extremely rare in the
wild.  I've also made use of `<stdint.h>` and `ptrdiff_t`, which I believe are
technically C99 features, but are common in practice even for non-compliant
compilers.

I've taken every precaution to prevent common bugs that can have security
impact, such as integer overflows that might lead to buffer overruns.  I
believe it's impossible to cause libbmpread to do anything besides properly
load a file or fail with error, even on maliciously crafted files.  I haven't
done a thorough audit, nor am I an expert at writing hardened "C/C++" code, so
take my belief with a grain of salt.

Tests
-----

To run the test suite:

    cd test
    make

The last line will read `All tests passed!` if everything was ok.  Be patient:
I test an unoptimized function over its entire 32-bit numeric range, so it can
take a minute or two to finish.

There are some basic unit tests, but more work is needed to test the behavior
of the public API.  The tests are compiled as both portable C and portable C++
code to cover compilation in each, and each resulting test binary is run to
check its runtime behavior.


Enjoy!
