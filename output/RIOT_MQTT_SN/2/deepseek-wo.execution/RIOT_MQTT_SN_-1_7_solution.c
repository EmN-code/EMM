
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    emcute_topic_t topic;
    char topic_name[CONFIG_EMCUTE_TOPIC_MAXLEN];
    int result;

    // Convert IP address string to byte representation
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: Invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    result = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (result != EMCUTE_OK) {
        printf("Error: Could not connect to MQTT-SN gateway (code: %d)\n", result);
        return 1;
    }

    // Register the topic
    strncpy(topic_name, TOPIC_NAME, sizeof(topic_name) - 1);
    topic_name[sizeof(topic_name) - 1] = '\0';
    topic.name = topic_name;
    result = emcute_reg(&topic);
    if (result != EMCUTE_OK) {
        printf("Error: Could not register topic (code: %d)\n", result);
        return 1;
    }

    // Publish the state every five seconds
    while (1) {
        result = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (result != EMCUTE_OK) {
            printf("Error: Could not publish message (code: %d)\n", result);
        } else {
            puts("Message published successfully");
        }
        xtimer_sleep(5);
    }

    return 0;
}
