#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"  // For MQTT network functionalities
#include "net/sock/udp.h"  // For socket network communication

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static void mqtt_publish(sock_udp_ep_t *remote) {
    // Define the message
    const char *topic = "device/status";
    const char *message = "work";

    // Prepare the packet
    char packet[128];
    snprintf(packet, sizeof(packet), "PUBLISH %s %s", topic, message);

    // Send packet through UDP
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);
    sock_udp_send(&sock, packet, strlen(packet), remote);
    sock_udp_close(&sock);
}

int main(void) {
    printf("Starting MQTT publisher to broker %s on port %d\n", BROKER_IP, BROKER_PORT);

    sock_udp_ep_t broker = {.family = AF_INET,
                            .port = BROKER_PORT};
    inet_pton(AF_INET, BROKER_IP, &broker.addr);

    while (1) {
        mqtt_publish(&broker);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}