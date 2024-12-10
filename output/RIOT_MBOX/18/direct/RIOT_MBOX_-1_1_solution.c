#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include <xtimer.h>

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_INTERVAL     (1000000U) // Interval in microseconds
#define MSG_TYPE_CUSTOM     (0x1234)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;

    while (1) {
        msg.type = MSG_TYPE_CUSTOM;
        msg.content.value = counter;
        printf("[Sender] Sending message with value: %d\n", counter);
        msg_send(&msg, sched_active_thread_pid);

        ++counter;
        xtimer_usleep(SENDER_INTERVAL);
    }

    return NULL;
}

static void receiver(void) {
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("[Receiver] Error receiving message\n");
            continue;
        }

        if (msg.type == MSG_TYPE_CUSTOM) {
            printf("[Receiver] Received message with value: %d\n", msg.content.value);
        }
        else {
            printf("[Receiver] Received unknown message type\n");
        }
    }
}

int main(void) {
    puts("RIOT Message Queue Example on ESP32\n");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, sender, NULL, "sender");

    receiver();

    return 0;
}