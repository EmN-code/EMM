#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "network.h"  // Assuming network related functions like NetworkConnect

#define MAIN_QUEUE_SIZE     (8)
#define HIGH_PRIO_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define LOW_PRIO_STACKSIZE  (THREAD_STACKSIZE_DEFAULT)

static char high_prio_thread_stack[HIGH_PRIO_STACKSIZE];
static char low_prio_thread_stack[LOW_PRIO_STACKSIZE];

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *high_prio_thread_handler(void *arg) {
    msg_t msg;
    char *response = "High Priority Response";
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char*)msg.content.ptr);
        msg.content.ptr = response;
        msg_reply(&msg, &msg);
    }
    return NULL;
}

void *low_prio_thread_handler(void *arg) {
    msg_t msg, reply;
    msg.content.ptr = "Low Priority Request";
    if (msg_send_receive(&msg, &reply, thread_getpid()) > 0) {
        printf("Low priority thread received reply: %s\n", (char*)reply.content.ptr);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t high_prio_thread_pid = thread_create(
        high_prio_thread_stack, sizeof(high_prio_thread_stack),
        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
        high_prio_thread_handler, NULL, "high_prio_thread");

    if (high_prio_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create high priority thread\n");
        return 1;
    }

    kernel_pid_t low_prio_thread_pid = thread_create(
        low_prio_thread_stack, sizeof(low_prio_thread_stack),
        THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
        low_prio_thread_handler, NULL, "low_prio_thread");

    if (low_prio_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create low priority thread\n");
        return 1;
    }

    struct Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, "192.168.1.1", 1883);
        if (rc != SUCCESS) {
            printf("Network connection failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    return 0;
}