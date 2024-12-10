
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void _publish_state(void)
{
    static int state = 0;
    const char *states[] = {"work", "not work"};

    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0; // Topic ID will be assigned by the broker

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    const char *msg = states[state];
    res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    } else {
        state = !state; // Toggle state
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    printf("Connecting to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    printf("Starting EMCUTE thread\n");
    thread_create(emcute_stack, sizeof(emcute_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_run, &gw, "emcute");

    while (1) {
        _publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
