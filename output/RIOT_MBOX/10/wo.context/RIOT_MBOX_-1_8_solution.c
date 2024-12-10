#include <xtimer.h> // Include for timers
#include <random.h> // Include for random number generation
#include <msg.h> // Include for msg_t and mailbox
#include <stdio.h> // Include for input and output

#define MSG_QUEUE_SIZE   (8U)
#define INTERVAL_USEC    (1000000U) // 1 second

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t my_mbox;

void sender(void) {
    while (1) {
        // Random delay to simulate event timing
        uint32_t delay = random_uint32_range(500000, 1500000);
        xtimer_usleep(delay);

        // Prepare and send a message
        msg_t msg;
        mbox_put(&my_mbox, &msg, 0);

        printf("Message sent after %u us\n", (unsigned) delay);
    }
}

void receiver(void) {
    while (1) {
        msg_t msg;
        mbox_get(&my_mbox, &msg);
        printf("Message received\n");
    }
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&my_mbox, msg_queue, MSG_QUEUE_SIZE);

    // Start the sender and receiver functions
#ifdef MODULE_THREAD
    thread_create(..., sender, ...);
    thread_create(..., receiver, ...);
#else
    puts("Thread module not available.");
#endif

    // Main thread can do other tasks
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}