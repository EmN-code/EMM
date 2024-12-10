
#include "event/thread.h"
#include "irq.h"
#include "thread.h"
#include "MQTTClient.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define UART_DEV UART_DEV(0)
#define UART_IRQ_NUM UART_0_IRQ_NUM

static volatile bool isr_flag = false;

static void uart_isr(void *arg)
{
    (void)arg;
    isr_flag = true;
}

int main(void)
{
    // Initialize UART
    uart_init(UART_DEV, NULL, NULL);

    // Attach ISR to UART IRQ
    irq_disable();
    irq_set(UART_IRQ_NUM, uart_isr, NULL);
    irq_enable(UART_IRQ_NUM);
    irq_restore(irq_disable());

    // Simulate an interrupt
    uart_irq_callback_set(UART_DEV, uart_isr);
    uart_irq_enable(UART_DEV, UART_RX_IRQ_ENABLE);

    // Disable UART RX IRQ
    uart_rxstart_irq_disable(UART_DEV);

    // Check if the ISR flag was set
    if (isr_flag) {
        printf("Interrupt handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
