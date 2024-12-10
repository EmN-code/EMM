
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

void publish_state(emcute_topic_t *topic) {
    const char *state = "work";
    emcute_pub(topic, state, strlen(state), 0);
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    sock_udp_ep_t gw = { .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state(&topic);
        sleep(5);
    }

    return 0;
}
