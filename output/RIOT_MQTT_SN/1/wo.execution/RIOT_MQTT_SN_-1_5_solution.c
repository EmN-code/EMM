#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <mqttclient.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <mutex.h>
#include <paho_mqtt.h>

#define EMCUTE_ID           "ger_io_prime"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         1883
#define SERVER_ADDR         "47.102.103.1"
#define DEFAULT_QOS         1
#define TOPIC_NAME          "work/status"

static char stack[THREAD_STACKSIZE_MAIN];
static const char *status_msg = "work";

static int publish(emcute_topic_t *topic, const void *data, size_t len)
{
    unsigned flags = EMCUTE_QOS_0;
    return emcute_pub(topic, data, len, flags);
}

int main(void)
{
    /* Initialize the emcute run thread for MQTT-SN */
    memset(stack, 0, sizeof(stack));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST, 
                  emcute_run, NULL, "emcute");

    /* Set up the remote endpoint */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    /* Connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[ERROR] Unable to connect to the server!\n");
        return 1;
    }

    /* Register a topic on broker */
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[ERROR] Unable to register topic!\n");
        return 1;
    }

    /* Publish status on topic every 5 seconds */
    while (1) {
        printf("[INFO] Publishing %s\n", status_msg);
        if (publish(&topic, status_msg, strlen(status_msg)) != EMCUTE_OK) {
            printf("[ERROR] Unable to publish data\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}