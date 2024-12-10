#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <msg.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <mma8x5x_regs.h>

#define INTERVAL (1U * US_PER_SEC)
#define QUEUE_SIZE (8)

// Define a message type
typedef struct msg {
    int type;
    int content;
} msg_t;

msg_t msg_queue[QUEUE_SIZE];

void sender_thread(void)
{
    msg_t message;
    message.type = 1;  // Example message type

    int counter = 0;
    while (1) {
        message.content = counter++;
        printf("Sending message %d\n", message.content);

        // Wait for the interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
        
        // Here, simulate sending a message to the queue
        // For example, could be: msg_send(&message, thread_pid); (in practical use)
    }
}

int main(void)
{
    /* Initialize the message queue to handle QUEUE_SIZE messages */
    msg_init_queue(msg_queue, QUEUE_SIZE);

    printf("[INFO] Message queue initialized.\n");

    /* Start sender thread to simulate periodic message sending */
    sender_thread();

    return 0;
}
