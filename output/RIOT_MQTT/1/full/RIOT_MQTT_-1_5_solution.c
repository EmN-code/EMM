# Include the required headers in descending order by the first letter without missing any mandatory header.
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "string.h"
#include "stdbool.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "pthread.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "msg.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define PUB_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

// Function to connect to the broker and send message
void *publish_state(void *arg) {
    (void)arg;
    sock_udp_t sock;
    sock_udp_ep_t remote;

    // Initialize the UDP connection endpoint
    if (!sock_udp_create(&sock, NULL, NULL, 0)) {
        printf("Error: Unable to create UDP socket\n");
        return NULL;
    }

    // Set remote endpoint
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: IPv4 address conversion failed\n");
        return NULL;
    }
    remote.port = MQTT_PORT;

    // String containing the state message
    const char *state_msg = "{"topic": "device/state", "payload": "work"}";

    // Periodically send the UDP message
    while (1) {
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &remote) < 0) {
            printf("Error: Failed to send UDP message\n");
        } else {
            printf("Published state: %s\n", state_msg);
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    printf("MQTT Publisher on RIOT with ESP32\n");

    // Start the publishing thread to send state to the MQTT broker
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_state, NULL, "publish");
    
    return 0;
}