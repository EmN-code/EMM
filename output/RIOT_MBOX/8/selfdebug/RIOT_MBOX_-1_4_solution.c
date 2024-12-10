#include "xtimer.h"  // Timer functions
#include "msg.h"    // Message and message queue structures
#include "kernel.h"  // Core kernel functionalities

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static char timer_stack[THREAD_STACKSIZE_DEFAULT];

void *message_sender(void *arg) {
    (void)arg;  // Unused, preventing compilation warnings
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        xtimer_sleep(2);
        mbox_put(&mbox, &msg);
    }
    return NULL;
}

void *message_receiver(void *arg) {
    (void)arg;  // Unused, preventing compilation warnings
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received Message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    
    kernel_pid_t sender_pid = thread_create(timer_stack, sizeof(timer_stack),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          message_sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(timer_stack, sizeof(timer_stack),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          message_receiver, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}