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
#if defined(_WIN32)
#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#endif

void process_request(
    const std::string& request,
    std::string& response);

void server_loop();

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
    }
    return status;
}

void server_loop()
{
    int server_fd, client_fd, opt(1), valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    char buffer[1024] = {0};
    std::stringstream request;
    std::string response;
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

    client_fd = accept(
        server_fd, 
        (struct sockaddr *)&address,  
        (socklen_t*)&addrlen);
    if(client_fd < 0)
    {
        throw std::runtime_error("accept failed");
    }

    do
    {
        valread = read(client_fd,buffer,1024);
        request << buffer;
    } while (valread == 1024);
    std::cerr << "localhost received: " << request.str() << std::endl;
    process_request(request.str(),response);
    send(client_fd,response.c_str(),response.size(),0);
    close(server_fd);
    close(client_fd);
}

void process_request(
    const std::string& request,
    std::string& response)
{
    std::string error;
    std::stringstream response_stream;
    json11::Json::array request_array = json11::Json::parse(
        request.c_str(),
        error).array_items();
    if (request_array.size() > 0)
    {
        if(request_array[0] == "add")
        {
            if(request_array.size() == 3)
            {
                std::string a = request_array[1].string_value().c_str();
                std::string b = request_array[2].string_value().c_str();
                std::cerr << a 
                        << " + " 
                        << b 
                        << std::endl;
                response_stream << atoi(a.c_str()) + atoi(b.c_str());
            }
            else
            {
                response_stream << "syntax: add a b" << std::endl;
            }
        }
        else if(request_array[0] == "sub")
        {
            if(request_array.size() == 3)
            {
                std::string a = request_array[1].string_value().c_str();
                std::string b = request_array[2].string_value().c_str();
                std::cerr << a 
                        << " - " 
                        << b 
                        << std::endl;
                response_stream << atoi(a.c_str()) - atoi(b.c_str());
            }
            else
            {
                response_stream << "syntax: sub a b" << std::endl;
            }            
        }
        else if(request_array[0] == "mlt")
        {
            if(request_array.size() == 3)
            {
                std::string a = request_array[1].string_value().c_str();
                std::string b = request_array[2].string_value().c_str();
                std::cerr << a 
                        << " * " 
                        << b 
                        << std::endl;
                response_stream << atoi(a.c_str()) * atoi(b.c_str());
            }
            else
            {
                response_stream << "syntax: mlt a b" << std::endl;
            }
        }
        else if(request_array[0] == "div")
        {
            if(request_array.size() == 3)
            {
                std::string a = request_array[1].string_value().c_str();
                std::string b = request_array[2].string_value().c_str();
                std::cerr << a 
                        << " / " 
                        << b 
                        << std::endl;
                response_stream << atoi(a.c_str()) / atoi(b.c_str());
            }
            else
            {
                response_stream << "syntax: div a b" << std::endl;
            }
        }
        else if(request_array[0] == "help")
        {
            response_stream << "Available commands:" <<std::endl
                            << "add" << std::endl
                            << "sub" << std::endl
                            << "mlt" << std::endl
                            << "div" << std::endl
                            << "exit" << std::endl;
        }
        else if(request_array[0] == "exit")
        {
            response_stream << "Goodbye!" << std::endl;
        }
        else
        {
            response_stream << "Invalid command."
                            << "Try \"help\" for more information." 
                            << std::endl;
        }
        response = response_stream.str();
    }
    else
    {
        response = "no command";
    }
}