- 14
    
    ```c
    #include "irq.h"
    #include "atomic_utils.h"
    #include "xtimer.h"
    #include "thread.h"
    
    static volatile uint32_t irq_counter = 0;
    static volatile uint32_t atomic_var = 0;
    
    void isr_handler(void *arg)
    {
        (void)arg;
        atomic_fetch_add_u32(&irq_counter, 1);
        atomic_fetch_add_u32(&atomic_var, 1);
    }
    
    void init_interrupt(void)
    {
        /* Disable interrupts */
        uint32_t state = irq_disable();
    
        /* Set up the interrupt handler */
        intr_cntrl_ll_set_int_handler(CPU_INUM_SOFTWARE, isr_handler, NULL);
        intr_cntrl_ll_enable_interrupts(BIT(CPU_INUM_SOFTWARE));
    
        /* Restore interrupts */
        irq_restore(state);
    }
    
    int main(void)
    {
        init_interrupt();
    
        while (1) {
            /* Trigger the interrupt */
            intr_cntrl_ll_trigger_interrupt(CPU_INUM_SOFTWARE);
    
            /* Sleep for a while */
            xtimer_sleep(1);
    
            /* Print the values of the atomic variables */
            printf("irq_counter: %lu, atomic_var: %lu\n", irq_counter, atomic_var);
        }
    
        return 0;
    }
    ```