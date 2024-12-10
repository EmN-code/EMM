#include <irq_arch.h>
#include <stdio.h>
#include <stdatomic.h>
#include <xtimer.h>

static atomic_int atomic_var;

void disable_interrupts(void) {
    unsigned state = irq_disable();
    atomic_store(&atomic_var, 1); // set atomic variable to simulate work
    irq_restore(state);
}

int main(void) {
    printf("Starting RIOT interrupt handling example\n");
    atomic_init(&atomic_var, 0);

    printf("Initial atomic variable value: %d\n", atomic_load(&atomic_var));
    disable_interrupts();
    printf("Atomic variable after interrupt handling: %d\n", atomic_load(&atomic_var));

    return 0;
}