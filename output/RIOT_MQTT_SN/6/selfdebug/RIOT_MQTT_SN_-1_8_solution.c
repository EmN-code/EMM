#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/conn/ip.h>
#include <xtimer.h>

#define BROKER_IP    "47.102.103.1"
#define BROKER_PORT  1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC   "device/state"

static emcute_topic_t topic;

void *publisher(void *arg) {
    (void) arg;

    const char *states[] = {"work", "not work"};
    size_t state_count = sizeof(states) / sizeof(states[0]);
    unsigned counter = 0;

    while (1) {
        const char *state = states[counter % state_count];
        printf("[Publisher] Publishing state: %s\n", state);
        int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);

        if (res != EMCUTE_OK) {
            printf("[Publisher] Error: unable to publish (code %d)\n", res);
        }
        
        counter++;
        xtimer_sleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    /* Set up EMCUTE */
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = MQTT_TOPIC;

    sock_udp_ep_t gw = { .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    printf("[MQTT-SN] Connecting to broker at %s:%u\n", BROKER_IP, BROKER_PORT);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[MQTT-SN] Error connecting to broker\n");
        return 1;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[MQTT-SN] Error: Unable to register topic\n");
        return 1;
    }

    /* Create a thread for publishing */
    char pub_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(pub_thread_stack, sizeof(pub_thread_stack), 
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publisher, NULL, "publisher");

    /* Run emcute, block the main thread */
    emcute_run(BROKER_PORT, "esp32_client");
    
    return 0;
}
