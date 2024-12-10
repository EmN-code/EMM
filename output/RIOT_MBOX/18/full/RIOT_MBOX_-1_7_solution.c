#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <dht_params.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <evtimer.h>
#include <atomic.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <msg.h>
#include <ztimer.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Receiver thread started.\n");
    while (1) {
        // Simulate receiving a mailbox message
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_t msg;

    // Initialize the mailbox
    mbox_t mbox;
    msg_t queue[8];  // arbitrary queue size
    mbox_init(&mbox, queue, 8);

    // Create the receiver thread
    receiver_pid = thread_create(stack, sizeof(stack), PRIORITY, 0, receiver_thread, NULL, "receiver");

    printf("Main function started. Sending messages.\n");
    for (int i = 0; i < 10; i++) {
        // Simulate sending a message to the mailbox
        msg.content.value = i;
        msg_send_receive(&msg, NULL, receiver_pid);
        xtimer_sleep(1);
    }

    return 0;
}