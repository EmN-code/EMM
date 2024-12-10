
#include <stdio.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting broker IP");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    puts("Topic registered");

    char msg_buf[32];
    while (1) {
        snprintf(msg_buf, sizeof(msg_buf), "work");
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published: work");
        }

        xtimer_usleep(MSG_INTERVAL);

        snprintf(msg_buf, sizeof(msg_buf), "not work");
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published: not work");
        }

        xtimer_usleep(MSG_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
