#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "msg.h"
#include "mmbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "esp_system.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "unistd.h"

#define EMCUTE_PORT     (1883U) /* Port of the MQTT-SN broker */
#define EMCUTE_ID       ("RIOT-Client")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME      ("device/state")

static char mqtt_msg[] = "work";  /* Message to be published */
static emcute_topic_t topic;
static sock_udp_ep_t gw;

void on_received(const emcute_topic_t *topic, const void *data, size_t len) {
    printf("### received publication for topic '%s' [%i] : %.*s\n",
           topic->name, topic->id, (int) len, (char *)data);
}

static int publish(void) {
    unsigned flags = EMCUTE_QOS_0;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&topic, mqtt_msg, sizeof(mqtt_msg), flags) != EMCUTE_OK) {
        puts("error: unable to publish");
        return 1;
    }
    puts("Published message\n");
    return 0;
}

static int connect(void) {
    while (1) {
        int rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            printf("Error connecting to the broker, rc=%d. Retrying...\n", rc);
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("Successfully connected to the broker\n");
            break;
        }
    }
    return 0;
}

int main(void) {
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.port = EMCUTE_PORT;
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    memset(&topic, 0, sizeof(emcute_topic_t));
    strcpy(topic.name, TOPIC_NAME);

    /* Connect to the MQTT-SN broker */
    if (connect() != 0) {
        puts("Failed to connect\n");
        return 1;
    }

    /* Publish message every 5 seconds */
    while (1) {
        publish();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}