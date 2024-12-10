#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define EMCUTE_PORT          (1883U)
#define EMCUTE_ID           ("emcute-client")
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char stack[THREAD_STACKSIZE_MAIN];

static const char *MQTT_BROKER_IP = "47.102.103.1";
static const char *MQTT_TOPIC = "device/state";
static const char *WORK_STATE_MSG = "work";
static const char *NOT_WORK_STATE_MSG = "not work";

static int publish_state(emcute_topic_t *t, bool is_working) {
    const char *message = is_working ? WORK_STATE_MSG : NOT_WORK_STATE_MSG;
    return emcute_pub(t, message, strlen(message), EMCUTE_QOS_0);
}

static void *emcute_thread(void *arg) {
    (void)arg;
    static emcute_topic_t topic;
    topic.name = MQTT_TOPIC;

    while(1) {
        /* Connect to broker */
        sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
        inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("Connection failed, retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Connected to %s\n", MQTT_BROKER_IP);

        /* Register topic */
        if (emcute_reg(&topic) != EMCUTE_OK) {
            printf("Failed to register topic\n");
            return NULL;
        }

        /* Publish state periodically */
        while(1) {
            bool is_working = true; 
            if (publish_state(&topic, is_working) != EMCUTE_OK) {
                printf("Publication failed, reconnecting...\n");
                ztimer_sleep(ZTIMER_MSEC, 1000);
                break;
            }
            printf("State published: %s\n", is_working ? WORK_STATE_MSG : NOT_WORK_STATE_MSG);
            ztimer_sleep(ZTIMER_SEC, 5);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    puts("Starting emcute client...");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, 
                  emcute_thread, NULL, "emcute");

    return 0;
}