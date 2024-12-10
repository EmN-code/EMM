#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ztimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/af.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MESSAGE "work"
#define SLEEP_DURATION 5

int main(void) {
    printf("MQTT Client started\n");

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("ERROR: unable to parse destination address\n");
        return 1;
    }

    while (1) {
        // For demonstration purposes. An actual MQTT library usage is advised
        // Construct and send a PUBLISH message using an existing MQTT library API
        printf("Publishing message: %s\n", MESSAGE);

        // Simulate work by sleeping
        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION * 1000);
    }
    return 0;
}