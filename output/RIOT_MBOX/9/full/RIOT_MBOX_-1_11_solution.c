#include "ztimer.h"
#include "ztimer_sleep.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"
#include "sys/socket.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define QUEUE_SIZE               (8U)
#define MQTT_BROKER_IP           "192.168.1.10"
#define MQTT_BROKER_PORT         (1883)

static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received message of type: %u\n", msg.type);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);
    mbox_init(&mbox, queue, QUEUE_SIZE);

    char *stack = (char*)malloc(THREAD_STACKSIZE_MAIN);
    if (stack == NULL) {
        printf("Failed to allocate memory for thread stack\n");
        return -1;
    }

    kernel_pid_t thread_pid = thread_create(stack, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN,
                                            0, thread_handler, NULL, "receiver_thread");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        free(stack);
        return -1;
    }

    Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second.\n");
            sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    evtimer_init_msg(&evtimer);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        evtimer_set(&evtimer, 5 * ZTIMER_SEC);
        evtimer_wait_period(&evtimer);

        msg_t msg;
        msg.type = 1;  // Example event type

        if (!mbox_try_send(&mbox, &msg)) {
            printf("Failed to send message\n");
        }
    }

    return 0;
}