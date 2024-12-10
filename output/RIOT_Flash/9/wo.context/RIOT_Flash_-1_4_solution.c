#include <xtimer.h> 
#include <timex.h>
#include <thread_flags.h>
#include <cpu_conf.h> 
#include <esp_system.h>
#include <nvs_flash.h>
#include <nvs.h>

#define INTERVAL (1U * 1000000U) // 1 second interval

static void store_state_in_nvs(uint8_t state) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }
    printf("Writing state %u to NVS...\n", state);
    err = nvs_set_u8(nvs_handle, "state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS!\n");
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes to NVS!\n");
    }
    nvs_close(nvs_handle);
}

static uint8_t read_state_from_nvs(void) {
    nvs_handle_t nvs_handle;
    uint8_t state = 0;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return state;
    }
    err = nvs_get_u8(nvs_handle, "state", &state);
    switch (err) {
        case ESP_OK:
            printf("Read state %u from NVS\n", state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("State not set in NVS\n");
            break;
        default:
            printf("Error (%d) reading state from NVS\n", err);
    }
    nvs_close(nvs_handle);
    return state;
}

int main(void) {
    // Initialize the NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_init());
    }

    uint8_t state = read_state_from_nvs();

    while (1) {
        state = !state; // Toggle state
        store_state_in_nvs(state);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}