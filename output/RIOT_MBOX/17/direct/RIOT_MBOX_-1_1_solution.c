#include <thread.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE (8)
#define INTERVAL_US (1000000) // 1 second

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[MSG_QUEUE_SIZE];
static kernel_pid_t sender_thread_pid;

// This function sends messages every INTERVAL_US microseconds
void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        xtimer_usleep(INTERVAL_US);
        msg_send(&msg, sender_thread_pid);
        puts("Message sent!");
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(queue, MSG_QUEUE_SIZE);
    sender_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, sender_thread,
                                      NULL, "sender_thread");
    
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}