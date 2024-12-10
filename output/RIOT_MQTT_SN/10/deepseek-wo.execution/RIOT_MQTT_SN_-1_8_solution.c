
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;

static void *pub_thread(void *arg)
{
    (void)arg;
    bool state = true;

    while (1) {
        const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
        state = !state;

        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", msg);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = HTONS(SERVER_PORT) };

    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "publisher");

    return 0;
}
