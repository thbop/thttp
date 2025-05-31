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

#ifndef CLIENTREQUEST_HPP
#define CLIENTREQUEST_HPP

#include <string>
#include "tstring.h"

// Parses the client's HTTP request
class ClientRequest {
public:
    ClientRequest() {}
    ClientRequest( unsigned char *buffer, int bufferLength ) {
        Parse( buffer, bufferLength );
    }

    // Quick enum for request types
    enum Type {
        INVALID,
        GET,
        POST,
    };

    // Parses the client's HTTP request
    void Parse( unsigned char *buffer, int bufferLength );

    // Returns the resource
    std::string resource() const {
        return _resource;
    }

    // Returns the request type
    int type() const {
        return _type;
    }

private:
    std::string _resource = "/";
    int _type = INVALID;

    // Sets the correct enum given the request type string (e.g. "GET")
    void SetType( char *typeString );
};

// Sets the correct enum given the request type string (e.g. "GET")
void ClientRequest::SetType( char *typeString ) {
    std::string typeStr( typeString );

    if ( typeStr == "GET" )
        _type = GET;
    else if ( typeStr == "POST" )
        _type = POST;
}

// Parses the client's HTTP request
void ClientRequest::Parse( unsigned char *buffer, int bufferLength ) {
    if ( buffer[0] == 'G' || buffer[0] == 'P' ) {
        int lineCount;
        char **lines = TStringSplit( (char*)buffer, '\n', &lineCount );

        int args0Count;
        char **args0 = TStringSplit( lines[0], ' ', &args0Count );

        // Set class values
        SetType( args0[0] );
        _resource = std::string( args0[1] );

        TStringFreeStringArray( args0, args0Count );
        TStringFreeStringArray( lines, lineCount );
    }
}



#endif