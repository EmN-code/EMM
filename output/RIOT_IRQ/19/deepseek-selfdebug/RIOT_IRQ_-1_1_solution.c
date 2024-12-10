
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t thread_pid;
static volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt flag set, disabling interrupts...\n");
            unsigned state = irq_disable();
            printf("Interrupts disabled, performing critical section...\n");
            xtimer_sleep(1);  // Simulate critical section
            irq_restore(state);
            printf("Interrupts restored, clearing interrupt flag...\n");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_usleep(100000);  // Sleep for 100 ms
    }

    return NULL;
}

void irq_handler(void)
{
    atomic_store(&interrupt_flag, 1);
    printf("Interrupt occurred, setting interrupt flag...\n");
}

int main(void)
{
    // Initialize the thread
    thread_pid = thread_create(
        thread_stack, THREAD_STACKSIZE, THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST, thread_func, NULL, "interrupt_handler");

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    printf("Main thread running, waiting for interrupts...\n");

    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
