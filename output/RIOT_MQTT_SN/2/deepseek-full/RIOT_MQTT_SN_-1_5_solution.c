
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"
#define MESSAGE_LEN (sizeof(MESSAGE) - 1)
#define RETRY_COUNT 5
#define SLEEP_TIME (5 * 1000000UL) // 5 seconds in microseconds

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

static void _on_pub(void *arg, int res) {
    (void)arg;
    printf("Publish result: %s\n", (res == EMCUTE_OK) ? "success" : "failure");
}

int main(void) {
    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: failed to connect to MQTT-SN gateway (%d)\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: failed to register topic (%d)\n", res);
        return 1;
    }

    // Publish loop
    while (1) {
        res = emcute_pub(&topic, MESSAGE, MESSAGE_LEN, 0);
        if (res != EMCUTE_OK) {
            printf("Error: failed to publish message (%d)\n", res);
        }

        // Sleep for 5 seconds
        thread_sleep(SLEEP_TIME);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
