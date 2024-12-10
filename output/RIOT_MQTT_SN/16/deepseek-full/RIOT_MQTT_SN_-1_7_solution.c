
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <msg.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static sock_udp_ep_t gw_ep = { .family = AF_INET };
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    emcute_pub(&topic, state, strlen(state), 0);
}

void connect_to_broker(void) {
    if (inet_aton(MQTT_SN_BROKER_IP, &gw_ep.addr.ipv4) == 0) {
        puts("Error: invalid broker IP address");
        return;
    }
    gw_ep.port = MQTT_SN_BROKER_PORT;

    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error: could not register topic");
        return;
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    connect_to_broker();

    while (1) {
        publish_state();
        sleep(5);
    }

    return 0;
}
