#include <thread_arch.h>
#include <thread_config.h>
#include <net/protnum.h>
#include <net/nanocoap/cache.h>
#include <hal/gpio_types.h>
#include <arch.h>

void *thread1_handler(void *arg) {
    (void)arg;
    for (;;) {
        // Simulate doing some work
        puts("Thread 1 is running...");
        thread_sleep();
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    for (;;) {
        // Simulate doing some work
        puts("Thread 2 is running...");
        thread_sleep();
    }
    return NULL;
}

int main(void) {
    // Create thread1
    kernel_pid_t pid1 = thread_create(NULL, THREAD_STACKSIZE_DEFAULT,
                                     THREAD_PRIORITY_MAIN - 1, 0,
                                     thread1_handler, NULL, "thread1");

    // Create thread2
    kernel_pid_t pid2 = thread_create(NULL, THREAD_STACKSIZE_DEFAULT,
                                     THREAD_PRIORITY_MAIN - 2, 0,
                                     thread2_handler, NULL, "thread2");

    // Simulate runtime priority change
    puts("Changing thread priorities...");
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    // Keep the main function alive
    while (1) {
        thread_sleep();
    }

    return 0;
}