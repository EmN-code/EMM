#include <stdio.h>
#include <msg.h>
#include <kernel.h>
#include <evtimer.h>

static evtimer_t my_eventtimer;
static msg_t my_msg_queue[8]; // Define a queue of 8 messages

void event_handler(evtimer_event_t *event)
{
    msg_t msg;
    msg_send(&msg, event->msg.sender_pid);
    puts("Event sent!");
}

int main(void)
{
    // Initialize the event timer using a message box handler
    evtimer_init_msg(&my_eventtimer);

    // Set up message queue
    msg_init_queue(my_msg_queue, 8);

    // Create a msg_event for the evtimer
    static evtimer_msg_event_t my_msg_event;
    my_msg_event.msg.type = 0x01; // Arbitrary type identifier

    // Add the msg_event to the evtimer
    evtimer_add_msg(&my_eventtimer, &my_msg_event, KERNEL_PID_MAIN);

    puts("Message box event timer example
");

    while (1) {
        // Wait for messages and process them
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }

    return 0;
}