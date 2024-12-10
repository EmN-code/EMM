#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>  // Correct header for inet_pton
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/state"
#define WORK_MSG "work"
#define NOT_WORK_MSG "not work"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds interval

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    int work_state = 1; // Let's say 1 for 'work', 0 for 'not work'

    while (1) {
        const char* message = work_state == 1 ? WORK_MSG : NOT_WORK_MSG;
        printf("Publishing message '%s' to topic '%s'\n", message, TOPIC);

        // Code to publish the message to the MQTT broker
        // e.g., using a UDP socket to send the message

        work_state = !work_state; // Toggle state for the next message
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return 0;
}