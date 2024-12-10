
#include <stdio.h>
#include <thread.h>
#include <irq.h>
#include <atomic_utils.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

atomic_int_t atomic_var = ATOMIC_INIT(0);

void *worker_thread(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt occurred and atomic variable was set to 1\n");
            atomic_store(&atomic_var, 0);
        }
        thread_yield();
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t pid;
    char stack[STACKSIZE];

    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    if (pid <= 0) {
        printf("Failed to create worker thread\n");
        return 1;
    }

    while (1) {
        unsigned state = irq_disable();
        atomic_store(&atomic_var, 1);
        irq_restore(state);
        thread_sleep(1000000); // Sleep for 1 second
    }

    return 0;
}
