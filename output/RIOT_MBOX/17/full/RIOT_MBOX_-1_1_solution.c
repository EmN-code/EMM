#include <ztimer.h>  // Time related functions for event triggering
#include <xtimer.h>  // General purpose timer
#include <msg.h>  // Message passing
#include <stdio.h>  // Standard input/output functions

#define MSG_QUEUE_SIZE   (8)
#define INTERVAL         (XTIMER_MSEC * 1000)  // Interval of 1 second
#define EVENT_TYPE       (0x1234)  // Arbitrary event type ID

static msg_t msg_queue[MSG_QUEUE_SIZE];  // Message queue
static ztimer_t event_timer;  // Timer struct

static void event_callback(void *arg){
    (void)arg;
    msg_t msg;
    msg.type = EVENT_TYPE;  // Set the event type
    if (msg_send(&msg, thread_getpid()) < 0){
        printf("Error sending message\n");
    }
}

int main(void){
    printf("Starting message box example\n");
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);  // Initialize the message queue
    ztimer_set_msg(ZTIMER_MSEC, &event_timer, INTERVAL, thread_getpid(), EVENT_TYPE);  // Set the timer

    while (1){
        msg_t msg;
        (void)msg_receive(&msg);  // Wait for the message

        if (msg.type == EVENT_TYPE){
            printf("Received event message\n");
        } else {
            printf("Unknown message received\n");
        }
    }

    return 0;
}