#include <net/sock/udp.h>
#include <emcute.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     1883
#define CLIENT_ID       "esp32_client"
#define STATE_TOPIC     "device/state"
#define STATE_MSG_WORK  "work"
#define STATE_MSG_IDLE  "not work"
#define PUBLISH_INTERVAL 5

static emcute_topic_t state_topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    puts("Publish acknowledged");
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    gw.addr = IPV6_ANY; // Assumed IPv6; address setting should be adjusted.
    
    if (emcute_con(&gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    // Register the topic
    state_topic.name = STATE_TOPIC;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return 1;
    }

    // Loop to publish the state
    while (1) {
        const char *msg = STATE_MSG_WORK;
        if (emcute_pub(&state_topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        // Sleep for the defined interval
        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}