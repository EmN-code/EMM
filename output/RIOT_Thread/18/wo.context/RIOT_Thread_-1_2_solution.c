#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_DEFAULT    1024
#define THREAD_PRIORITY_MAIN        7
#define NUM_MSGS                    16

typedef struct {
    char stack[THREAD_STACKSIZE_DEFAULT];
    msg_t msg_queue[NUM_MSGS];
} thread_params_t;

void *thread_function(void *arg) {
    thread_params_t *params = (thread_params_t *)arg;
    msg_t msg;
    msg_init_queue(params->msg_queue, NUM_MSGS);

    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received message: %ld\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    printf("Sender and Receiver Threads example\n");

    thread_params_t sender_params;
    thread_t sender_thread;

    sender_params.stack[0] = 'S';  // Dummy initialization to prevent warnings
    msg_init_queue(sender_params.msg_queue, NUM_MSGS);
    
    thread_create(sender_params.stack, sizeof(sender_params.stack),
                  THREAD_PRIORITY_MAIN + 1,
                  0, thread_function, &sender_params, "sender");
    
    while (1) {
        msg_t msg;
        msg.content.value = 42;
        msg_send(&msg, thread_getpid());
        printf("Sent message: %ld\n", msg.content.value);
        xtimer_sleep(1);
    }
    return 0;
}
