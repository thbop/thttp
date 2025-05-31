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

#ifndef TSTRING_H
#define TSTRING_H

// My custom string functions

#include "string.h"
#include "stdlib.h"

// Returns a heap-allocated substring given a string, a starting pos,
// and the number of characters to copy over (len).
// Returns NULL if the input is invalid
char *TStringSubStr( char *string, int pos, int len ) {
    // Check for invalid conditions
    if ( string == NULL ) return NULL;
    if ( pos < 0 || len < 0 ) return NULL;
    if ( (int)strlen( string ) < pos + len ) return NULL;

    // Allocate new buffer and copy
    char *buffer = (char*)malloc( len + 1 );
    buffer[len] = '\0';

    strncpy( buffer, string + pos, len );


    return buffer;
}

// Returns a heap-allocated copy of the provided string
char *TStringCopy( char *string ) {
    return TStringSubStr( string, 0, strlen( string ) );
}

// Returns the number of times a particular character appears in a string.
int TStringCountChar( char *string, char character ) {
    int length = strlen( string );
    int count = 0;
    for ( int i = 0; i < length; i++ )
        if ( string[i] == character )
            count++;

    return count;
}

// Returns the index of the first occurrence of the character provided
// Returns -1 if the character is not found
int TStringFindChar( char *string, char character ) {
    for ( int i = 0; i < (int)strlen( string ); i++ )
        if ( string[i] == character )
            return i;
    
    return -1;
}

// Splits a given null-terminated string by a given char
// Returns an array of strings and the number of strings (stringCount)
// NOTE: Everything returned by this function is heap-allocated and must be
// freed by the user.
char **TStringSplit( char *string, char separator, int *stringCount ) {
    *stringCount = TStringCountChar( string, separator ) + 1;
    char **list = (char**)malloc( sizeof(char*) * *stringCount + 1 );

    int length = strlen( string );
    int start = 0; // Start of the current substring
    int j = 0; // Position in the list of strings

    // Ignore if the last character is the separator
    if ( string[length - 1] == separator )
        (*stringCount)--;

    for ( int i = 0; i < length; i++ ) {
        if ( string[i] == separator ) {
            list[j] = TStringSubStr( string, start, i - start );
            start = i + 1;
            j++;
        }
        else if ( i == length - 1 ) {
            list[j] = TStringSubStr( string, start, i - start + 1 );
        }
    }

    return list;
}

// Frees a string array
// Ensure that stringCount reflects the actual size of the array or the program
// will seg fault.
void TStringFreeStringArray( char **stringArray, int stringCount ) {
    if ( stringArray == NULL ) return;

    for ( int i = 0; i < stringCount; i++ ) {
        if ( stringArray[i] != NULL )
            free( stringArray[i] );
    }
}

#endif