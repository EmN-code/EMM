#include <msg.h>
#include <kernel.h>

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void) {
    /* Initialize the message queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    msg_t msg;

    /* Loop to send messages */
    while(1) {
        /* Prepare a message */
        msg.content.value = 42; // Example value
        msg_send_receive(&msg, &msg_received, KERNEL_PID_ANY);
        printf("Message sent: %d\n", msg.content.value);

        /* Wait for a message (this will block) */
        msg_receive(&msg);
        printf("Message received: %d\n", msg.content.value);

        /* Sleep for a specified interval */
        xtimer_sleep(2);
    }

    return 0;
}