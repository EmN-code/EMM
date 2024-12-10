#include <msg.h>
#include <kernel_types.h>
#include <kernel.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static msg_t msg;

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    printf("Message Queue Example\n");

    while (1) {
        // Wait for a message (blocking call)
        msg_receive(&msg);
        // Print the received message type and content
        printf("Received message: %d\n", msg.type);
    }

    return 0;
}