#include <stdio.h>  // Needed for printf
#include <thread.h> // RIOT Thread API
#include <msg.h>    // RIOT IPC messages API
#include <kernel_defines.h> // Kernel options

#define MAIN_QUEUE_SIZE (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

#define MSG_TYPE_DATA (0x01)

char stack_a[THREAD_STACKSIZE_MAIN];
char stack_b[THREAD_STACKSIZE_MAIN];

static void *thread_a(void *arg) {
    msg_t msg;
    (void)arg;
    
    for (int i = 0; i < 5; i++) {
        msg.type = MSG_TYPE_DATA;
        msg.content.value = i;
        printf("Thread A: Sending message with value %d\n", i);
        msg_send(&msg, thread_getpid());
    }
    
    return NULL;
}

static void *thread_b(void *arg) {
    msg_t msg;
    (void)arg;
    
    while (1) {
        if (msg_receive(&msg) >= 0) {
            if (msg.type == MSG_TYPE_DATA) {
                printf("Thread B: Received message with value %ld\n", msg.content.value);
            }
        }
    }

    return NULL;
}

int main(void) {
    puts("RIOT inter-thread communication example");
    
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(stack_a, sizeof(stack_a), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_a, NULL, "thread_a");
    thread_create(stack_b, sizeof(stack_b), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_b, NULL, "thread_b");

    return 0;
}