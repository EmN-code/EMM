#include <ztimer.h>
#include <msg.h>
#include <net/emcute.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

int main(void) {
    // Initialize the message queue
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Initialize emcute
    emcute_run(stack, sizeof(stack), EMCUTE_PRIO, "esp32");

    // Address and configuration settings for the MQTT-SN broker
    sock_udp_ep_t gateway = { .family = AF_INET, .port = 1883 };  
    inet_pton(AF_INET, "47.102.103.1", &gateway.addr.ipv4);

    // Connect to the gateway
    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return 1;
    }

    // Allocate a topic and publish something
    emcute_topic_t t;
    t.name = "sensor_data";
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        const char *data = "work";
        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }

        // Wait 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect (though we'll never reach here)
    emcute_discon();

    return 0;
}