
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE 512
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
            printf("Interrupts disabled and restored correctly.\n");
        }
        atomic_store(&interrupt_flag, 0);
        irq_restore(state);

        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, thread_func, NULL, "interrupt_thread");

    // Simulate an interrupt by calling the handler directly
    while (1) {
        xtimer_sleep(2);
        interrupt_handler();
    }

    return 0;
}
