#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "mqtt.h"
#include "msg.h"
#include "net/emcute.h"
#include "xtimer.h"

#define SERVER_ADDR           "47.102.103.1"
#define SERVER_PORT           1883
#define BUF_SIZE              128
#define CLIENT_ID             "riot_client"
#define PUB_TOPIC             "state"
#define PUB_MSG_WORK          "work"
#define PUB_MSG_NOT_WORK      "not work"
#define INTERVAL              (5U * US_PER_SEC)
#define QOS                   0

static char stack[THREAD_STACKSIZE_DEFAULT];

void *publish_thread(void *arg) {
    (void)arg;

    mqtt_client_t client;
    mqtt_connect(&client, SERVER_ADDR, SERVER_PORT, QOS);

    while (1) {
        const char *msg = PUB_MSG_WORK;
        mqtt_publish(&client, PUB_TOPIC, msg, strlen(msg), QOS);

        xtimer_sleep(INTERVAL/US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    printf("RIOT MQTT Publisher Example\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, NULL, "publish_thread");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}