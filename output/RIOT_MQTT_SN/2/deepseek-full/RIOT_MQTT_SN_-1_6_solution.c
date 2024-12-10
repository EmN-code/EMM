
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"
#define RETRY_COUNT 5
#define SLEEP_TIME 5000000

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

static void _on_pub(void *arg, int res) {
    (void)arg;
    printf("Publish result: %d\n", res);
}

int main(void) {
    int result;
    unsigned flags = 0;

    // Convert IP address string to byte representation
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    result = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (result != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    result = emcute_reg(&topic);
    if (result != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish message every 5 seconds
    while (1) {
        result = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, flags);
        if (result != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published successfully");
        }

        // Sleep for 5 seconds
        thread_sleep(SLEEP_TIME);
    }

    return 0;
}
