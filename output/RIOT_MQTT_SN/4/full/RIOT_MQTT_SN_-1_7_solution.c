#include <xtimer.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <dht.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <ztimer.h>
#include <time_units.h>
#include <timex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <MQTTClient.h>

#define EMCUTE_ID 0
#define EMCUTE_QUEUE_SIZE 4
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL 5ULL

static char stack[THREAD_STACKSIZE_SMALL];
static msg_t queue[EMCUTE_QUEUE_SIZE];
static kernel_pid_t main_thread_pid;

static int publish_state(void) {
    static emcute_topic_t topic;
    static const char *topic_name = "device/state";
    topic.name = topic_name;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    const char *state = "work or not work";
    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Failed to publish state\n");
        return 1;
    }
    return 0;
}

static void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gateway = {.port = BROKER_PORT};
    inet_pton(AF_INET, BROKER_IP, &gateway.addr.ipv4);

    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, EMCUTE_QUEUE_SIZE);
    main_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt");
    if (main_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
    }
    return 0;
}