
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&counter, 1);
}

void *worker_thread(void *arg)
{
    (void)arg;
    while (1) {
        printf("Counter: %d\n", atomic_load(&counter));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                                    THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    irq_set_handler(0, irq_handler, NULL);
    irq_trigger(0);

    thread_join(pid);

    return 0;
}
