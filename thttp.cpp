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

#include <iostream>
#include <thread>

#include "tsl.hpp"
#include "ClientRequest.hpp"


class THttp {
public:
    // Initializes the server
    THttp() {
        sock = tsl::Socket();
        sock.Bind( 80 );
    }

    // Server main loop - listens for and accepts connections
    void Run();

    // Client main loop - responses to requests
    void ClientHandler( tsl::Socket client );

private:
    tsl::Socket sock;
};

// Server main loop - listens for and accepts connections
void THttp::Run() {
    std::cout << "Listening...\n";
    while ( true ) {
        sock.Listen( 0 );
        tsl::Socket client = sock.Accept();
        std::thread clientHandler( &THttp::ClientHandler, this, client );
        clientHandler.detach();
    }
}

// Client main loop - responses to requests
void THttp::ClientHandler( tsl::Socket client ) {
    while ( true ) {
        tsl::Buffer inContent = client.Recv();
        if ( inContent.length() == 0 )
            break;
        
        ClientRequest request( inContent.data(), inContent.length() );
        if ( request.type() != ClientRequest::INVALID ) {
            switch ( request.type() ) {
                case ClientRequest::GET:  std::cout << "GET ";  break;
                case ClientRequest::POST: std::cout << "POST "; break;
            }
            std::cout << request.resource() << '\n';

            tsl::Buffer outContent(
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 12\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Hello World"
            );
            client.SendChunked( &outContent );
        }
    }
    client.Close();
}

int main() {
    THttp thttp = THttp();
    thttp.Run();

    return 0;
}