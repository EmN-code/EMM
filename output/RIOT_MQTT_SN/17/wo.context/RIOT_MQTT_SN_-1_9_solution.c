#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/emcute.h>  // Correct header for emcute library

#define EMCUTE_ID           "riot"           // Unique Client ID
#define EMCUTE_PORT         1883              // Default MQTT-SN port
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)  // Priority of emcute thread

static emcute_sub_t subscriptions[EMCUTE_PORT]; // Array for subscriptions
static char stack[THREAD_STACKSIZE_DEFAULT];     // Stack for emcute thread

static emcute_topic_t topic;

/**
 * @brief   Publish a message
 *
 * This function publishes a message to a given topic with QoS level 0.
 */
static int publish(const char *topic_name, const char *data)
{
    unsigned flags = EMCUTE_QOS_0;
    topic.name = topic_name;
    topic.id = 0;  // The ID will be set by the broker

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("[MQTT-SN] Error: unable to register topic.");
        return 1;
    }

    if (emcute_pub(&topic, data, strlen(data), flags) != EMCUTE_OK) {
        puts("[MQTT-SN] Error: unable to publish data.");
        return 1;
    }

    puts("[MQTT-SN] Published data successfully.");
    return 0;
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    char *message = "working";
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    gw.netif = SOCK_ADDR_ANY_NETIF;

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) == 0) {
        puts("[MQTT-SN] Error: unable to parse gateway address.");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[MQTT-SN] Error: unable to connect to gateway.");
        return NULL;
    }

    puts("[MQTT-SN] Connected successfully.");
    
    while (1) {
        publish("state/topic", message);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    puts("[MQTT-SN] Starting example...");

    /* initialize emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}