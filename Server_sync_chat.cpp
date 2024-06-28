// Server-side Synchronous Chatting Application
// using C++ boost::asio
#include <mutex>
#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

// Driver program for receiving data from buffer
string getData(tcp::socket& socket)
{
    boost::asio::streambuf buf;
    read_until(socket, buf, "\n");
    string data = buffer_cast<const char*>(buf.data());
    return data;
}

// Driver program to send data
void sendData(tcp::socket& socket, const string& message)
{
    write(socket, buffer(message + "\n"));
}

int main(int argc, char* argv[])
{
    io_context io_cntxt;

    // Listening for any new incomming connection at port 8099 with IPv4 protocol
    tcp::acceptor acceptor_srvr( io_cntxt, tcp::endpoint(tcp::v4(), 8099));
    // Creating socket object
    tcp::socket srvr_socket(io_cntxt);
    // waiting for connection [Synchronous]
    acceptor_srvr.accept(srvr_socket);

    // Reading username
    string u_name = getData(srvr_socket);
    // Removing "\n" from the username
    u_name.pop_back();

    // Replying with default message to initiate chat
    string response, reply;
    reply = "Hello " + u_name + "!";
    cout << "Server: " << reply << endl;
    sendData(srvr_socket, reply);

    while (true) {   // infinite loop
        // Fetching response
        response = getData(srvr_socket);
        // Popping last character "\n"
        response.pop_back();

        // Validating if the connection has to be closed
        if (response == "exit") {
            cout << u_name << " left!" << endl;
            break;
        }
        cout << u_name << ": " << response << endl;

        // Reading new message from input stream
        cout << "Server" << ": ";
        getline(cin, reply);
        sendData(srvr_socket, reply);

        if (reply == "exit")
            break;
    }
    return 0;
}