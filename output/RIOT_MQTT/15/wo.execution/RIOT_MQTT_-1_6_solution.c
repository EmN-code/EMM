#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883

static const char *states[2] = {"Not Work", "Work"};

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sock);
        return 1;
    }

    while (1) {
        for (int i = 0; i < 2; i++) {
            int send_res = sendto(sock, states[i], strlen(states[i]), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
            if (send_res < 0) {
                perror("sendto failed");
            }
            usleep(5000000); // Sleep for 5 seconds
        }
    }

    close(sock);
    return 0;
}