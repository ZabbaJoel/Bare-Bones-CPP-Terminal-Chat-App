#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

//For the terminal input, input the IP address of the computer hosting the server
int main(int argc, char* argv[]) {
    
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    // Create a hint structure for the server we're connecting with
    int port = 54000;
    string ipAddress = argv[1]; //if connecting to a server on another device change the IP here!

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }

    // While loop:
    char buf[4096];
    string userInput;

    do {
        // Enter lines of text
        cout << "> ";
        getline(cin, userInput);

        // Send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1)
        {
            cout << "Could not send to server! Whoops!\r\n";
            continue;
        }

        // Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);

        // Display response
        cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";

    } while(true);

    // Close the socket
    close(sock);
    
    return 0;
}
