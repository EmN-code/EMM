#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "stdio.h"

#define MSG_QUEUE_SIZE (8)
#define EVENT_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static msg_t event_queue[EVENT_QUEUE_SIZE];
static mbox_t event_mbox;

void *event_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Process the received event message.
        printf("Received event: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize mailbox
    mbox_init(&event_mbox, event_queue, EVENT_QUEUE_SIZE);

    // Create event thread
    char thread_stack[KERNEL_CONF_STACKSIZE_MAIN];
    kernel_pid_t event_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, 
                                           event_thread, NULL, "event");

    // Periodically send messages
    msg_t msg;
    msg.content.value = 0;
    while (1) {
        xtimer_sleep(2);
        msg.content.value++;
        mbox_try_put(&event_mbox, &msg);
        printf("Sent event: %d\n", msg.content.value);
    }

    return 0;
}