
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <thread.h>
#include <ztimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

typedef struct {
    char name[64];
    uint16_t id;
} emcute_topic_t;

static emcute_topic_t topic;
static sock_udp_ep_t remote;
static sock_udp_t sock;

static void publish_state(void) {
    static char state_message[] = STATE_MESSAGE;
    int res = sock_udp_send(&sock, state_message, sizeof(state_message) - 1, &remote);
    if (res < 0) {
        printf("Failed to publish state\n");
    } else {
        printf("State published\n");
    }
}

static void connect_to_broker(void) {
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    int res = sock_udp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        printf("Failed to create UDP socket\n");
        return;
    }

    printf("Connected to MQTT-SN broker\n");
}

static void register_topic(void) {
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    topic.id = 0; // Topic ID will be assigned by the broker

    // Simulate topic registration success
    topic.id = 1; // Assuming the broker assigns ID 1
    printf("Topic registered with ID %u\n", topic.id);
}

static void periodic_publish(void) {
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }
}

int main(void) {
    connect_to_broker();
    register_topic();
    periodic_publish();

    return 0;
}
