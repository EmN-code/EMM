#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC) // 1-second interval
#define QUEUE_SIZE 8

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[QUEUE_SIZE];
static msg_t msg;
static kernel_pid_t main_pid;

void *msg_sender(void *arg) {
    (void)arg;
    while (1) {
        printf("Sending message...\n");
        msg.type = 0x1234;
        msg.content.value = 42;
        msg_send(&msg, main_pid);
        xtimer_sleep(1); // wait for 1 second
    }
    return NULL;
}

int main(void) {
    puts("RIOT message box example\n");
    msg_init_queue(queue, QUEUE_SIZE);
    main_pid = thread_getpid();
    printf("Main thread PID: %d\n", main_pid);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, msg_sender, NULL, "msg_sender");

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %04x and value: %d\n", msg.type, msg.content.value);
    }

    return 0;
}