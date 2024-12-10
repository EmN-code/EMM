#include <thread.h>
#include <fmt.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE           (8)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[MSG_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %ld\n", (long)msg.content.value);
    }

    return NULL;
}

int main(void) {
    puts("RIOT: Thread communication with msg_t example\n");

    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                  thread_handler, NULL, "receiver_thread");

    msg_t msg;
    for (int i = 0; i < 5; i++) {
        msg.content.value = (int32_t)i;
        printf("Sending message with content: %ld\n", (long)msg.content.value);
        msg_send(&msg, thread_getpid() + 1);
    }

    return 0;
}