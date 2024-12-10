#include <ztimer.h>
#include <xtimer.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5

int main(void) {
    printf("Starting UDP client example with RIOT and ESP32\n");

    // Initialize the UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t server;

    char server_addr[] = SERVER_IP;
    server.port = SERVER_PORT;
    inet_pton(AF_INET, server_addr, &server.addr.ipv4);
    int retry_count = 0;

    // Create the socket and handle potential connection issues
    while (sock_udp_create(&sock, &local, NULL, 0) < 0) { 
        printf("Failed to create a UDP socket, attempt: %d\n", retry_count);
        xtimer_sleep(1);
        retry_count++;
    }

    printf("Socket created successfully\n");

    // Send device state every MESSAGE_INTERVAL
    while (1) {
        const char *state = "work";  // Example state message
        ssize_t res = sock_udp_send(&sock, state, strlen(state), &server);

        if (res < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent to %s:%d\n", SERVER_IP, SERVER_PORT);
        }

        ztimer_sleep(ZTIMER_SEC, MESSAGE_INTERVAL);
    }

    /* As per the requirements, use another approach instead of close() */
    //sock_udp_close(&sock);  // Normally, you would close the socket like this

    return 0;
}