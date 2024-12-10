#include <event/periodic.h>
#include <event/callback.h>
#include <esp_partition.h>
#include <esp_idf_gpio_install_isr_service.h>

// Declare an event callback function
void callback(void *arg) {
    // This function will be called every second
    printf("Callback executed every second\n");
}

int main(void) {
    // Initialize the periodic event
    event_periodic_t periodic_event;

    // Define the period for the event (1 second)
    uint32_t period = 1 * 1000 * 1000; // in microseconds

    // Initialize the periodic event with the callback
    event_periodic_init(&periodic_event, period, callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop
    while(1) {
        // Add other tasks if needed
        // ..
        // Sleep or wait to maintain the loop
        vTaskDelay(pdMS_TO_TICKS(1000));  // using a hypothetical FreeRTOS delay function
    }

    return 0;
}