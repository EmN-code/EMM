#include <thread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUB_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static char stack_pub[THREAD_STACKSIZE_DEFAULT];

static bool work_state = true;

void publisher(void) {
    while (1) {
        char message[20];
        snprintf(message, sizeof(message), "{\"state\":\"%s\"}", work_state ? "work" : "not work");

        printf("Publishing: %s\n", message);
        // Here you would normally call a real publish function
        // publishing(message, MQTT_TOPIC);

        xtimer_sleep(PUB_INTERVAL);
        work_state = !work_state;
    }
}

void *main_thread(void *arg) {
    emcute_run(MQTT_BROKER_PORT, MQTT_BROKER_IP);

    thread_create(stack_pub, sizeof(stack_pub), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, main_thread, NULL, "main_thread");

    return 0;
}