
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <mutex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;
static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

void publish_state(void) {
    const char *state = "work or not work";
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    } else {
        printf("State published: %s\n", state);
    }
}

void *mqtt_thread(void *arg) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Failed to convert IP address\n");
        return NULL;
    }

    res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
        return NULL;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}
