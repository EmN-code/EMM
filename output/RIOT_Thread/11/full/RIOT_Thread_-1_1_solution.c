#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
char main_thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

#define THREAD_A_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_B_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_A_PRIORITY  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_B_PRIORITY  (THREAD_PRIORITY_MAIN - 2)

static char thread_a_stack[THREAD_A_STACKSIZE];
static char thread_b_stack[THREAD_B_STACKSIZE];

static kernel_pid_t thread_a_pid;
static kernel_pid_t thread_b_pid;

void thread_a_func(void *arg) {
    (void)arg;
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    while(1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Thread A received message: %d\n", msg.content.value);
    }
}

void thread_b_func(void *arg) {
    (void)arg;
    while (1) {
        msg_t msg;
        msg.content.value = 42; // example message content
        int rc = msg_send(&msg, thread_a_pid);

        if (rc <= 0) {
            printf("Thread B failed to send message\n");
        } else {
            printf("Thread B sent message\n");
        }

        xtimer_sleep(5);
    }
}

int main(void) {
    printf("ESP32 RIOT Inter-Thread Communication Example\n");

    thread_a_pid = thread_create(thread_a_stack, sizeof(thread_a_stack),
                                 THREAD_A_PRIORITY, THREAD_CREATE_STACKTEST,
                                 thread_a_func, NULL, "thread_a");

    thread_b_pid = thread_create(thread_b_stack, sizeof(thread_b_stack),
                                 THREAD_B_PRIORITY, THREAD_CREATE_STACKTEST,
                                 thread_b_func, NULL, "thread_b");

    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main thread received message\n");
    }
    return 0;
}