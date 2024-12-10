#include <xtimer.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_SEND (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVE (THREAD_PRIORITY_MAIN - 2)

static char send_thread_stack[THREAD_STACKSIZE];
static char receive_thread_stack[THREAD_STACKSIZE];

static kernel_pid_t send_thread_pid;
static kernel_pid_t receive_thread_pid;

static msg_t msg_queue[8];

void *send_thread_handler(void *arg) {
    (void) arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;  // Example message content
    
    while(1) {
        msg_send(&msg, receive_thread_pid);
        printf("Message sent with value %d\n", (int)msg.content.value);
        xtimer_sleep(3);
    }
    return NULL;
}

void *receive_thread_handler(void *arg) {
    (void) arg;
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    msg_t msg;
    
    while(1) {
        msg_receive(&msg);
        printf("Message received with value %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    receive_thread_pid = thread_create(receive_thread_stack, sizeof(receive_thread_stack),
                                       THREAD_PRIORITY_RECEIVE, 0, receive_thread_handler, NULL, "recv thread");

    send_thread_pid = thread_create(send_thread_stack, sizeof(send_thread_stack),
                                    THREAD_PRIORITY_SEND, 0, send_thread_handler, NULL, "send thread");

    if(send_thread_pid <= KERNEL_PID_UNDEF || receive_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating threads\n");
        return 1;
    }

    printf("Threads successfully created\n");

    xtimer_sleep(5); // Allow threads to run

    return 0;
}