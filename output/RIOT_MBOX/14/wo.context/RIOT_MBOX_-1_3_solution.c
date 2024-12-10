#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL            (2U * US_PER_SEC)  // 2 seconds

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *message_sender(void *arg) {
    (void)arg;
    kernel_pid_t main_pid = thread_getpid();
    msg_t msg;
    msg.content.value = 42;  // Example content value to be sent

    while (1) {
        printf("Sending message with content: %lu\n", msg.content.value);
        // Use msg_send and pass the correct integer-type content
        msg_send(&msg, main_pid);
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Message box example on ESP32\n");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  0, message_sender, NULL, "message_sender");
    
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with content: %lu\n", msg.content.value);
    }
    return 0;
}