#include <emcute.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <msg.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID   "riot_client"

static emcute_topic_t topic;
static char topic_name[] = "state_update";

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Published successfully!\n");
}

int main(void) {
    /* Emcute setup */
    emcute_sub_t subscriptions[1] = {{ .topic.name = topic_name, .cb = on_pub}};
    memset(subscriptions, 0, sizeof(subscriptions));
    emcute_run(SERVER_PORT, CLIENT_ID);
    
    /* Connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET, .addr.ipv4 = {SERVER_ADDR}, .port = SERVER_PORT };
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    /* Register topic */
    topic.name = topic_name;
    emcute_reg(&topic);

    /* Publish state every 5 seconds */
    char *msg = "work or not work";
    while (1) {
        emcute_pub(&topic, msg, strlen(msg), 0);
        xtimer_sleep(5);
    }
    return 0;
}