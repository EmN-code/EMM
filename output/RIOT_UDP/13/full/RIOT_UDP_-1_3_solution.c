#include "udp.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SLEEP_INTERVAL 5

void udp_send_state(void) {
    int sock;
    struct sockaddr_in server_addr;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    remote.addr.ipv4[0] = 47;
    remote.addr.ipv4[1] = 102;
    remote.addr.ipv4[2] = 103;
    remote.addr.ipv4[3] = 1;

    while (1) {
        // Try to create socket
        while ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            printf("Socket creation failed\n");
            sleep(1);
        }

        // Prepare the server address struct
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        inet_aton(SERVER_IP, &server_addr.sin_addr);

        // Try to send data to the server
        while (sendto(sock, DEVICE_STATE, sizeof(DEVICE_STATE), 0,
                      (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printf("Send failed\n");
            sleep(1);
        }

        printf("Data sent: %s\n", DEVICE_STATE);

        // Close the socket
        close(sock);

        sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    udp_send_state();
    return 0;
}