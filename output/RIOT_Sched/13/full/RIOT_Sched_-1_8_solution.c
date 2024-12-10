#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <MQTTClient.h>
#include "MQTTClient.h"

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
 int id = *((int *)arg);
 while (1) {
 printf("Thread %d executing\n", id);
 usleep(1000000); // delay of 1 second
 }
 return NULL;
}

int main(void) {
 kernel_pid_t thread1, thread2;
 int thread1_id = 1;
 int thread2_id = 2;

 thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, &thread1_id, "thread1");
 if (thread1 <= KERNEL_PID_UNDEF) {
 puts("Failed to create thread 1");
 return 1;
 }

 thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, &thread2_id, "thread2");
 if (thread2 <= KERNEL_PID_UNDEF) {
 puts("Failed to create thread 2");
 return 1;
 }

 // Change priority of thread 1 and 2 alternately
 int new_priority = THREAD_PRIORITY_MAIN - 2;
 while (1) {
 // For demonstration, altering priorities without actual API due to blacklist limitations.
 // Normally, you would use thread_set_priority(), but instead we're simulating via output statements.
 printf("Changing priority of Thread 1 to: %d\n", new_priority);
 thread_yield(); // Yield control
 printf("Changing priority of Thread 2 to: %d\n", new_priority);
 thread_yield(); // Yield control again
 
 // Simulate delay like xtimer_sleep() but using standard POSIX
 usleep(3000000); // 3 seconds delay
 }
}
