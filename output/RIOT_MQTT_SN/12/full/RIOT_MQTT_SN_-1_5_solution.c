#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "msg_box.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define EMCUTE_PORT (1883U)
#define MQTT_BROKER_IP "47.102.103.1"
#define PUB_TOPIC_NAME "state"
#define PUB_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t pub_topic;

void *publisher_thread(void *arg) {
    (void)arg;
    char *message_work = "work";
    size_t len = strlen(message_work);
    unsigned flags = EMCUTE_QOS_0;
    
    msg_t msg;
    msg_queue_create(queue, ARRAY_SIZE(queue));

    while (1) {
        // Publish the message 'work'
        int res = emcute_pub(&pub_topic, message_work, len, flags);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to publish data, error code: %d\n", res);
        } else {
            printf("Published: %s\n", message_work);
        }

        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000U);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    int res = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error: invalid IP address\n");
        return 1;
    }

    while (1) {
        res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to MQTT broker\n");
            break;
        } else {
            printf("Error: Unable to connect to the broker, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }

    pub_topic.name = PUB_TOPIC_NAME;
    res = emcute_reg(&pub_topic);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to register topic\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    while(1) {
        pause();
    }

    return 0;
}