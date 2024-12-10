#include <ztimer.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <msg.h>
#include <mutex.h>
#include <time_units.h>
#include <thread.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <event/thread.h>
#include <mboz.h>
#include <evtimer_mboz.h>
#include <evtimer_mzo.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define PUB_TOPIC "esp32/state"
#define PUB_INTERVAL 5
#define PUB_MSG_WORK "work"
#define PUB_MSG_NOT_WORK "not work"

static emcute_topic_t topic;
static sock_udp_ep_t remote;

void *mqttsn_thread(void *arg) {
    (void)arg;
    
    // Prepare the address for the MQTT-SN broker
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    // Initialize message queue
    msg_t queue[8];
    msg_init_queue(queue, 8);

    // Connect to the MQTT-SN broker
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return NULL;
    }

    // Register the topic
    topic.name = PUB_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    // Publish messages every five seconds
    while (1) {
        const char *state = (some_condition()) ? PUB_MSG_WORK : PUB_MSG_NOT_WORK;
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }

        // Sleep for PUB_INTERVAL seconds
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000);
    }
    return NULL;
}

int main(void) {
    char *stack = malloc(THREAD_STACKSIZE_DEFAULT);
    if (!stack) {
        puts("Error: unable to allocate stack memory");
        return 1;
    }

    thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 
                  THREAD_CREATE_SLEEPING, mqttsn_thread, NULL, CLIENT_ID);

    thread_yield();

    return 0;
}