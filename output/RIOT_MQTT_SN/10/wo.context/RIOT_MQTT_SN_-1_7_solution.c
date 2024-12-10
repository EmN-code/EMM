#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP  "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5U * 1000000U)  // Publish interval in microseconds

static char stack_buffer[THREAD_STACKSIZE_MAIN];

static emcute_topic_t topic;
static char pub_buffer[64];

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("ERROR: Unable to parse broker address");
        return 1;
    }
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: Unable to connect to gateway");
        return 1;
    }
    
    // Topic initialization
    topic.name = "state";
    topic.id = 0;
    
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: Unable to register topic");
        return 1;
    }
    return 0;
}

static void publish_state(void) {
    snprintf(pub_buffer, sizeof(pub_buffer), "work");
    if (emcute_pub(&topic, pub_buffer, strlen(pub_buffer), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("ERROR: Unable to publish state");
    }
}

int main(void) {
    emcute_run(SERVER_PORT, "riotnode");
    if (connect_to_broker() != 0) {
        puts("ERROR: Connect to broker failed");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(PUB_INTERVAL / 1000000U);
    }
    return 0;
}