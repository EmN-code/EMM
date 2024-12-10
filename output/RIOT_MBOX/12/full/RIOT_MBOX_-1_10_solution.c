#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define MAIN_QUEUE_SIZE          (32)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count;
        msg_send(&msg, thread_getpid());
        printf("Message %d sent\n", count);
        count++;
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    puts("RIOT ESP32 Message Box Example");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender_thread");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return 0;
}