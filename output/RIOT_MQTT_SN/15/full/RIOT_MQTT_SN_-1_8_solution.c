#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "string.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "ipv6_addr.h"
#include "dht_params.h"
#include "dht.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define EMCUTE_PORT 1885
#define DEFAULT_MSG "work or not work"

static emcute_topic_t _topic;
static sock_udp_ep_t _remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };

void reconnect_network(void) {
    int res;
    while (1) {
        if ((res = emcute_con(&_remote, true, NULL, NULL, 0, 0)) == EMCUTE_OK) {
            printf("Connected to gateway at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
            break;
        }
        else {
            printf("Failed to connect, retrying...\n");
            xtimer_sleep(1);
        }
    }
}

void publisher(void) {
    int res;
    strcpy(_topic.name, "my_topic");
    _topic.id = 0;

    if ((res = emcute_reg(&_topic)) != EMCUTE_OK) {
        printf("Error: unable to register topic
");
        return;
    }

    while (1) {
        if ((res = emcute_pub(&_topic, DEFAULT_MSG, strlen(DEFAULT_MSG), EMCUTE_QOS_0)) != EMCUTE_OK) {
            printf("Failed to publish data
");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    reconnect_network();
    publisher();
    return 0;
}