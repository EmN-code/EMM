
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MESSAGE "work"

static sock_udp_ep_t server_addr = { .family = AF_INET, .port = SERVER_PORT };

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;
    emcute_topic_t topic;

    /* Initialize the socket */
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Convert IP address from string to binary form */
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&server_addr, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        if (emcute_pub(&topic, MESSAGE, strlen(MESSAGE), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published");
        }
        xtimer_sleep(5);
    }

    /* Disconnect from the MQTT-SN broker */
    emcute_discon();

    return 0;
}
