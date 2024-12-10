
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_wifi.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <shell.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>
#include <sched.h>
#include <assert.h>
#include <stdatomic.h>
#include <stddef.h>

#define THREAD_STACKSIZE_DEFAULT 512

void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    nvs_flash_init();
    esp_wifi_start();

    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(2);

    printf("Changing priority of thread 1 to higher\n");
    sched_change_priority(thread_get_unchecked(pid1), THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(2);

    printf("Changing priority of thread 2 to higher\n");
    sched_change_priority(thread_get_unchecked(pid2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
