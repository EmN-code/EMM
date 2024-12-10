#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtime.h"
#include "ztimer.h"
#include "thread.h"
#include <stdio.h>

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         1883
#define EMCUTE_TOPIC_NAME   "state"
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         1883

static emcute_topic_t topic;

static void *_publisher_thread(void *arg) {
    (void)arg; 

    char *message_work = "work";
    char *message_not_work = "not work";
    bool state = true;

    while (1) {
        const char *message_content = state ? message_work : message_not_work;
        printf("Publishing: %s\n", message_content);
        int res = emcute_pub(&topic, message_content, strlen(message_content), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to publish data\n");
        }
        state = !state;
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    emcute_sock_t sock;
    sock_udp_ep_t gw = {.family = AF_INET6, .port = BROKER_PORT};

    if (inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6) != 1) {
        puts("Error parsing broker IP");
        return 1;
    }

    emcute_init(&sock);

    if (emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error connecting to broker");
        return 1;
    }

    topic.name = EMCUTE_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, _publisher_thread, NULL, "publisher");

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    return 0;
}