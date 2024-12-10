
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>

#define TOPIC_NAME "state"
#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define MSG_QUEUE_SIZE 10
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    emcute_pub(&topic, state, strlen(state), 0);
}

void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        publish_state();
        usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
