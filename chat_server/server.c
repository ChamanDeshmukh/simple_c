#include <stdio.h>
#include <sys/poll.h>
#include<sys/socket.h>
#include "netinet/in.h"
#include "poll.h"
#include "unistd.h"

int main(int argc, char *argv[])
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        AF_INET,
        htons(9090),
        0
    }; 

    bind(sock_fd,&addr,sizeof(addr));

    listen(sock_fd, 10);
    int clint_fd = accept(sock_fd,0,0);

    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0
        },
        {
            clint_fd,
            POLLIN,
            0
        }   
    };
    

    for (;;) {
        char buffer[256] = {0};
        poll(fds, 2, 50000);
    
        if(fds[0].revents & POLLIN){
           read(0,buffer,255);
           send(clint_fd, buffer, 255, 0);
        }
        else if(fds[1].revents & POLLIN){
            if(recv(clint_fd, buffer, 255, 0) == 0){
                return 0;
            }
            printf("%s\n",buffer);
        }
    }
    return 0;
}
