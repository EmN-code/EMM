#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <dht.h>
#include <xtimer.h>

#define DHT_PIN 4
#define STORAGE_NAMESPACE "storage"

static void writer_thread(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    int16_t temperature = 0;
    int16_t humidity = 0;
    dht_t dev;
    dht_init(&dev, DHT_PIN);

    while (1) {
        // Open
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        } else {
            // Read the state
data from DHT sensor
            int8_t stored_temp = -1;
            // Read the last written temperature
            err = nvs_get_i8(my_handle, "temperature", &stored_temp);

            // Read temperature and humidity
            if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
                printf("Failed to read from DHT sensor\n");
            } else {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
                // Write
                if (temperature / 10 != stored_temp) {
                    err = nvs_set_i8(my_handle, "temperature", temperature / 10);
                    if (err != ESP_OK) {
                        printf("Failed to write temperature to NVS!. Error: %s\n", esp_err_to_name(err));
                    } else {
                        printf("Temperature written to NVS: %d°C\n", temperature / 10);
                        // Commit written value.
                        err = nvs_commit(my_handle);
                        if (err != ESP_OK) {
                            printf("Failed to commit temperature changes to NVS!\n");
                        }
                    }
                }
            }
            // Close
            nvs_close(my_handle);
        }
        
        // Wait 1 second
        xtimer_sleep(1);
    }
}

int main(void)
{
    msg_t main_msg_queue[8];
    msg_init_queue(main_msg_queue, 8);
    
    // Start writer thread to update temperature state
    writer_thread();
    
    return 0;
}