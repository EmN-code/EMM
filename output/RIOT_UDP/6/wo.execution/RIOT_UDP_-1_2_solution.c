#include "sys/socket.h"
#include "net/sock/util.h"
#include <stdio.h>
#include <string.h>

#define SERVER_PORT "20001"
#define SERVER_ADDR "47.102.103.1"
#define MESSAGE "work"
#define DELAY_S (5U)

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = atoi(SERVER_PORT) };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        int res = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote);
        if (res < 0) {
            printf("Error sending data: %d\n", res);
        } else {
            printf("Message sent\n");
        }
        sleep(DELAY_S);
    }
    return 0;
}