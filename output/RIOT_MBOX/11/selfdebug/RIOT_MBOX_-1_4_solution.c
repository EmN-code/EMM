#include <xtimer.h>
#include <stdio.h>
#include <msg.h>

#define MSG_QUEUE_SIZE     (8)
#define INTERVAL           (1000000U)  // 1 second intervals

static msg_t queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        printf("Sending message with value: %lu\n", (unsigned long)msg.content.value);
        msg_send(&msg, thread_getpid());  // Send to self

        msg.content.value++;
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, MSG_QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), 
                                            THREAD_PRIORITY_MAIN - 1, 0, sender_thread, 
                                            NULL, "sender");
    (void)sender_pid; // Remove this line if sender_pid is necessary elsewhere

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}

static char sender_stack[THREAD_STACKSIZE_DEFAULT];