#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_TOPIC "state"
#define PUB_DATA_WORK "work"
#define PUB_DATA_NOT_WORK "not work"
#define PUB_INTERVAL (5 * US_PER_SEC) // 5 seconds interval

static char stack[THREAD_STACKSIZE_MAIN];

static void *pub_thread(void *arg) {
    (void)arg;

    emcute_topic_t topic;
    topic.name = PUB_TOPIC;

    emcute_con_t mqtt_con = {
        .port = MQTT_BROKER_PORT,
    };

    memset(&mqtt_con.address.ipv4, 0, sizeof(mqtt_con.address.ipv4));
    inet_pton(AF_INET, MQTT_BROKER_IP, &mqtt_con.address.ipv4);

    if (emcute_con(&mqtt_con) != EMCUTE_OK) {
        printf("MQTT-SN connection failed\n");
        return NULL;
    }

    while (1) {
        const char *state_pub = PUB_DATA_WORK;

        int result = emcute_pub(&topic, state_pub, strlen(state_pub), EMCUTE_QOS_0);
        if (result != EMCUTE_OK) {
            printf("Error while publishing, return code: %d\n", result);
        } else {
            printf("Published: %s\n", state_pub);
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    printf("MQTT-SN publisher starting\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "pub_thread");
    return 0;
}