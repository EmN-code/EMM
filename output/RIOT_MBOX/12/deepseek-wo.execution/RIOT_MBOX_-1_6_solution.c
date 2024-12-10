
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        // Simulate sending events at intervals
        sleep(5);
        msg.content.value = rand();
        msg.type = 1;
        msg_send(&msg, thread_getpid());
    }
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Create a thread for sending messages
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        // Receive messages from the mailbox
        msg_receive(&msg);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }

    return 0;
}
