#include <xtimer.h>
#include <net/sock/udp.h>
#include <string.h>

#include "emcute.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds.
#define MQTT_TOPIC "state"
#define MQTT_MSG "work"

static emcute_topic_t topic;
static char stack[THREAD_STACK_SIZE_MAIN];

static int publish_state(void) {
    int res;
    topic.name = MQTT_TOPIC;
    topic.id = 0;  // Updated to ensure topic reg assigns id

    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (%d)\n", res);
        return res;
    }

    if ((res = emcute_pub(&topic, MQTT_MSG, strlen(MQTT_MSG), EMCUTE_QOS_0)) != EMCUTE_OK) {
        printf("Error: unable to publish message (%d)\n", res);
    }

    return res;
}

static void *thread_handler(void *arg) {
    (void)arg;
    while(1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    if(emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%u\n", SERVER_ADDR, SERVER_PORT);
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  thread_handler, NULL, "pub_thread");

    emcute_run(SERVER_PORT, "esp32_mqtt_sn");

    return 0;
}