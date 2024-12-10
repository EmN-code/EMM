#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t main_pid = (kernel_pid_t)(uintptr_t)arg;

    while (1) {
        msg_receive(&msg);
        printf("Received message type: %d\n", msg.type);
        msg_reply(&msg, NULL);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t second_pid;
    kernel_pid_t main_pid = thread_getpid();

    second_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, (void *)(uintptr_t)main_pid, "secondary");

    msg_t msg;
    msg.type = 0x1234;
    msg_send_receive(&msg, &msg, second_pid);

    printf("Message sent and acknowledged\n");

    return 0;
}