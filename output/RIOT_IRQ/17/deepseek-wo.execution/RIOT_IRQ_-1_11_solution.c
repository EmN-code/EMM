
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <esp_system.h>
#include <esp_intr_alloc.h>

atomic_int interrupt_count = ATOMIC_VAR_INIT(0);

void IRAM_ATTR interrupt_handler(void *arg) {
    atomic_fetch_add(&interrupt_count, 1);
}

int main(void) {
    esp_err_t err;
    int intr_alloc_flags = 0;

    // Allocate interrupt
    err = esp_intr_alloc(ETS_UART_INTR_SOURCE, intr_alloc_flags, interrupt_handler, NULL, NULL);
    if (err != ESP_OK) {
        printf("Failed to allocate interrupt\n");
        return -1;
    }

    // Enable the interrupt
    esp_intr_enable(ETS_UART_INTR_SOURCE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Interrupt count: %d\n", atomic_load(&interrupt_count));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Disable the interrupt
    esp_intr_disable(ETS_UART_INTR_SOURCE);

    printf("Final interrupt count: %d\n", atomic_load(&interrupt_count));

    return 0;
}
