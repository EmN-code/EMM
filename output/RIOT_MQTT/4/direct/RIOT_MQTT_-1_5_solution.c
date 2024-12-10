#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqtt.h>
#include <net/emcute.h>
#include <net/emcute_sock.h>
#include <net/gnrc.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC          "state"
#define MQTT_MSG_WORK       "work"
#define MQTT_MSG_NOT_WORK   "not work"
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define PUB_INTERVAL        (5U * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_pub_thread(void *arg) {
    emcute_sock_t sock;
    sock.data = NULL;
    sock.cb = NULL;

    sockaddr_in6_t gw;
    ipv6_addr_from_str((ipv6_addr_t *)&gw.sin6_addr, "::0");
    gw.sin6_family = AF_INET6;
    gw.sin6_port = SERVER_PORT;

    if (emcute_sock_init(&sock, &gw, EMCUTE_PRIO) != EMCUTE_OK) {
        printf("Error: unable to initialize socket\n");
        return NULL;
    }

    emcute_topic_t topic;
    topic.slice = MQTT_TOPIC;
    topic.slen  = strlen(MQTT_TOPIC);

    while (1) {
        const char *msg;
        // Simulate change of state
        static int state = 0;
        state = !state;
        msg = state ? MQTT_MSG_WORK : MQTT_MSG_NOT_WORK;

        if (emcute_sock_pub(&sock, &topic, msg, strlen(msg), EMCUTE_MSG_RETAIN) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }

        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("RIOT MQTT Client on ESP32\n");

    thread_create(stack, sizeof(stack), EMCUTE_PRIO,
                  THREAD_CREATE_STACKTEST, mqtt_pub_thread, NULL, "mqtt_pub");

    return 0;
}