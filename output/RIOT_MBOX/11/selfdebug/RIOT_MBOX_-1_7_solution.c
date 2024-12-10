#include <thread.h>
#include <stdio.h>
#include <inttypes.h>
#include <xtimer.h>
#include <msg.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[KERNEL_CONF_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t message;
    message.type = 1;
    while (1) {
        printf("Sending message: %d\n", message.type);
        msg_send(&message, thread_get(SENDER_PRIO));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            SENDER_PRIO, 0, sender_thread,
                                            NULL, "sender");
    if (sender_pid > 0) {
        printf("Sender thread started successfully with PID: %" PRIkernel_pid_t "\n", sender_pid);
    } else {
        printf("Failed to start sender thread\n");
    }

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message of type: %d\n", msg.type);
    }
    return 0;
}