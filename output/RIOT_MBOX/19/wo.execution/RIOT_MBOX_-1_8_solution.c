#include <xtimer.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <thread.h>

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_INTERVAL     (1U * US_PER_SEC)  // Send every 1 second

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    puts("[sender] Started");

    while (1) {
        msg.content.value = 0xBEEF;
        printf("[sender] Sending message with value: 0x%lx\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_sleep(SENDER_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("RIOT message box example");
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("[receiver] Received message with value: 0x%lx\n", msg.content.value);
    }
    return 0;
}