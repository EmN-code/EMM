
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;

static volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt flag set, disabling interrupts...\n");
            unsigned state = irq_disable();
            atomic_store(&interrupt_flag, 0);
            irq_restore(state);
            printf("Interrupts restored, interrupt flag cleared.\n");
        }
        xtimer_sleep(1);
    }

    return NULL;
}

void isr(void)
{
    atomic_store(&interrupt_flag, 1);
    printf("Interrupt occurred, setting interrupt flag.\n");
}

int main(void)
{
    // Initialize the thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, thread_func, NULL, "interrupt_handler");

    // Register the interrupt service routine
    irq_set_handler(0, isr); // Assuming interrupt 0 for this example

    // Enable the interrupt
    irq_enable(0);

    printf("Main thread running, waiting for interrupts...\n");

    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}
