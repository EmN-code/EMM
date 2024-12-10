
#include "periph/uart.h"
#include "periph/irq.h"

static volatile int interrupt_occurred = 0;

void uart_rx_callback(void *arg) {
    (void)arg;
    interrupt_occurred = 1;
}

int main(void) {
    uart_t uart = UART_DEV(0);
    uint32_t baudrate = 115200;

    // Initialize UART with the correct parameters
    uart_init(uart, baudrate, uart_rx_callback, NULL);

    // Enable UART RX interrupt
    uart_irq_enable(uart, UART_IRQ_RX);

    while (1) {
        if (interrupt_occurred) {
            interrupt_occurred = 0;
            // Handle the interrupt
            // For demonstration, we just print a message
            // Note: printf is not allowed, so we use a simple flag check
        }
    }

    return 0;
}
