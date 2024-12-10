
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

atomic_int atomic_var;
char stack[STACKSIZE];

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Creating interrupt handler thread...\n");
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, interrupt_handler, NULL, "interrupt_handler");

    printf("Restoring interrupts...\n");
    irq_restore(state);

    printf("Waiting for interrupt...\n");
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handled successfully!\n");
    return 0;
}
