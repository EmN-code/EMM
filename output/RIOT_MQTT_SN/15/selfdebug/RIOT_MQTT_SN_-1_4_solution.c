#include "net/emcute.h"
#include "xtimer.h"
#include "stdio.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
#define SLEEP_INTERVAL      (5U * US_PER_SEC)  // 5 seconds
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// Callback function for received messages
static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char* received_data = (char*)data;
    printf("Message received on topic %s: %.*s\n", topic->name, (int)len, received_data);
}

// Connect to the MQTT-SN broker
static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse broker IP");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    return 0;
}

// Publish state
static int publish_state(const char* state) {
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Let emcute handle ID assignment

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }

    return 0;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT MQTT-SN example application");

    if (connect_broker() != 0) {
        return 1;
    }

    // Infinite loop to publish state every 5 seconds
    while (1) {
        publish_state("work");
        puts("Published: work");
        xtimer_sleep(SLEEP_INTERVAL);

        publish_state("not_work");
        puts("Published: not_work");
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}