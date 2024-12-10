// Correcting the error by including the necessary header files

#include <stdio.h>
#include <stdlib.h>
#include "net/sock/udp.h"  // To handle UDP socket operations related to MQTT-SN
#include "net/emcute.h"     // To access emcute library functions and constants
#include "xtimer.h"        // To introduce periodic waiting functions

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6,
                            .port = 1883,
                            .ipv6 = { 
                                .u8 = {47, 102, 103, 1} // IPv4 mapped in IPv6 format
                            }};

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publication for topic %s [%i] \n", topic->name, topic->id);
    if (len) {
        printf("Data: %.*s\n", len, (char *)data);
    }
    else {
        puts("[no data]\n");
    }
}

int main(void) {
    printf("MQTT-SN client example\n");
    
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(1883, "ESP32_Client"); // Initialize emcute

    // Setup connection to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Connection failed\n");
        return 1;
    }

    // Topic registration
    topic.name = "state/work_status";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        const char *data = "work";
        size_t len = strlen(data);

        if (emcute_pub(&topic, data, len, EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish data\n");
        }
        else {
            printf("Successfully published data\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}