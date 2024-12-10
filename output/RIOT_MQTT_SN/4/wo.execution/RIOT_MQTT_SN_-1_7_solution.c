#include <assert.h>
#include <arpa/inet.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>

#define EMCUTE_PORT 1883
#define MQTT_BROKER_IP "47.102.103.1"
#define TOPIC_NAME "state"  

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Publishing state...\n");
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%i\n", MQTT_BROKER_IP, EMCUTE_PORT);
        return 1;
    }

    // Registering a topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, thread_handler, NULL, "mqtt_pub");
    return 0;
}