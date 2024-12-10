#include "ztimer.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"
#include "esp_system.h"  // Correct header for esp32 systems
typedef struct {
    int16_t temp;
    int16_t hum;
} dht_t; // Correct way to define a structure for DHT sensor readings

#define GPIO_PIN_LED GPIO_NUM_2 // Placeholder for the GPIO pin number for LED
#define GPIO_PIN_DHT GPIO_NUM_4 // Placeholder for the GPIO pin number for DHT sensor
#define TEMP_THRESHOLD 30 // Define the temperature threshold


void read_dht_task(void *param) {
    dht_t dht_data;
    // dht_init(GPIO_PIN_DHT, DHT11); // Initialize DHT sensor on specified GPIO - Correct this definition as per the RIOT API, assuming it's provided
    
    while (1) {
        int16_t temp, hum;
        dht_read(&dht_data, &temp, &hum); // Read the sensor data into the structure
        printf("Temp: %d Hum: %d\n", temp, hum);

        if (temp > TEMP_THRESHOLD) {
            gpio_set(GPIO_PIN_LED); // Set the LED GPIO if the temperature exceeds threshold
        } else {
            gpio_clear(GPIO_PIN_LED); // Clear LED GPIO otherwise
        }
        ztimer_sleep(ZTIMER_SEC, 5); // Sleep for 5 seconds
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_task_func_t task_func = read_dht_task;
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, task_func, NULL, "Read DHT Task");
    return 0;
}