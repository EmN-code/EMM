
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return NULL;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }
    printf("Registered topic: %s\n", topic.name);

    // Publish state every 5 seconds
    char msg[20];
    while (1) {
        strcpy(msg, "work");
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", msg);
        }

        xtimer_usleep(MSG_INTERVAL);

        strcpy(msg, "not work");
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", msg);
        }

        xtimer_usleep(MSG_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Start the EMCUTE thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
