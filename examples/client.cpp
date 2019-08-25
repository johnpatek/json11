/**
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * file: client.cpp
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

void tokenize(
    const std::string& line,
    std::vector<std::string>& tokens)
{
    std::istringstream line_buffer(line);
    std::copy(
        std::istream_iterator<std::string>(line_buffer),
        std::istream_iterator<std::string>(),
        std::back_inserter(tokens));
}

void serialize(
    const std::vector<std::string>& tokens,
    json11::Json& command)
{
    json11::Json::array result;
    for(const std::string& token : tokens)
    {
        result.push_back(token);
    }
    command = result;
}

void client(
    std::string host,
    const uint32_t port,
    std::string message)
{
    int client_fd(0), valread(0);
    struct sockaddr_in address;
    char buffer[1024] = {0};
    std::stringstream response;

    client_fd = socket(AF_INET,SOCK_STREAM,0);
    
    if(client_fd < 1)
    {
        throw std::runtime_error("Socket creation error");
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if(inet_pton(AF_INET,host.c_str(), &address.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid address/ Address not supported");
    }

    if(connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        throw std::runtime_error("Connection failed");
    }

    send(client_fd, message.c_str(), message.size(),0);
    do
    {
        valread = read(client_fd,buffer,1024);
        response << buffer;
    }
    while(valread == 1024);

    std::cerr << response.str() << std::endl;
    close(client_fd);
}

int main()
{
    int status(EXIT_SUCCESS);
    std::vector<std::string> tokens;
    std::string line,message,error;
    json11::Json command;
    do
    {
        std::cerr << "client@127.0.0.1$ ";
        std::getline(std::cin,line);
        tokenize(line,tokens);
        serialize(tokens,command);
        try
        {
            client("127.0.0.1", 8080,command.dump());
        } 
        catch(std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        tokens.clear();
    } while (line != "exit");
    return status;
}