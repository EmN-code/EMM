#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define MSG_QUEUE_SIZE     (8U)
#define THREAD_STACKSIZE   (THREAD_STACKSIZE_DEFAULT)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char thread_stack[THREAD_STACKSIZE];

void *message_sender(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;

    while (1) {
        msg.content.value = counter++;
        puts("Sending message...");
        msg_send(&msg, thread_getpid());
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    puts("Message queue initialized.");

    kernel_pid_t pid = thread_create(
        thread_stack,
        sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        message_sender,
        NULL,
        "message_sender"
    );

    if (pid <= KERNEL_PID_UNDEF) {
        puts("Thread creation failed");
        return 1;
    }

    puts("Message sender thread created.");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}