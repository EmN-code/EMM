
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char buffer[BUFFER_SIZE];

int main(void)
{
    sock_tcp_t sock;
    struct sockaddr_in server_addr;
    int res;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    res = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    if (res <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    res = sock_tcp_create(&sock, NULL, &server_addr, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Error: Failed to create socket");
        return 1;
    }

    res = sock_tcp_connect(&sock, &server_addr, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Error: Failed to connect to server");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        const char *message = "work or not work";
        int msg_len = strlen(message);

        res = sock_tcp_write(&sock, message, msg_len);
        if (res < 0) {
            puts("Error: Failed to send message");
            break;
        }

        printf("Message sent: %s\n", message);

        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
