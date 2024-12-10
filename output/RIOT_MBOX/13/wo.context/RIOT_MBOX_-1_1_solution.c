#include <thread.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_INTERVAL     (1000000U)  // Interval in us (i.e. 1 second)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    
    while (1) {
        msg.content.value++;
        msg_send(&msg, thread_getpid());
        puts("Sent a message.");
        xtimer_usleep(SENDER_INTERVAL);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received a message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    thread_create(
        thread_getpid(),
        THREAD_STACKSIZE_MAIN,
        PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    thread_create(
        thread_getpid(),
        THREAD_STACKSIZE_MAIN,
        PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    return 0;
}