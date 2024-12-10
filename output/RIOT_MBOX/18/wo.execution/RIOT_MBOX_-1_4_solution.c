#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "unistd.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "msg.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "time_units.h"
#include "thread.h"

#define MSG_QUEUE_SIZE 8  // Size of the message queue
#define SENDER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)  // Priority of the sender thread

// Declare the message queue
static msg_t msg_queue[MSG_QUEUE_SIZE];

// Message content type
typedef struct {
    int event_id;
    char data[32];
} my_event_t;

// Sender thread function
void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    my_event_t event;
    int event_id = 0;

    while (1) {
        event.event_id = event_id++;
        snprintf(event.data, sizeof(event.data), "Event %d", event.event_id);

        msg.type = 0;
        msg.content.ptr = &event;

        // Send the message
        if (msg_send(&msg, thread_getpid()) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Sent: %s\n", event.data);
        }

        // Wait 1 second
        xtimer_sleep(1);
    }

    return NULL;
}

// Main function
int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the sender thread
    char sender_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIO, 0,
                  sender_thread, NULL, "sender_thread");

    msg_t msg;

    // Receive messages
    while (1) {
        msg_receive(&msg);
        my_event_t *event = (my_event_t *)msg.content.ptr;
        printf("Received: Event ID = %d, Data = %s\n", event->event_id, event->data);
    }

    return 0;
}