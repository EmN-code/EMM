
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY THREAD_PRIORITY_MAIN - 1

static char thread_stack[THREAD_STACKSIZE];
static atomic_int atomic_var = ATOMIC_INIT(0);

void *interrupt_handler_thread(void *arg)
{
    (void)arg;

    while (1) {
        unsigned state = irq_disable();
        atomic_int_t temp = atomic_var;
        temp++;
        atomic_var = temp;
        irq_restore(state);

        xtimer_sleep(1); // Sleep for 1 second
    }

    return NULL;
}

int main(void)
{
    puts("Interrupt Handling Test with Atomic Variables");

    // Create the interrupt handler thread
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, interrupt_handler_thread, NULL,
                  "interrupt_handler");

    // Main thread will also modify the atomic variable to verify atomic operations
    while (1) {
        unsigned state = irq_disable();
        atomic_int_t temp = atomic_var;
        temp--;
        atomic_var = temp;
        irq_restore(state);

        xtimer_sleep(2); // Sleep for 2 seconds
    }

    return 0;
}
