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

// Thbop Socket Layer
// A C++ abstraction for TCP sockets
#include <iostream>
#include "string.h"

#define GPTSOCK_HIDE_ERRORS
#include "gptsock.h"

// Thbop Socket Layer namespace
namespace tsl {
    bool _started = false;

    // A class manage heap-allocated buffers
    class Buffer {
    public:
        // Allocate the buffer
        Buffer( int size = GPTSOCK_BUFFER_SIZE ) : size( size ) {
            buffer = new unsigned char[size];
        }

        // Create a buffer from a cstring
        Buffer( const char *string ) : size( strlen( string ) + 1 ) {
            buffer = new unsigned char[size];
            memcpy( buffer, string, size );
        }

        // Deallocate when we're done
        ~Buffer() {
            delete[] buffer;
        }

        // Direct pointer to the buffer
        unsigned char *data() const {
            return buffer;
        }

        // Returns the size of the buffer
        int length() const {
            return size;
        }
    private:
        unsigned char *buffer;
        int size;
    };

    // A C++ abstraction for TCP sockets
    // It will close itself when it goes out of scope
    class Socket {
    public:
        // Initializes the Socket object
        Socket() {
            // Ensure GPTSock has started
            if ( !_started ) {
                _started = true;
                GPTSockInit();
            }

            sock = NewGPTSocket();
            if ( sock == INVALID_SOCKET )
                std::cout << "[TSL] Failed to create socket!\n";
        }

        // Initializes an existing sock_t
        Socket( sock_t sock ) : sock( sock ) {}

        // Binds server socket to a port
        int Bind( int port ) {
            int result = GPTSockBind( sock, port );
            if ( result == SOCKET_ERROR )
                std::cout << "[TSL] Failed to bind socket to port "
                    << port << "!\n";
            
            return result;
        }

        // Starts listening for clients
        int Listen( int backlog ) {
            return GPTSockListen( sock, backlog );
        }

        // Accepts a client's connection
        Socket Accept() {
            sock_t clientSock = GPTSockAccept( sock );
            if ( clientSock == INVALID_SOCKET )
                std::cout << "[TSL] Failed to accept client!\n";

            return Socket( clientSock );
        }

        // Connects to a server
        int Connect( const char *ip, int port ) {
            return GPTSockConnect( sock, ip, port );
        }

        // Sends a Buffer to the current connection
        void Send( Buffer *buffer ) {
            for (
                int sent = 0;
                sent < buffer->length();
                sent += GPTSockSend(
                        sock,
                        (const char*)buffer->data() + sent,
                        buffer->length() - sent
                    )
            );
        }

        // Receives a Buffer from the current connection
        Buffer Recv() {
            Buffer buffer = Buffer();
            GPTSockRecv(
                sock,
                (char*)buffer.data(),
                buffer.length()
            );

            return buffer;
        }

        // Closes the connection
        void Close() {
            GPTSockClose( sock );
        }


    private:
        sock_t sock;
        
    };
}