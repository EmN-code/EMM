#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "arpa/inet.h"

#define MSG_QUEUE_SIZE (8U)
#define EMCUTE_ID      ('z')
#define EMCUTE_PRIO    (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT    (1883U)
#define MQTT_TOPIC     "state"
#define MQTT_DATA      "work"
#define BROKER_IP      "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[MSG_QUEUE_SIZE];
static emcute_topic_t my_topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    msg_init_queue(queue, MSG_QUEUE_SIZE);

    /* Setup emcute */
    while (emcute_setup(EMCUTE_PORT, EMCUTE_ID) != EMCUTE_OK) {
        printf("emcute setup failed\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    printf("emcute setup successful\n");

    /* Connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("failed to connect to broker %s\n", BROKER_IP);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    /* Register topic */
    strncpy(my_topic.name, MQTT_TOPIC, sizeof(my_topic.name));
    while (emcute_reg(&my_topic) != EMCUTE_OK) {
        printf("failed to register topic\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    /* Publish data every 5 seconds */
    while (1) {
        int res = emcute_pub(&my_topic, MQTT_DATA, strlen(MQTT_DATA), 0);
        if (res != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        else {
            printf("published data\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}