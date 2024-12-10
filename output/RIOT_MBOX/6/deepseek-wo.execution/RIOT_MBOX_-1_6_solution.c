
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <time_units.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <mbox.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1); // Sleep for 1 second
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_t *sender = thread_create(NULL, 128, THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }

    return 0;
}
