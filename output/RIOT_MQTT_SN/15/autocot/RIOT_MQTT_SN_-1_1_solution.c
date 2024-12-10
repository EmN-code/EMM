#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <msg.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define INTERVAL 5 /* seconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[5];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    t.name = TOPIC_NAME;
    t.id = 0; // Let emcute choose an ID

    /* establish a connection to MQTT-SN broker */
    sockaddr_t gw = {
        .family = AF_INET,
        .addr.ipv4 = { .u32 = inet_addr(MQTT_BROKER_IP) },
        .port = MQTT_BROKER_PORT
    };

    if (emcute_con(&gw, true, "esp32") != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return NULL;
    }

    printf("Connected to broker [%s]:%i\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    /* publish state every INTERVAL seconds */
    while (1) {
        const char *state_msg = "work";
        if (emcute_pub(&t, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish to topic %s\n", TOPIC_NAME);
        }
        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}