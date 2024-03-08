#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 13213

void handle_request(int client_socket)
{

    char response[] = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/html\r\n"
                      "\r\n"
                      "<!DOCTYPE html>\r\n"
                      "<html>\r\n"
                      "<head><title>Hello, World!</title></head>\r\n"
                      "<body>\r\n"
                      "<h1>Hello, World!</h1>\r\n"
                      "</body>\r\n"
                      "</html>\r\n";
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept and handle connections
    while (1)
    {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        handle_request(client_socket);
    }

    return 0;
}