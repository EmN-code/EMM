#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define LOW_PRIO_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define HIGH_PRIO_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define HIGH_PRIO_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIO_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

msg_t msg_queue[8];

static char low_prio_thread_stack[LOW_PRIO_THREAD_STACKSIZE];
static char high_prio_thread_stack[HIGH_PRIO_THREAD_STACKSIZE];

void *low_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content
    while (1) {
        printf("[Low Prio Thread] Sending message...\n");
        msg_send_receive(&msg, &msg_reply, thread_getpid_of(&high_prio_thread_handler));
        xtimer_sleep(1);
    }
    return NULL;
}

void *high_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("[High Prio Thread] Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t low_prio_thread_pid = thread_create(low_prio_thread_stack, LOW_PRIO_THREAD_STACKSIZE,
                                                LOW_PRIO_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                                low_prio_thread, NULL, "low_prio_thread");
    kernel_pid_t high_prio_thread_pid = thread_create(high_prio_thread_stack, HIGH_PRIO_THREAD_STACKSIZE,
                                                HIGH_PRIO_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                                high_prio_thread, NULL, "high_prio_thread");

    while (1) {
        // Simulating network connection status
        int rc = 0;
        while (1) {
            rc = NetworkConnect();  // Hypothetical network connect function
            if (rc != 0) {
                printf("Network connection failed, retrying...\n");
                xtimer_sleep(1);
            } else {
                printf("Network connected successfully.\n");
                break;
            }
        }
        xtimer_sleep(10);
    }
    return 0;
}