#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "msg.h"

#define THREAD_PRIORITY_HIGH   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW    (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE       (THREAD_STACKSIZE_MAIN)

static char stack_high[THREAD_STACKSIZE];
static char stack_low[THREAD_STACKSIZE];
static msg_t msg_queue[8];

static kernel_pid_t peer_high;

void *thread_high(void *arg) {
    msg_t msg, reply;
    (void)arg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %lu on high priority thread\n", msg.content.value);
        reply.content.value = msg.content.value + 1; // Use reply
        msg_reply(&msg, &reply); // Send a reply
    }
    
    return NULL;
}

void *thread_low(void *arg) {
    msg_t msg, reply;
    (void)arg;

    msg_receive(&msg);
    printf("%s: Received message with content: %lu\n", "Low priority thread", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    peer_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH, 0, thread_high, NULL, "high");
    kernel_pid_t peer_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW, 0, thread_low, NULL, "low");

    msg_t m;
    m.content.value = 42;

    printf("Sending message with content: %lu\n", m.content.value);
    msg_send(&m, peer_high);
    
    return 0;
}