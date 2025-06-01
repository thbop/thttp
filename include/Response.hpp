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

#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <sstream>
#include "tstring.h"

// Basic class to generate HTTP responses
class Response {
public:
    // Enum and strings for constructing the response status
    int status = NO_CONTENT;
    enum Status {
        OK,
        NO_CONTENT,
        NOT_FOUND,
        TEAPOT,       // The most important one
        STATUS_COUNT,
    };

    // Constructor
    // `content` can be NULL if we are sending a 404
    // Otherwise, ensure that `content` is a null-terminated string
    Response( const char *content ) {
        if ( content == NULL )
            status = NOT_FOUND;
        else {
            status = OK;
            this->content = TStringCopy( (char*)content );
            contentLength = strlen( content ) + 1;
        }
    }

    // Cleanup
    ~Response() {
        free( content );
    }

    // Exports the response packet
    std::string Export();

private:
    char *content = NULL;
    int contentLength = 0;
    std::string outData;

    constexpr static char statuses[STATUS_COUNT][32] = {
        "200 OK",
        "204 No Content",
        "404 Not Found",
        "418 I'm a teapot",
    };
    
};

std::string Response::Export() {
    std::stringstream out;

    out << "HTTP/1.1 " << statuses[status] << "\r\n";
    out << "Content-Type: text/html\r\n";
    out << "Content-Length: " << contentLength << "\r\n";
    out << "Connection: close\r\n";
    out << "\r\n";
    if ( content != NULL )
        out << content;

    return out.str();
}

#endif