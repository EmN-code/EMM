
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <stdio.h>
#include <dht_params.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 10
#define MSG_TYPE_PUB 1

static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;
static sock_udp_ep_t gw;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    emcute_pub(&topic, state, strlen(state), 0);
}

void *emcute_thread(void *arg) {
    (void)arg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    gw.family = AF_INET;
    gw.port = HTONS(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name) - 1);
    topic.name[sizeof(topic.name) - 1] = '\0';

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.type == MSG_TYPE_PUB) {
            publish_state();
        }
    }

    return NULL;
}

int main(void) {
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    while (1) {
        msg_t msg;
        msg.type = MSG_TYPE_PUB;
        msg_send(&msg, thread_getpid(), true);
        xtimer_sleep(5);
    }

    return 0;
}
