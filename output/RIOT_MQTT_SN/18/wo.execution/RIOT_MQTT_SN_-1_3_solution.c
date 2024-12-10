#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "netinet/in.h"

#define EMCUTE_ID           1
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define TOPIC_NAME          "state"
#define MESSAGE_DELAY_S     5

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t topic;

void *mqtt_thread(void *arg) {
    (void) arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("MQTT-SN connection failed");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("MQTT-SN topic registration failed");
        return NULL;
    }

    while(1) {
        const char *msg = "work";
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("MQTT-SN publish failed");
        }
        ztimer_sleep(ZTIMER_SEC, MESSAGE_DELAY_S);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    printf("Starting MQTT-SN publisher\n");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt_thread");
    return 0;
}