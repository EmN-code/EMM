
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <msg.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <timex.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t broker;
static emcute_topic_t topic;
static mutex_t mtx = MUTEX_INIT;

void connect_to_broker(void) {
    broker.family = AF_INET;
    broker.port = BROKER_PORT;
    inet_aton(BROKER_ADDR, (struct in_addr *)&broker.addr.ipv4);

    int res = emcute_con(&broker, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return;
    }
    printf("Connected to MQTT-SN broker at %s:%d\n", BROKER_ADDR, BROKER_PORT);
}

void register_topic(void) {
    topic.name = TOPIC_NAME;
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return;
    }
    printf("Topic '%s' registered with ID %u\n", topic.name, (unsigned)topic.id);
}

void publish_message(void) {
    int res = emcute_pub(&topic, MSG_CONTENT, sizeof(MSG_CONTENT) - 1, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Message published to topic '%s'\n", topic.name);
    }
}

void periodic_publish(void) {
    while (1) {
        xtimer_usleep(PUBLISH_INTERVAL);
        mutex_lock(&mtx);
        publish_message();
        mutex_unlock(&mtx);
    }
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    connect_to_broker();
    register_topic();

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, periodic_publish, NULL, "periodic_publish");

    return 0;
}
