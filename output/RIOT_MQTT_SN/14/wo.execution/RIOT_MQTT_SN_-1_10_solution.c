#include "mutex.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT      (1883U)
#define EMCUTE_ID        ("riot-esp32")
#define SERVER_ADDR      "47.102.103.1"

static sock_udp_ep_t gw = {
    .family = AF_INET,
    .port = EMCUTE_PORT
};

static emcute_topic_t topic;
static char *topic_name = "device/status";

void publish_state(void) {
    const char *message = "work";
    unsigned flags = EMCUTE_QOS_0;

    int res = emcute_pub(&topic, message, strlen(message), flags);
    if (res != EMCUTE_OK) {
        printf("[error] unable to publish data: %d\n", res);
    }
}

void *mqtt_thread(void *arg) {
    (void) arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    /* This should never be reached */
    return NULL;
}

int main(void) {
    msg_t queue[QUEUE_SIZE];
    msg_init_queue(queue, QUEUE_SIZE);

    /* Set the gateway address */
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    /* Connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[error] unable to connect to gateway\n");
        return 1;
    }

    /* Setup and register the topic */
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = topic_name;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[error] unable to obtain topic ID\n");
        return 1;
    }

    /* Start the emcute thread */
    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, mqtt_thread, NULL,
                      "mqtt") <= KERNEL_PID_UNDEF) {
        printf("[error] unable to start thread\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}