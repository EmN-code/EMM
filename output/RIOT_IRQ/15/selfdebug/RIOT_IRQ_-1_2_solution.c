#include <irq_arch.h>
#include <stdbool.h>
#include <stdio.h>

static volatile bool interrupt_disabled = false;

void check_interrupts(void);

unsigned irq_enable(void);
unsigned irq_disable(void);
void irq_restore(unsigned state);
bool irq_is_enabled(void);

void check_interrupts()
{
    if (irq_is_enabled()) {
        printf("Interrupts are enabled.\n");
    }
    else {
        printf("Interrupts are disabled.\n");
    }
}

int main(void)
{
    unsigned state;
    
    printf("Starting interrupt handling test...\n");
    
    check_interrupts();
    
    state = irq_disable();
    interrupt_disabled = true;
    printf("Interrupts disabled.\n");
    
    check_interrupts();
    
    irq_restore(state);
    interrupt_disabled = false;
    printf("Interrupts restored.\n");
    
    check_interrupts();
    
    return 0;
}