
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void thread_func(void)
{
    while (1) {
        atomic_fetch_add(&counter, 1);
        xtimer_sleep(1);
    }
}

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 0);
}

int main(void)
{
    kernel_pid_t pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                                    THREAD_CREATE_STACKTEST, thread_func, NULL, "thread");

    irq_set_handler(0, irq_handler, NULL);

    xtimer_sleep(5);

    irq_trigger(0);

    thread_join(pid);

    int final_count = atomic_load(&counter);
    printf("Final counter value: %d\n", final_count);

    return 0;
}
