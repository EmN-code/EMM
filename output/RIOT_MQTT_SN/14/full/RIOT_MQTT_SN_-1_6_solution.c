#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define MQTT_TOPIC_MAXLEN 64
#define SLEEP_TIME 5
#define EMCUTE_PORT (1883U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static int publish(const char *topic_name, const char *message) {
    unsigned flags = EMCUTE_QOS_0;
    topic.name = (char *)topic_name;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&topic, message, strlen(message), flags) != EMCUTE_OK) {
        puts("error: unable to publish message");
        return 1;
    }
    return 0;
}

static void reconnect_mqtt(emcute_topic_t topic, sock_udp_ep_t gw) {
    while (emcute_con(&gw, true, "", NULL, 0, 0) != EMCUTE_OK) {
        puts("reconnect attempt...");
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    puts("connected");
}

static void message_thread(void) {
    sock_udp_ep_t gw;
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    gw.port = EMCUTE_PORT;

    reconnect_mqtt(topic, gw);

    while (1) {
        if (publish("state", "work") != 0) {
            reconnect_mqtt(topic, gw);
        }
        ztimer_sleep(ZTIMER_SEC, SLEEP_TIME);
    }
}

int main(void) {
    puts("MQTT-SN Publication over EMCUTE Example");
    msg_init_queue(msg_q, ARRAY_SIZE(msg_q));

    if(thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                     THREAD_CREATE_STACKTEST, message_thread, NULL, "mqtt_thread") <= KERNEL_PID_UNDEF) {
        puts("error: unable to create thread");
        return 1;
    }
    return 0;
}