
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        // Simulate sending events at intervals
        msg.content.value = rand();
        msg.type = 1;
        mbox_put(&mbox, &msg);
        usleep(INTERVAL_MS * 1000);
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create sender and receiver threads
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender_thread, "sender");
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, receiver_thread, "receiver");

    // Main thread can do other tasks or just sleep
    while (1) {
        usleep(1000000);
    }

    return 0;
}
