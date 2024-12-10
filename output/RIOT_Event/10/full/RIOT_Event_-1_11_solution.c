#include <stdint.h>

#include "esp_timer.h"

void periodic_callback(void* arg) {
    printf("Timer callback called every second\n");
}

int main(void) {
    // Configure the timer
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic"};

    esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &periodic_timer);

    // Start the timer
    esp_timer_start_periodic(periodic_timer, 1000000); // 1 second in microseconds

    while (1) {
        // Do other tasks
    }
    return 0;
}