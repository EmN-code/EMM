#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PORT        (1883U)
#define BROKER_ADDRESS     "47.102.103.1"
#define CLIENT_ID          "esp32_client"
#define TOPIC_NAME         "device/state"
#define PUBLISH_INTERVAL   (5U * US_PER_SEC)  // Publish every 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static char broker_address[64];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("### Received publication for topic '%s' [%i]
", topic->name, (int)topic->id);
    printf("Data: %s
", in);
}

int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS);

    // connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    return 0;
}

int register_topic(void) {
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return 1;
    }
    return 0;
}

int main(void) {
    emcute_run(EMCUTE_PORT, CLIENT_ID);

    xtimer_sleep(1); // give some time to settle

    if (connect_broker() != 0) {
        return 1;
    }

    if (register_topic() != 0) {
        emcute_discon();
        return 1;
    }

    char message[] = "work";

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    emcute_discon();
    return 0;
}