#include <stdlib.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h> 
#include <kernel_defines.h> 
#include <mutex.h> 
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h> 
#include <net/sock/udp.h> 
#include <net/sock/util.h> 
#include <net/gcoap.h> 
#include <net/emcute.h> 
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <esp_wifi.h>
#include <esp_system.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL 5000000 // 5 seconds in microseconds

int main(void) {
    struct sockaddr_in server_addr;
    int sock;
    char *state = "work";

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);

    if (inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton error");
        close(sock);
        return EXIT_FAILURE;
    }

    while (1) {
        if (sendto(sock, state, sizeof(state), 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
        }
        xtimer_usleep(SEND_INTERVAL); // wait for 5 seconds
    }

    close(sock);
    return 0;
}