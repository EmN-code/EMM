#include "xtimer.h"
#include "nvs_flash.h"
#include "msg.h"

#define NVS_NAMESPACE "storage"
#define KEY "device_state"
#define THREAD_STACKSIZE_DEFAULT 1024

static char stack[THREAD_STACKSIZE_DEFAULT];

void store_state(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    // Write state to NVS
    err = nvs_set_u8(my_handle, KEY, state);
    if (err != ESP_OK) {
        printf("Error (%d) writing state to NVS!\n", err);
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing state to NVS!\n", err);
    }

    // Close NVS handle
    nvs_close(my_handle);
}

void init_nvs_storage() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void *state_update_loop(void *arg) {
    (void)arg;
    uint8_t current_state = 0;

    while (1) {
        current_state = (current_state == 0) ? 1 : 0;
        store_state(current_state);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    init_nvs_storage();

    kernel_pid_t pid;
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, state_update_loop, NULL, "state_updater");
    if (pid < 0) {
        printf("Error creating thread\n");
        return 1;
    }

    return 0;
}