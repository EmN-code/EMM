#include <thread.h>
#include <xtimer.h>
#include <msg.h>
#include <stdio.h>

#define STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_HELPER (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define PRIORITY_HELPER  (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE   (8)

static char main_stack[STACKSIZE_MAIN];
static char helper_stack[STACKSIZE_HELPER];

msg_t main_msg_queue[MSG_QUEUE_SIZE];
msg_t helper_msg_queue[MSG_QUEUE_SIZE];

void *main_thread(void *arg) {
    (void)arg;
    msg_init_queue(main_msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;
    printf("Main thread: Waiting for messages...\n");
    while (1) {
        msg_receive(&msg);
        printf("Main thread: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

void *helper_thread(void *arg) {
    (void)arg;
    msg_init_queue(helper_msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;
    msg.content.value = 42;  // Arbitrary value
    while (1) {
        xtimer_sleep(2);
        printf("Helper thread: Sending message...\n");
        msg_send(&msg, thread_get(PID_MAIN));
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT message passing example...\n");
    thread_create(main_stack, sizeof(main_stack), PRIORITY_MAIN, 0,
                  main_thread, NULL, "main");
    thread_create(helper_stack, sizeof(helper_stack), PRIORITY_HELPER, 0,
                  helper_thread, NULL, "helper");
    return 0;
}