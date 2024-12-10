#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>

K_THREAD_STACK_DEFINE(thread_stack, 1024);
static struct k_thread thread_data;

/* Atomic variable to track interrupt state */
static atomic_t interrupt_occurred = ATOMIC_INIT(0);

/* Interrupt handler function */
void my_isr_handler(void *arg) {
    ARG_UNUSED(arg);

    /* Set the atomic variable to signify interrupt occurrence */
    atomic_inc(&interrupt_occurred);
}

void setup_interrupt(void) {
    /* Get existing interrupt service routine number */
    unsigned int irq_line = IRQ_LINE_NUM;
    unsigned int key;

    /* Lock interrupts to ensure critical section */
    key = irq_lock();

    /* Allocate interrupt */
    irq_connect_dynamic(irq_line, 0, my_isr_handler, NULL, 0);

    /* Unlock interrupts */
    irq_unlock(key);

    /* Enable the interrupt */
    irq_enable(irq_line);
}

void thread_func(void) {
    while (1) {
        /* Check if interrupt occurred */
        if (atomic_get(&interrupt_occurred) != 0) {
            printf("Interrupt has occurred!\n");

            /* Reset atomic variable */
            atomic_set(&interrupt_occurred, 0);
        }

        k_sleep(K_MSEC(1000));
    }
}

int main(void) {
    setup_interrupt();
    
    k_thread_create(&thread_data, thread_stack, K_THREAD_STACK_SIZEOF(thread_stack),
                    (k_thread_entry_t)thread_func, NULL, NULL, NULL,
                    5, 0, K_NO_WAIT);

    return 0;
}