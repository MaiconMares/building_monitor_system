#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../inc/queue.h"
#include "../inc/TCP_communication.h"

unsigned short central_server_port = 10034;
char central_server_address[256];


typedef struct server_info {
    unsigned short port;
    char ip_addr[13];
} server_info;

void *read_from_central_server(void *args){
    int server_socket;
    int client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    unsigned int client_length;

    server_info *my_server_info = (server_info *) args;

    if ((server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        printf("Error when opening server socket\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(my_server_info->ip_addr);
    server_addr.sin_port = htons(my_server_info->port);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        printf("Error when trying to bind\n");

    if (listen(server_socket, 10) < 0)
        printf("Error when trying to listen\n");

    while(1){
        printf("waiting for requests!\n");
        char buffer[1024];
        int length_msg;
        int msg;

        client_length = sizeof(client_addr);
        if((client_socket = accept(server_socket, (struct sockaddr *) &client_addr,
            &client_length)) < 0)
            printf("Error when trying to accept\n");

        printf("Connection received: %s\n", inet_ntoa(client_addr.sin_addr));

        if ((length_msg = recv(client_socket, buffer, 1024, 0)) < 0)
            printf("Error when receiving message from client\n");
        else {
            buffer[length_msg] = '\0';

            memcpy(&msg, buffer, 4);
            printf("%d\n", msg);
        }
        close(client_socket);
    }

    close(server_socket);
    return NULL;
}

void *write_to_central_server(void *args){
    int client_socket;
    struct sockaddr_in server_addr;
    int msg;
    char buffer[1024];

    server_info *my_server_info = (server_info *) args;

    while(1){
        if (!is_empty()){
            if((client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                printf("Error when trying to open socket\n");

            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = inet_addr(my_server_info->ip_addr);
            server_addr.sin_port = htons(my_server_info->port);

            if(connect(client_socket, (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0)
                printf("Error when trying to connect()\n");


            msg = remove_element();
            memcpy(buffer, &msg, 4);
            int msg_size = 4;

            if (send(client_socket, buffer, msg_size, 0) != msg_size)
                printf("Number of sent bytes are different from original message\n");
            else
                printf("Message sent successfully\n");

            close(client_socket);
        }
        usleep(10);
    }
    return NULL;
}