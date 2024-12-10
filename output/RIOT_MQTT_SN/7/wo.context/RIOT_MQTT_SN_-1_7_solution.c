#include <string.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include "msg.h"
#include "xtimer.h"
#include "shell.h"
#include "net/emcute.h"

#define EMCUTE_ID           "mqtt_client"
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define PUBLISH_INTERVAL    (5U * 1000000U)  // 5 seconds

static emcute_topic_t topic;
static char topic_name[] = "state";

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // Do nothing
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    // Initialize emcute
    emcute_run(SERVER_PORT, EMCUTE_ID);

    // Set destination
    sock_udp_ep_t gw = {.family = AF_INET, .port = SERVER_PORT};
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    // Connect to gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Cannot connect to broker\n");
        return 1;
    }
    puts("Connected to broker\n");

    // Now register and subscribe
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Failed to register topic\n");
        return 1;
    }

    // Start publishing loop
    while (1) {
        const char *data = "work";  // Assume work state
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish\n");
        } else {
            puts("Published: work\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return 0;
}