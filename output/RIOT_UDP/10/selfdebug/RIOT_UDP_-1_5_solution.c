#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/sock/udp.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work or not work"
#define DELAY_SEC (5U * 1000000U) // 5 seconds in microseconds

void *network_thread(void *arg);

int main(void) {
    printf("Starting UDP client\n");
    thread_create(network_thread_stack, sizeof(network_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, network_thread, NULL, "network");
    return 0;
}

void *network_thread(void *arg) {
    (void)arg; // Unused variable
    sock_udp_ep_t server = {.family = AF_INET, .port = UDP_SERVER_PORT};
    inet_pton(AF_INET, UDP_SERVER_IP, &server.addr.ipv4);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);
    while (1) {
        ssize_t res = sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &server);
        if (res <= 0) {
            printf("Error sending data\n");
        }
        else {
            printf("Sent: %s\n", DEVICE_STATE);
        }
        xtimer_usleep(DELAY_SEC);
    }
    return NULL;
}