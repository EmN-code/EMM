#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "thread.h"
#include "event/thread.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

static char device_state[15];

void send_device_state(sock_udp_ep_t remote) {
    sock_udp_t sock;
    uint8_t retries = 0;
    ssize_t res;

    while (1) {
        if (retries >= 5) {
            printf("Max retries reached, could not send state.\n");
            break;
        }

        res = sock_udp_create(&sock, NULL, NULL, 0);
        if (res < 0) {
            printf("Error creating UDP socket\n");
            retries++;
            continue;
        }

        res = sock_udp_send(&sock, device_state, sizeof(device_state), &remote);
        if (res < 0) {
            printf("Error sending UDP packet\n");
            retries++;
        } else {
            printf("Device state sent: %s\n", device_state);
            break;
        }

        sock_udp_close(&sock);
    }
}

int main(void) {
    strcpy(device_state, STATE_WORK);

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    while (1) {
        send_device_state(remote);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}