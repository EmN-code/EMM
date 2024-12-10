#include "ztimer.h"
#include "xtimer.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "thread.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "mutex.h"
#include "dht.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "timex.h"
#include "unistd.h"
#include "time_units.h"
#include "arpa/inet.h"

#define EMCUTE_PORT         (1883U)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (128U)
#define PUB_INTERVAL        (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[NUMOFSUBS];

static emcute_sub_t subscription[NUMOFSUBS];

void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static int connect_to_broker(sock_udp_ep_t *gw) {
    int res;
    while (1) {
        res = emcute_con(gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to broker\n");
            break;
        } else {
            printf("Connection attempt failed, retrying...\n");
            usleep(1000000); // Sleep for 1 second before retrying
        }
    }
    return res;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr);

    emcute_run(THREAD_STACKSIZE_DEFAULT, "emcute" , NULL);

    if (connect_to_broker(&gw) != EMCUTE_OK) {
        fprintf(stderr, "Failed to connect to broker\n");
        return 1;
    }

    emcute_topic_t t;
    strcpy(t.name, "state");
    emcute_reg(&t);

    char *publish_data = "work"; // The state data to be published

    while (1) {
        if (emcute_pub(&t, publish_data, strlen(publish_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publication of message failed\n");
        }
        printf("Published: %s\n", publish_data);
        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_discon();
    return 0;
}