#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <msg.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <timex.h>
#include <unistd.h>
#include <kernel_defines.h>

#define MAIN_QUEUE_SIZE     (8U)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *send_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;
    while (1) {
        msg.content.value = count++;
        printf("Sending message with value: %d\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receive_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t send_pid = thread_create(send_stack, sizeof(send_stack), THREAD_PRIORITY_MAIN - 1, 0,
                                          send_thread, NULL, "send_thread");
    kernel_pid_t receive_pid = thread_create(receive_stack, sizeof(receive_stack), THREAD_PRIORITY_MAIN - 1, 0,
                                             receive_thread, NULL, "receive_thread");
    (void)send_pid;  // to avoid 'unused variable' warnings
    (void)receive_pid; // to avoid 'unused variable' warnings
    char *stack_buffer = malloc(sizeof(char) * THREAD_STACKSIZE_DEFAULT * THREAD_PRIORITY_MAIN);
    if (stack_buffer == NULL) {
        printf("Failed to allocate memory for thread stack.\n");
        return 1;
    }
    thread_yield();  
    free(stack_buffer);
    return 0;
}