#include "xtimer.h"
#include "emcute.h"
#include "net/ipv6/addr.h"
#include <string.h>
#include <arpa/inet.h>  // Required for inet_pton

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MSG_WORK "work"
#define MQTT_MSG_NOT_WORK "not work"
#define PUB_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds

static emcute_topic_t topic;

static void *publisher(void *arg) {
    (void)arg;
    while (1) {
        int res = emcute_pub(&topic, MQTT_MSG_WORK, strlen(MQTT_MSG_WORK), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_stack, sizeof(_stack)/sizeof(_stack[0]));

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return 1;
    }

    strcpy(topic.name, MQTT_TOPIC);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    printf("Connected and registered topic. Starting publisher thread...\n");
    thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher, NULL, "publisher");

    return 0;  // Keep the main function running
}