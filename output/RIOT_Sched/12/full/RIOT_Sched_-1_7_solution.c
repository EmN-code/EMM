#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread_config.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "stdio.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "netemcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
n
#define MAIN_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN)
#define MAIN_QUEUE_SIZE       (8)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

kernel_pid_t thread1_pid, thread2_pid;

void *thread_handler(void *arg) {
    while (1) {
        // Simulate work
        for (int i = 0; i < 100000; ++i) {
            __asm__ volatile ("nop");
        }
        printf("%s running...\n", (char *)arg);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    thread1_pid = thread_create(stack1, THREAD_STACKSIZE_DEFAULT,
                                THREAD_PRIORITY_MAIN, 0, thread_handler,
                                "Thread 1", "Thread 1");
    
    thread2_pid = thread_create(stack2, THREAD_STACKSIZE_DEFAULT,
                                THREAD_PRIORITY_MAIN + 1, 0, thread_handler,
                                "Thread 2", "Thread 2");

    // Change thread priorities
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    // Network connection with retry mechanism
    int rc;
    while (1) {
        rc = NetworkConnect();  // Placeholder: Replace with actual function
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
        } else {
            printf("Network connected successfully!\n");
            break;
        }
    }

    return 0;
}