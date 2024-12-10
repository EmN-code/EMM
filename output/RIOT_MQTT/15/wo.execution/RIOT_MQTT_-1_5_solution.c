#include <stddef.h>
#include <stdbool.h>
#include <timex.h>
#include <esp_wifi.h>
#include <event/thread.h>
#include <esp_system.h>
#include <mma8x5x_params.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <NET/sock/util.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <dht.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MESSAGE_INTERVAL_USEC (5 * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    char buffer[64];
    ssize_t res;

    while (true) {
        char *message = "work or not work";
        res = sock_udp_send(&sock, message, strlen(message), &remote);

        if (res < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("UDP packet sent: %s\n", message);
        }

        ztimer_sleep(ZTIMER_USEC, MESSAGE_INTERVAL_USEC);
    }

    return 0;
}