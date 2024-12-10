#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <assert.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <msg.h>
#include <esp_wifi.h>
#include <time_units.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MAIN_QUEUE_SIZE     (8)
#define MQTT_TOPIC_MAXLEN   (64)
#define PUBLISH_INTERVAL    (5U)

// Static memory for the stack of the thread
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static emcute_topic_t topic;

void *mqtt_thread(void *arg) {
    (void)arg;
    int res;
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

    // Convert the IP address from string format to a binary format
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        printf("error: unable to parse gateway address\n");
        return NULL;
    }

    // Start a connection to the MQTT-SN gateway
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("error: unable to connect to MQTT-SN gateway [%d]\n", res);
        return NULL;
    }

    // Register a topic to publish
    strncpy(topic.name, "work/status", MQTT_TOPIC_MAXLEN);
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("error: unable to register topic [%d]\n", res);
        return NULL;
    }

    while (1) {
        // Publish "work" every 5 seconds
        const char *state = "work";
        res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("error: unable to publish data [%d]\n", res);
        }
        // Wait 5 seconds before publishing again
        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }
}

int main(void) {
    /* initialize the message queue */
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    /* run the main scheduler loop */
    while (1) {
        // simulate the scheduler loop
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}