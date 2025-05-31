/*
* Copyright © 2025 Thbop
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the “Software”), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef TFILE_H
#define TFILE_H

// A simple file opening utility

#include "stdio.h"
#include "stdlib.h"

// Opens a file and returns a heap-allocated, null-terminated buffer with its
// contents.
char *TFileOpen( const char *fileName ) {
    FILE *fp = fopen( fileName, "r" );
    if ( fp == NULL )
        return NULL;

    // Get file size
    fseek( fp, 0, SEEK_END );
    long size = ftell( fp );
    fseek( fp, 0, SEEK_SET );


    // Allocate buffer
    char *buffer = (char*)calloc( size + 1, sizeof(char) ); // +1 for the null terminator
    
    // Read into buffer
    fread( buffer, sizeof(char), size, fp );

    fclose( fp );
    
    return buffer;
}

#endif