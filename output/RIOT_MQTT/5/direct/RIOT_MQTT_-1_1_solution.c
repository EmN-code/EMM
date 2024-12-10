#include <thread.h>
#include <timex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqtt.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

// The address of the MQTT broker and the default port.
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

// MQTT client ID
#define CLIENT_ID "esp32_client"

// Topic to publish the state
#define TOPIC "esp32/state"

// Function to publish the state
void *publish_state(void *arg) {
    emcute_sock_t sock_fd;
    sock_fd = emcute_sock_init();
    
    sock_fd.sin6_family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&sock_fd.sin6_addr, BROKER_IP);
    sock_fd.sin6_port = htons((uint16_t)BROKER_PORT);

    // Connect to the broker
    if (emcute_sock_connect(sock_fd, CLIENT_ID, true, NULL, 0) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    // Message buffer
    char message[20];
    int state = 0; // 0 means not work, 1 means work

    while (1) {
        // Toggle state
        state = !state;
        // Prepare the message
        snprintf(message, sizeof(message), "%s", state ? "work" : "not work");

        // Publish the message
        if (emcute_sock_pub(sock_fd, TOPIC, (uint8_t *)message, strlen(message), EMCUTE_QOS_0) < 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", message);
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    // Clean up
    emcute_sock_close(sock_fd);
    return NULL;
}

int main(void) {
    // Start the publish thread
    kernel_pid_t publish_pid = thread_create(
        stack,
        sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        publish_state,
        NULL,
        "publish_state"
    );

    // Wait for the publish thread to terminate
    thread_join(publish_pid, NULL);
    return 0;
}