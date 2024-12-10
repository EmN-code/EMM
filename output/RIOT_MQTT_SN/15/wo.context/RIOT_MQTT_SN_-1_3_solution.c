#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC   "device/state"
#define INTERVAL    (5U * US_PER_SEC)

static emcute_topic_t topic;

int main(void) {
    int res;
    emcute_sub_t sub;
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    res = inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if(res <= 0) {
        puts("ERROR: inet_pton");
        return 1;
    }

    while (1) {
        res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if(res != EMCUTE_OK) {
            printf("ERROR: unable to connect to MQTT-SN broker: %d\n", res);
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        else {
            printf("Successfully connected to broker\n");
            break;
        }
    }
    
    topic.name = PUB_TOPIC;
    res = emcute_reg(&topic);
    if(res != EMCUTE_OK) {
        printf("ERROR: unable to register topic: %d\n", res);
        return 1;
    }

    while (1) {
        const char *state = "work";
        res = emcute_pub(&topic, state, strlen(state), 0);
        if(res != EMCUTE_OK) {
            printf("ERROR: unable to publish data: %d\n", res);
        }
        else {
            printf("Published state: %s\n", state);
        }
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}