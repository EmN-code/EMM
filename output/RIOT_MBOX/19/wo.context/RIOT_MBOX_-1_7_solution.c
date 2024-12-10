#include <thread.h>  // Provides the threading functionalities
#include <stdio.h>    // Standard input and output in C
#include <xtimer.h>   // For timer functionalities
#include <msg.h>      // For message passing functionalities

#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE    (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define QUEUE_SIZE    (8U)

static char message_stack[STACK_SIZE];
static msg_t msg_queue[QUEUE_SIZE];

static void *message_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, QUEUE_SIZE);

    msg_t msg;
    while (1) {
        /* Receiving messages */
        msg_receive(&msg);
        printf("Received message with type: %u and content: %p\n", msg.type, msg.content.ptr);

        /* Responding to the received message */
        msg_t reply;
        reply.type = msg.type + 1; // Modify the type slightly
        reply.content.ptr = msg.content.ptr; // Use the same content

        if (msg_reply(&msg, &reply) < 1) {
            printf("Failed to send reply.\n");
        }
    }
    return NULL;
}

int main(void) {
    /* Create the message-handling thread */
    kernel_pid_t pid = thread_create(message_stack, sizeof(message_stack),
                                     PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                     message_thread, NULL, "message_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error: failed to create thread\n");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();
    msg_t msg;

    while (1) {
        msg.type = 100; // Example message type
        msg.content.ptr = (void *)0xABCD; // Example pointer content
        msg_send(&msg, pid);
        printf("Sent message with type: %u and content: %p\n", msg.type, msg.content.ptr);

        /* Sleep for 2 seconds */
        xtimer_periodic_wakeup(&last_wakeup, 2 * US_PER_SEC);
    }
    return 0;
}