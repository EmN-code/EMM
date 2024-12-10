#include <xtimer.h>
#include <msg.h>
#include <kernel_types.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];
static msg_t queue_high[8];
static msg_t queue_low[8];

void *high_priority_thread(void *arg) {
    msg_t msg;
    msg_receive(&msg);  // Receive the message
    printf("High priority thread received message: %d\n", msg.content.value);
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    msg.content.value = 42;  // Message content
    msg_send(&msg, thread_getpid_of(arg));
    printf("Low priority thread sent message\n");
    return NULL;
}

int main(void) {
    thread_create(stack_high, sizeof(stack_high), HIGH_PRIORITY,
                  THREAD_CREATE_STACKTEST, high_priority_thread,
                  thread_getpid(), "high_priority_thread");
    
    thread_create(stack_low, sizeof(stack_low), LOW_PRIORITY,
                  THREAD_CREATE_STACKTEST, low_priority_thread,
                  thread_getpid(), "low_priority_thread");
    
    return 0;
}