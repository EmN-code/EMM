#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/mqtt.h>
#include <thread.h>

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define BUFFER_SIZE 128
#define QOS         1

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void* mqtt_thread(void *arg) {
    (void)arg;
    mqtt_client_t client;
    mqtt_connect(&client, BROKER_IP, BROKER_PORT, QOS);

    while (1) {
        const char *state = "work";
        mqtt_publish(&client, "state", state, QOS, 0);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}