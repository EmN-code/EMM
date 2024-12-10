
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    // This function is defined but not used in this example
}

int main(void)
{
    // Initialize the UDP endpoint for the gateway
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Initialize the EMCUTE stack
    emcute_run(SERVER_PORT, CLIENT_ID);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return 1;
    }
    puts("Connected to MQTT-SN gateway");

    // Register the topic
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }
    puts("Topic registered");

    // Publish state every 5 seconds
    char msg[20];
    while (1) {
        strcpy(msg, "work");
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("Published: work");
        }

        xtimer_usleep(MSG_INTERVAL);

        strcpy(msg, "not work");
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("Published: not work");
        }

        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
