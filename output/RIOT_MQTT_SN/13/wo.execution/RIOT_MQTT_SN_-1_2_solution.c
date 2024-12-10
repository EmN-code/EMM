#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <event/thread.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define UDP_MSG_SIZE        (128U)
#define MQTT_TOPIC_NAME     "work_status"
#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    (1883U)
#define PUB_INTERVAL_SEC    (5)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

void *publisher_thread(void *arg) {
    (void) arg;
    
    thread_sleep(PUB_INTERVAL_SEC);
    const char *data = "work";
    
    while (1) {
        emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0);
        thread_sleep(PUB_INTERVAL_SEC * 1000);
        data = (strcmp(data, "work") == 0) ? "not work" : "work";
    }

    return NULL;
}

int main(void) {
    printf("MQTT-SN Publisher starting\n");

    /* Initialize network stack */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_ADDR);
    gw.port = MQTT_BROKER_PORT;

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    strcpy(topic.name, MQTT_TOPIC_NAME);
    emcute_reg(&topic);

    /* Start MQTT-SN publisher thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publisher_thread, NULL, "publisher");

    while (1) {
        /* Let the main thread do nothing */
        thread_sleep(1);
    }

    return 0;
}