#include "thread.h"
#include "msg.h"
#include "fmt.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

char stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        // Send a message to the main thread
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    msg_t msg;
    while (1) {
        // Receive a message
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}