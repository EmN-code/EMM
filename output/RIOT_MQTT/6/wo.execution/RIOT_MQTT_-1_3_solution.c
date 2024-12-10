#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5U * 1000U * 1000U) // in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *thread_entry(void *arg) {
    (void)arg;
    static char state_msg[] = "work";
    while (1) {
        // Code to connect to MQTT and publish data
        printf("Publishing: %s\n", state_msg);
        // Simulate successful MQTT publish
        if (rand() % 2) {
            strcpy(state_msg, "work");
        }
        else {
            strcpy(state_msg, "not work");
        }

        // Publish message to MQTT broker
        MQTTClient_publishMessage(MQTT_TOPIC, &state_msg, strlen(state_msg), QOS1);

        // Sleep for the defined interval
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_entry, NULL, "mqtt_pub_thread");

    // Wait forever, the thread will handle everything
    while (1) {
        msg_t m;
        msg_receive(&m);
    }
    return 0;
}