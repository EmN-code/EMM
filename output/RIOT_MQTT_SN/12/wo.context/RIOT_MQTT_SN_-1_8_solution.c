#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 64
#define PUB_INTERVAL (5U) // Publish interval in seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6);   
    
    printf("Connecting to MQTT-SN broker on %s:%d...\n", SERVER_ADDR, SERVER_PORT);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    topic.name = "device/status";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        emcute_discon();
        return 1;
    }

    while (1) {
        const char *state = "work";  // replace with "not work" as needed
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }
        else {
            printf("Published: %s\n", state);
        }

        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    emcute_discon();
    return 0;
}