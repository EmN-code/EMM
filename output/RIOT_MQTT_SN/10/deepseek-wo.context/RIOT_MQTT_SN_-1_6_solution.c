
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    char msg[20];
    while (1) {
        strcpy(msg, "work");
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_usleep(MSG_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN client");

    thread_create(emcute_stack, sizeof(emcute_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}
