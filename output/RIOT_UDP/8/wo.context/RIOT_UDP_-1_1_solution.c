#include <xtensa/hal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <net/inet.h>
#include <net/af.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5U)

void *send_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &server.addr) != 1) {
        printf("Error: unable to parse server address\n");
        return NULL;
    }
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return NULL;
    }

    while (1) {
        sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &server);
        printf("Message sent to server: %s\n", DEVICE_STATE);
        xtimer_sleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting UDP client...\n");
    send_thread(NULL);
    return 0;
}