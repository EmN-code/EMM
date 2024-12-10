#include "stdio.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U)  // publish every 5 seconds
#define EMCUTE_PORT 1883

static emcute_topic_t topic;

int emcute_con(const char *topic_name, const char *message) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    gw.port = EMCUTE_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    int rc;
    while (1) {
        if ((rc = emcute_con(&gw, true, topic_name, NULL, 0, 0)) != EMCUTE_OK) {
            printf("Connection to broker failed
");
            vtimer_usleep(PUB_INTERVAL * US_PER_SEC);
        } else {
            printf("Successfully connected to broker
");
            break;
        }
    }

    uint16_t t_id = emcute_reg(&topic, topic_name);
    if (t_id == 0) {
        printf("Error: Unable to obtain topic ID.
");
        return -1;
    }

    while (1) {
        rc = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (rc != EMCUTE_OK) {
            printf("Publish failed.
");
            vtimer_usleep(PUB_INTERVAL * US_PER_SEC);
        } else {
            printf("Publish success: %s -> %s
", message, topic_name);
            vtimer_usleep(PUB_INTERVAL * US_PER_SEC);
        }
    }

    return 0;
}

int main(void) {
    puts("RIOT MQTT-SN publisher started.");
    emcute_con("work_status", "work or not work");
    return 0;
}