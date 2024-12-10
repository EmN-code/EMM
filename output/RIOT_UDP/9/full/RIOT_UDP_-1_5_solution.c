#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "ztimer.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "mutex.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define SLEEP_TIME 5

int main(void) {

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse server IP\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return -1;
    }

    char buffer[128];
    int retries;

    while (1) {
        retries = 0;

        ssize_t res = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
            retries++;
            if (retries >= 3) {
                printf("Exceed maximum retries, giving up...\n");
                break;
            } else {
                printf("Retrying...\n");
                xtimer_sleep(1);
                continue;
            }
        }

        printf("Message sent: %s\n", MESSAGE);

        xtimer_sleep(SLEEP_TIME);
    }

    sock_udp_close(&sock);
    return 0;
}