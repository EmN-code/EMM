#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U) /* seconds */

static const char *mqtt_topic = "state";
static const char *mqtt_message = "work or not work";

void *mqtt_publisher(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET,
                             .port = MQTT_BROKER_PORT };

    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        printf("Publishing message: %s to topic: %s\n", mqtt_message, mqtt_topic);
        sock_udp_send(&sock, mqtt_message, strlen(mqtt_message), &remote);

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(mqtt_publisher, NULL);
    return 0;
}