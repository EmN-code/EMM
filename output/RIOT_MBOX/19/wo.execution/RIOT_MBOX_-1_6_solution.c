#include <thread.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 8
#define THREAD_PRIORITY_RECV (THREAD_PRIORITY_MAIN - 1)

static char send_stack[THREAD_STACKSIZE_DEFAULT];
static char receive_stack[THREAD_STACKSIZE_DEFAULT];
kernel_pid_t receive_pid;

void *send_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = (uint32_t)42;
    printf("Sending message with value: %lu\n", msg.content.value);
    msg_send(&msg, receive_pid);
    return NULL;
}

void *receive_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Received message with value: %lu\n", msg.content.value);
    return NULL;
}

int main(void) {
    receive_pid = thread_create(receive_stack, sizeof(receive_stack),
                                THREAD_PRIORITY_RECV, 0,
                                receive_thread, NULL, "receive thread");

    thread_create(send_stack, sizeof(send_stack),
                  THREAD_PRIORITY_MAIN, 0,
                  send_thread, NULL, "send thread");

    /* Let the main thread idle */
    while (1) {
        thread_yield();
    }
    return 0;
}