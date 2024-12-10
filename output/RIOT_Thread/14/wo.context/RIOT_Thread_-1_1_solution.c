#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACK_SIZE  (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)

static char stack_low[THREAD_STACK_SIZE];
static char stack_high[THREAD_STACK_SIZE];

static msg_t msg_queue[8];

void *thread_low(void *arg) {
    (void)arg;
    msg_t msg;
    msg_recv(&msg);
    printf("Thread Low Priority received message with content: %d\n", (int)msg.content.value);
    return NULL;
}

void *thread_high(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content
    msg_send(&msg, thread_getpid_of((thread_t *)arg));
    printf("Thread High Priority sent message with content: %d\n", (int)msg.content.value);
    return NULL;
}

int main(void) {
    thread_t thread_low_pid, thread_high_pid;
    msg_init_queue(msg_queue, 8);
    
    thread_high_pid = thread_create(stack_high, sizeof(stack_high),
                                    THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                                    thread_high, NULL, "thread_high");
    
    thread_low_pid = thread_create(stack_low, sizeof(stack_low),
                                   THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                                   thread_low, &thread_high_pid, "thread_low");
    
    return 0;
}