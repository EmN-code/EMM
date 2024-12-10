#include "xtimer.h"
#include "timex.h"
#include "thread_flags.h"
#include "nvs.h"

static void update_temperature(nvs_handle_t my_handle) {
    int32_t temp = 1;  // Simulating reading temperature as either 1 or 0
    nvs_set_i32(my_handle, "temperature", temp);
    nvs_commit(my_handle);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        // Handle error
        return -1;
    }

    while (1) {
        update_temperature(my_handle);
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}