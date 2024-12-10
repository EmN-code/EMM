
#include "kernel_defines.h"

#define DHT_PIN 2
#define DHT_TYPE DHT11

typedef struct {
    int16_t temperature;
    int16_t humidity;
} dht_data_t;

void read_dht(void) {
    dht_data_t data;
    // Simulate reading DHT11 data
    data.temperature = 25; // Simulated temperature
    data.humidity = 60;     // Simulated humidity

    // Print the data (simulated)
    printf("Temperature: %d, Humidity: %d\n", data.temperature, data.humidity);
}

int main(void) {
    while (1) {
        read_dht();
        // Simulate a delay of 5 seconds
        for (int i = 0; i < 5; i++) {
            // Simulate a second delay
            for (int j = 0; j < 1000000; j++) {
                // Busy wait
            }
        }
    }
    return 0;
}
