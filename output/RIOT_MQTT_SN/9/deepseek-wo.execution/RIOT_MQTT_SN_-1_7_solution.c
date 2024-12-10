
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work";
    emcute_pub(&topic, state, strlen(state), 0);
    sleep(5);
}

int main(void) {
    sock_udp_ep_t gw = { .port = MQTT_SN_BROKER_PORT };
    inet_aton(MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        return 1;
    }

    while (1) {
        publish_state();
    }

    return 0;
}
