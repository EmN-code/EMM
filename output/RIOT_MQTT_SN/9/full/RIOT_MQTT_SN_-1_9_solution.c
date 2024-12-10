#include <xtimer.h>
#include <thread.h>
#include <string.h>
#include <stdio.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           "esp32_mqttsn"
#define CONFIG_EMCUTE_BUFSIZE   (128U)
#define CONFIG_EMCUTE_TOPIC_MAXLEN (64U)
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

enum { STATUS_WORKING, STATUS_NOT_WORKING };
static const char *states[] = { "work", "not work" };

static char mqtt_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: cannot parse provided IP address");
        return NULL;
    }

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("Connected to gateway");
            break;
        } else {
            puts("Error: unable to connect to gateway");
            xtimer_sleep(1);
        }
    }

    topic.name = "device/status";

    while (1) {
        static int status = STATUS_WORKING;
        status = !status;

        if (emcute_pub(&topic, states[status], strlen(states[status]), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    thread_create(mqtt_stack, sizeof(mqtt_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}
