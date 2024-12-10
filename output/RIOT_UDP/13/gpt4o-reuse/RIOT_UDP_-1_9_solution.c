
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>  // Include this header for inet_pton
#include "xtimer.h"     // Include this header for xtimer

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(int fd, struct sockaddr_in *dst)
{
    socklen_t len;
    while(1)
    {
        char buf[BUFF_LEN] = "work";  // Example device state
        len = sizeof(*dst);
        printf("client: %s\n", buf);  // Print the message to be sent
        sendto(fd, buf, BUFF_LEN, 0, (struct sockaddr*)dst, len);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
}

int main(void)
{
    int client_fd;
    struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &ser_addr.sin_addr);  // Convert IP address from text to binary form
    ser_addr.sin_port = htons(SERVER_PORT);  // Convert port number to network byte order

    udp_msg_sender(client_fd, &ser_addr);

    close(client_fd);

    return 0;
}
