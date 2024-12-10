
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <stdbool.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static sock_udp_ep_t broker_ep = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

int main(void)
{
    // Initialize the endpoint with the broker's IP address
    if (inet_pton(AF_INET, BROKER_IP, &broker_ep.addr.ipv4_addr) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&broker_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker (%d)\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (%d)\n", res);
        emcute_discon();
        return 1;
    }

    // Publish the message every 5 seconds
    while (1) {
        res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish message (%d)\n", res);
        } else {
            puts("Message published successfully");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect from the broker
    emcute_discon();

    return 0;
}
