#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/ipv4/addr.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET,
                        .port = BROKER_PORT};

    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) == 0) {
        puts("Error: unable to parse broker IPV4 address");
        return 1;
    }

    puts("Connecting to MQTT broker...");

    while (1) {
        puts("Publishing state: work");
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}