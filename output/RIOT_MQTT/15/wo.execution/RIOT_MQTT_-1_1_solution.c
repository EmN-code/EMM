#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_TOPIC "device/state"
#define PUB_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
char line_buf[64];

static void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = 6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_ADDR, &remote.addr.ipv6);

    while (1) {
        char *payload = "work";

        printf("Publishing MQTT message: %s\n", payload);

        // Assuming there is a function to publish the data
        if (sock_udp_send(NULL, payload, strlen(payload), NULL, &remote) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent successfully\n");
        }

        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("RIOT MQTT example without emcute");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt");
    return 0;
}