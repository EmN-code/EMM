#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define EMCUTE_ID           "esp32_mqttsn"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    // Callback when a message is published to a subscribed topic
}

int main(void)
{
    // Prepare the message queue for the thread
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Initialize the emcute
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);

    // Setup target-server UDP endpoint
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    if (inet_pton(AF_INET6, "47.102.103.1", &gw.addr) != 1) {
        printf("ERROR: Unable to parse IPv6 address\n");
        return 1;
    }

    // Connect to the specified Gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("ERROR: Unable to connect to [%s]:%i\n", "47.102.103.1", (int)gw.port);
        return 1;
    }
    puts("Successfully connected to gateway\n");

    // Register and publish to the topic periodically
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    char *topic_name = "esp32/state";

    t.name = topic_name;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("ERROR: Unable to register topic '%s'\n", topic_name);
        return 1;
    }

    while (1) {
        char message[] = "work or not work";
        if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
            printf("ERROR: Unable to publish data\n");
        }

        // Sleep 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}