#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

void check_error_and_retry(esp_err_t err) {
    if (err != ESP_OK) {
        printf("Error (0x%x) \n", err);
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    check_error_and_retry(err);

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    check_error_and_retry(err);

    int32_t device_state = 0;
    while (1) {
        // Write
        err = nvs_set_i32(my_handle, "state", device_state);
        check_error_and_retry(err);
        
        // Commit written value.
        err = nvs_commit(my_handle);
        check_error_and_retry(err);

        // Toggle state
        device_state = !device_state;

        // Simulate delay for one second (actual function needs hardware access)
        #ifdef BOARD_NATIVE  
           sleep(1); // pseudo code

        #else
            // pseudo code
            printf("waiting for one second");
        #endif
    }

    // Close
    nvs_close(my_handle);
    return 0;
}