/**
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * file: server.cpp
 * author: John Patek
 * email: jpatek@ubiqsecurity.com
 */

#include <json11/json11.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 


void server_loop()
{
    int server_fd, connection, opt(1), valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    char buffer[1024] = {0};
    std::stringstream request;
    std::string response = "hello from the server";
    if(server_fd < 0)
    {
        throw std::runtime_error("socket failed");
    }

    if (setsockopt(
        server_fd, 
        SOL_SOCKET, 
        SO_REUSEADDR | SO_REUSEPORT, 
        &opt, 
        sizeof(opt)))
    {
        throw std::runtime_error("sockopt failed");
    } 

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(
        server_fd, 
        (struct sockaddr *)&address,  
        sizeof(address)) < 0)
    {
        throw std::runtime_error("bind afiled");
    }

    if(listen(server_fd, 3) < 0)
    {
        throw std::runtime_error("listen failed");
    }

    connection = accept(
        server_fd, 
        (struct sockaddr *)&address,  
        (socklen_t*)&addrlen);
    if(connection < 0)
    {
        throw std::runtime_error("accept failed");
    }

    do
    {
        valread = read(connection,buffer,1024);
        request << buffer;
    } while (valread == 1024);
    std::cerr << "localhost received: " << request.str() << std::endl;
    send(connection,response.c_str(),response.size(),0);
    close(server_fd);
    close(connection);
}

int main()
{
    int status(EXIT_SUCCESS);
    bool exit(false);
    while(!exit)
    {
        try
        {
            server_loop();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit = true;
        }
        
        server_loop();
    }
    return status;
}