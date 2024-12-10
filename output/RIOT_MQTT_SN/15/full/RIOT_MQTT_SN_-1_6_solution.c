#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "timex.h"
#include "stdatomic.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID "my_device_id"
#define PUBLISH_INTERVAL (5U)

enum {
    STATE_WORK,
    STATE_NOT_WORK
};

static sock_udp_ep_t gateway = { .family = AF_INET }
static emcute_topic_t topic;

static void *publish_thread(void *arg) {
    (void)arg;
    char *message;
    int state = STATE_WORK;

    while(1) {
        if (state == STATE_WORK) {
            message = "work";
        } else {
            message = "not work";
        }

        while (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("publish failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
        }

        printf("Published: %s\n", message);

        // Toggle state for next publication
        state = (state == STATE_WORK) ? STATE_NOT_WORK : STATE_WORK;

        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    mutex_t lock;
    mutex_init(&lock);

    char *mqtt_broker_ip = "47.102.103.1";
    inet_pton(AF_INET, mqtt_broker_ip, &gateway.addr.ipv4);
    gateway.port = EMCUTE_PORT;

    pthread_t pid;

    while (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Connection to broker failed, retrying...\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    printf("Connected to broker\n");

    topic.name = "device-state";

    while (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Registering topic failed, retrying...\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    printf("Registered topic: %s\n", topic.name);

    pthread_create(&pid, NULL, publish_thread, NULL);

    msg_t msg_queue[8];

    // Main loop to handle incoming messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);

        switch (msg.type) {
            case CIAA_GPIO_LOW:
                mutex_lock(&lock);
                state = STATE_NOT_WORK;
                mutex_unlock(&lock);
                break;
            case CIAA_GPIO_HIGH:
                mutex_lock(&lock);
                state = STATE_WORK;
                mutex_unlock(&lock);
                break;
            default:
                break;
        }
    }

    emcute_discon();

    return 0;
}