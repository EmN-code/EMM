#include "net/emcute.h"
#include "net/nanocoap.h"
#include "msg.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT (1883)
#define INTERVAL 5

static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t _mqtt_pid;

void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    t.name = "state";
    t.id = 1;

    while (1) {
        emcute_pub(&t, "work or not work", strlen("work or not work"), EMCUTE_QOS_0);
        sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    _mqtt_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}