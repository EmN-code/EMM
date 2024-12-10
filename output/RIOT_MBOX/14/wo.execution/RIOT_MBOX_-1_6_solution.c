#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define MSG_PERIOD (1U * US_PER_SEC)  // 1 second
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char sender_thread_stack[THREAD_STACKSIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t mbox_recipient_pid = thread_getpid();
    while (1) {
        msg.content.value = 42;  // example message value
        printf("Sending message with value: %lu\n", msg.content.value);
        msg_send(&msg, mbox_recipient_pid);
        xtimer_sleep(MSG_PERIOD);
    }
    return NULL;
}

int main(void) {
    puts("RIOT-OS Mailbox Example");

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender_thread");
    /* Keep the main thread alive */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}