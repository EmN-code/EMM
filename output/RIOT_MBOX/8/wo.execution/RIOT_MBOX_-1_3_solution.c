#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>

#define INTERVAL 1000000 // 1 second interval in microseconds
#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void periodic_task(void)
{
    msg_t msg;
    msg.content.value = 42; // Example event content

    // Send the message to the mbox
    mbox_post(&mbox, &msg, 42);
}

int main(void)
{
    // Initialize message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);
    
    // Initialize the mbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Create a timer to call periodic_task every INTERVAL microseconds
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, periodic_task, NULL, INTERVAL);
    ztimer_periodic_start(&periodic_timer);

    puts("Message box example is running.");

    // Main loop (simulate doing other things while waiting for messages)
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %u\n", (unsigned)msg.content.value);
    }

    return 0;
}