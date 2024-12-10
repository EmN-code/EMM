
#include <stdio.h>
#include <thread.h>
#include <irq.h>
#include <atomic_utils.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static kernel_pid_t worker_pid;

static atomic_int_t atomic_var = ATOMIC_INIT(0);

static void worker_thread(void)
{
    while (1) {
        printf("Worker thread running\n");
        xtimer_sleep(1);
    }
}

static void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Create worker thread
    worker_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    // Set IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Test atomic operations
    atomic_store(&atomic_var, 0);
    printf("Atomic variable set to 0\n");

    // Trigger interrupt
    irq_trigger(0);

    // Check atomic variable
    int value = atomic_load(&atomic_var);
    printf("Atomic variable value: %d\n", value);

    return 0;
}
