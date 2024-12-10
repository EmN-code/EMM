#include <stdio.h>  // Required for printf
#include <string.h> // Required for strlen and memset

#include "net/sock/udp.h" // Required for UDP communication
#include "net/sock/tcp.h" // Required for TCP communication
#include "net/nanocoap.h"  // Required for CoAP communication
#include "net/ipv6/addr.h"  // Required for IPv6 address handling

#include "xtimer.h"  // Required for timer functionalities

// MQTT connection parameters
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

// Pseudo MQTT packet for publishing "work" state, just for demonstration
const char *work_mqtt_packet = "PUBLISH: state=work";

// Pseudo MQTT packet for publishing "not work" state, just for demonstration
const char *not_work_mqtt_packet = "PUBLISH: state=not work";

void publish_state(int socket) {
    // Toggle state between work and not work for this example
    static int work_state = 1;

    if (work_state) {
        sendto(socket, work_mqtt_packet, strlen(work_mqtt_packet), 0, NULL, 0);
        printf("Published: work\n");
    } else {
        sendto(socket, not_work_mqtt_packet, strlen(not_work_mqtt_packet), 0, NULL, 0);
        printf("Published: not work\n");
    }

    work_state = !work_state; // Toggle state
}

int main(void) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    // Placeholder for actual connection logic
    printf("Connect to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Publish state every 5 seconds
    while (1) {
        publish_state(sock);
        xtimer_sleep(5);
    }

    return 0;
}