#include "xtimer.h"
#include "stdlib.h"
#include "net/emcute.h"

#define EMCUTE_ID       "mqtt_client"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

static emcute_sub_t subscriptions[1];
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *publish_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    t.name = "devices/esp32/state";
    emcute_reg(&t);

    while (1) {
        char *msg = "work";
        printf("Publishing: %s\n", msg);
        emcute_pub(&t, msg, strlen(msg), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    memset(subscriptions, 0, (sizeof(emcute_sub_t) * 1));

    msg_init_queue(queue, 8);
    emcute_run(CONFIG_GCOAP_PORT, EMCUTE_ID);
    
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_thread, NULL, "publish_thread");
    
    return 0;
}
