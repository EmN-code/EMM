#include "string.h"
#include "mma8x5x.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "xtimer.h"
#include "stdbool.h"
#include "time_units.h"
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "net/gcoap.h"
#include "timex.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "net/emcute.h"
#include "stdio.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "stddef.h"
#include "dht_params.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG "work"
#define INTERVAL (5U * US_PER_SEC)

static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

void *publisher(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    int res;
    
    strcpy(t.name, TOPIC_NAME);
    res = emcute_reg(&t);
    if (res != EMCUTE_OK) {
        printf("error: unable to register topic
");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&t, MSG, strlen(MSG), flags) != EMCUTE_OK) {
            puts("error: unable to publish data
");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher, NULL, "publisher");

    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));
    
    return 0;
}