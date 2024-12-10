#include "thread.h"
#include "timex.h"
#include "nvs_flash.h"
#include "nvs.h"

static nvs_handle_t nvs_handle;
static const char *NVS_NAMESPACE = "storage";
static const char *DEVICE_STATE_KEY = "device_state";

void init_nvs(void) {
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        result = nvs_flash_init();
    }
    if (result != ESP_OK) {
        printf("Failed to initialize NVS\n");
    }
    
    result = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (result != ESP_OK) {
        printf("Error opening NVS handle!\n");
    }
}

void update_device_state(void) {
    static uint8_t state = 0;
    
    // Read current state
    size_t length = sizeof(state);
    esp_err_t result = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (result == ESP_ERR_NVS_NOT_FOUND) {
        printf("State not found, defaulting to 0\n");
    }

    // Toggle state
    state = !state;
    
    // Save new state
    result = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (result == ESP_OK) {
        nvs_commit(nvs_handle);
    } else {
        printf("Failed to save the state!\n");
    }
}

void *device_loop(void *arg) {
    (void) arg;
    while (1) {
        update_device_state();
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    init_nvs();
    
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, device_loop, NULL, "device_loop");
    
    return 0;
}
