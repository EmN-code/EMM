#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <emcute.h>
#include <sock/udp.h>

#define EMCUTE_PORT           (1883U)
#define MQTT_BROKER_IP        "47.102.103.1"
#define PUB_INTERVAL          (5U)
#define TOPIC_NAME            "state"
#define NODE_NAME             "esp32_node"

static emcute_topic_t topic;

static void *publish_thread(void *arg) {
    (void)arg;
    char *states[2] = {"work", "not work"};
    int state_index = 0;
    
    while (1) {
        xtimer_sleep(PUB_INTERVAL);

        state_index = !state_index; // toggle between 0 and 1
        printf("Publishing state: %s\n", states[state_index]);

        if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
    }
    return NULL;
}

int main(void) {
    struct sockaddr_in gw;
    memset(&gw, 0, sizeof(struct sockaddr_in));
    gw.sin_family = AF_INET;
    gw.sin_port = htons(EMCUTE_PORT);
    gw.sin_addr.s_addr = inet_addr(MQTT_BROKER_IP);

    if (emcute_sock(&gw, sizeof(struct sockaddr_in)) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return -1;
    }

    topic.name = TOPIC_NAME;
    topic.id = 0; // will be set by emcute

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return -1;
    }

    puts("Starting publish thread\n");
    thread_create(publish_thread, NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0, "publish_thread");

    return 0;
}