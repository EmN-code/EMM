#include "ztimer.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "msg.h"
#include "mutex.h"
#include "mq.h"
#include "mutex.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "pthread.h"
#include "mqtt-client.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "net/gcoap.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         1883
#define EMCUTE_TOPIC_MAXLEN 64
#define EMCUTE_BUFSIZE      128

static emcute_topic_t _topic;
static char _topic_name[EMCUTE_TOPIC_MAXLEN] = "work_state";

/* add socket address of the MQTT broker */
sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *_mqtt_thread(void *arg) {
    (void)arg;
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    int res;
    
    /* Attempt to convert/generate the broker's address */
    while (1) {
        res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
        if (res != 1) {
            printf("Failed to parse IPv4 address\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        break;
    }

    /* Connect to the broker */
    while (1) {
        res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to connect to broker\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Connected to broker\n");
        break;
    }

    /* Register the topic */
    _topic.name = _topic_name;
    while (1) {
        res = emcute_reg(&_topic);
        if (res != EMCUTE_OK) {
            printf("Failed to register topic\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Registered topic\n");
        break;
    }

    while (1) {
        const char* payload = "work";

        /* Publish data */
        while (1) {
            res = emcute_pub(&_topic, payload, strlen(payload), 0);
            if (res != EMCUTE_OK) {
                printf("Failed to publish data\n");
                ztimer_sleep(ZTIMER_MSEC, 1000);
                continue;
            }
            printf("Published: %s\n", payload);
            break;
        }

        /* Toggle the work state message */
        if (strcmp(payload, "work") == 0) {
            payload = "not work";
        } else {
            payload = "work";
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    /* Create the thread for the MQTT publishing loop */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, _mqtt_thread, NULL, "mqtt_pub_thread");
    /* The main thread idles here */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}