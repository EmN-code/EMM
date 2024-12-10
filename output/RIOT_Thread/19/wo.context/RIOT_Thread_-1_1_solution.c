#include <thread.h>
#include <stdio.h>

#define STACK_SIZE (1024)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN - 2)

static char stack_thread1[STACK_SIZE];
static char stack_thread2[STACK_SIZE];

msg_t msg_queue[8];

void *thread_low(void *arg) {
    msg_t msg;
    (void)arg;

    int my_pid = thread_getpid();
    printf("Thread %d: Waiting for messages\n", my_pid);

    while (1) {
        msg_receive(&msg);
        printf("Thread %d: Received message: %ld\n", my_pid, msg.content.value);
        if (msg.content.value == 0) break;
    }

    printf("Thread %d: Exiting\n", my_pid);
    return NULL;
}

void *thread_high(void *arg) {
    (void)arg;

    int target_pid = *((int *)arg);
    msg_t msg;
    msg.type = 0;

    printf("High priority thread sending messages\n");
    for (long i = 1; i <= 5; ++i) {  // Sending 5 messages
        msg.content.value = i;
        msg_send(&msg, target_pid);
        xtimer_sleep(1);
    }

    // Sending a last message to signal termination
    msg.content.value = 0;
    msg_send(&msg, target_pid);

    printf("High priority thread exiting\n");
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    int pid_thread1 = thread_create(stack_thread1, sizeof(stack_thread1), PRIORITY_LOW, 0,
                                    thread_low, NULL, "thread_low");

    thread_create(stack_thread2, sizeof(stack_thread2), PRIORITY_HIGH, 0,
                  thread_high, &pid_thread1, "thread_high");

    printf("Threads created, and running\n");

    while (1) {
        // Main loop can perform other tasks
        xtimer_sleep(10);
    }
    return 0;
}