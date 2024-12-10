#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock.h"
#include "net/emcute.h"
#include <stdio.h>

#define SLEEP_DURATION   (5U)
#define EMCUTE_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME       "riot/test"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication of length %u\n", (unsigned)len);    
}

int main(void) {
    random_init(0);

    // Prepare gateway socket
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // Initialize emcute
    emcute_run(EMCUTE_PRIO, NULL, EMCUTE_BUFSIZE);

    // Connect with retry handling
    printf("Attempting to connect to MQTT-SN broker\n");
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to broker at %s\n", MQTT_BROKER_IP);
            break;
        }
        else {
            printf("Connection failed: %d. Retrying...\n", res);
            xtimer_sleep(1);
        }
    }
    
    // Register topic
    topic.name = TOPIC_NAME;
    while (1) {
        int res = emcute_reg(&topic);
        if (res == EMCUTE_OK) {
            printf("Registered topic %s\n", topic.name);
            break;
        }
        else {
            printf("Topic registration failed: %d. Retrying...\n", res);
            xtimer_sleep(1);
        }
    }

    // Periodic publish loop
    printf("Starting publish loop\n");
    while (1) {
        const char *message = "work";
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res == EMCUTE_OK) {
            printf("Published: %s\n", message);
        }
        else {
            printf("Publish failed: %d\n", res);
        }
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}