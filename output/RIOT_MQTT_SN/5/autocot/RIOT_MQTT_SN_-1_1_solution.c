#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/gnrc/netif.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           ("esp32_device")
#define MSG_TYPE_MQTT_UP    (0x23)
#define SENSOR_TOPIC_NAME   ("state")
#define PUBLISH_INTERVAL    (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t state_topic;

void *emcute_thread(void *arg) {
    (void)arg;

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int mqtt_publish(emcute_topic_t *topic, const char *data) {
    emcute_msg_t msg;
    
    msg.data = (void *)data;
    msg.len = strlen(data);
    msg.type = EMCUTE_MSG_TYPE_PUBLISH;
    msg.flags = EMCUTE_MSG_FLAG_RETAIN;

    return emcute_pub(topic, &msg);
}

static void create_topic(void) {
    state_topic.name = SENSOR_TOPIC_NAME;
    emcute_reg(&state_topic);
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    create_topic();

    while (1) {
        mqtt_publish(&state_topic, "work");
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}