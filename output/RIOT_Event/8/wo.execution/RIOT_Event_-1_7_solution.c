#include <time.h>
#include <event.h>
#include <esp_timer.h>
#include <assert.h>
#include <stdio.h>

// Define the callback function that will be called every second
timer_event_t evt;

typedef struct {
    void (*callback)(void);
} timer_event_t;

void event_periodic_callback() {
    printf("Periodic event triggered\n");
}

// Initialize and start the periodic event
void init_event_periodic()
{
    evt.callback = event_periodic_callback;

    // Espressif timer API initialization
    esp_timer_create_args_t timer_args = {
        .callback = (void*)evt.callback,
        .name = "periodic"
    };

    esp_timer_handle_t timer_handle;
    esp_timer_create(&timer_args, &timer_handle);

    // Start the timer, firing every 1 second (1,000,000 microseconds)
    esp_timer_start_periodic(timer_handle, 1000000);
}

int main() {
    init_event_periodic();

    // Let the main loop process runtime events
    while(1) {
        // Simulate the scheduler of RIOT
        usleep(1000000);
    }

    return 0;
}