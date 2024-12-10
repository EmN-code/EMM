
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE (512)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;

static volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        unsigned state = irq_disable();
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt was handled while interrupts were disabled!\n");
        } else {
            printf("Interrupt was not handled while interrupts were disabled.\n");
        }
        atomic_store(&interrupt_flag, 0);
        irq_restore(state);

        xtimer_sleep(1); // Sleep for 1 second
    }

    return NULL;
}

int main(void)
{
    // Initialize the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Create a thread to check the interrupt flag
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, thread_func, NULL, "interrupt_checker");

    // Simulate an interrupt every 2 seconds
    while (1) {
        xtimer_sleep(2);
        irq_set(IRQ_NUM);
    }

    return 0;
}
