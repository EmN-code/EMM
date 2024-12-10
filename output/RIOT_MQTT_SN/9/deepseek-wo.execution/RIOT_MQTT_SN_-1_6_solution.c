
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>

#define TOPIC_NAME "state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define MSG_QUEUE_SIZE 8
#define MSG_TYPE_PUB 1

static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work";
    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    }
}

void *publisher_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        publish_state();
        sleep(5);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_aton(BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    thread_t *pub_thread;
    pub_thread = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    if (pub_thread == NULL) {
        puts("Error: unable to create publisher thread");
        return 1;
    }

    emcute_run(BROKER_PORT, CLIENT_ID);

    return 0;
}
