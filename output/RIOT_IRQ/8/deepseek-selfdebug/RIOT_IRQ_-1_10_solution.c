
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

void worker_thread(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt handled correctly.\n");
            atomic_store(&atomic_var, 0);
        }
        thread_yield();
    }
}

int main(void)
{
    kernel_pid_t worker_pid;

    worker_pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    irq_set_handler(IRQ_NUM_USER, interrupt_handler, NULL);

    printf("Starting interrupt handling test...\n");

    while (1) {
        thread_yield();
    }

    return 0;
}
