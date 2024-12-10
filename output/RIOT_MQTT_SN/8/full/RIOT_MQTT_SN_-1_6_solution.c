#include "ztimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "thread.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC "device/status"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define RECONNECT_DELAY (1U * ZTIMER_MSEC_PER_SEC)

static emcute_sub_t subscription;

int main(void) {
    puts("MQTT-SN Publisher with RIOT
");

    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    // Connect to MQTT-SN gateway with retry on failure
    int res;
    while (true) {
        res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("Successfully connected to the MQTT-SN gateway!");
            break;
        }
        else {
            puts("Error: Unable to connect to MQTT-SN gateway. Retrying in 1 second...");
            ztimer_sleep(ZTIMER_MSEC, RECONNECT_DELAY);
            continue;
        }
    }

    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;

    // Register topic
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error: Unable to register topic");
        return 1;
    }

    // Publish loop
    while (1) {
        // Publish "work"
        res = emcute_pub(&topic, MQTT_PAYLOAD_WORK, strlen(MQTT_PAYLOAD_WORK), 0);
        if (res != EMCUTE_OK) {
            puts("Error: Failed to publish data");
            continue;
        }

        puts("Published 'work' state.");

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);

        // Publish "not work"
        res = emcute_pub(&topic, MQTT_PAYLOAD_NOT_WORK, strlen(MQTT_PAYLOAD_NOT_WORK), 0);
        if (res != EMCUTE_OK) {
            puts("Error: Failed to publish data");
            continue;
        }

        puts("Published 'not work' state.");

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}