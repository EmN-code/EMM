#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <dht.h>
#include <dht_params.h>
#include <msg.h>
#include <mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 64

int main(void) {
    printf("UDP client for ESP32\n");

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to parse the server IP\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return -1;
    }

    char message[BUFFER_SIZE] = "work";
    while (1) {
        ssize_t res = sock_udp_send(&sock, message, sizeof(message), &remote);
        if (res < 0) {
            printf("Error sending data\n");
        } else {
            printf("Sent %d bytes to server\n", (int)res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}