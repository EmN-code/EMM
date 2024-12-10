#include "nvs_flash_secure_init_partition.h"
#include "nvs_flash_generate_keys.h"
#include "nvs_flash_read_security_cfg.h"
#include "nvs_flash_secure_init.h"
#include "nvs_commit.h"
#include "nvs_set_i32.h"
#include "esp_idf_gpio_set_drive_capability.h"
#include "esp_idf_gpio_set_intr_type.h"
#include "esp_idf_gpio_isr_handler_add.h"
#include "esp_idf_rtc_gpio_pulldown_en.h"
#include "esp_idf_rtc_gpio_pulldown_dis.h"
#include "esp_idf_rtc_gpio_pullup_en.h"
#include "esp_idf_rtc_gpio_pullup_dis.h"
#include "ringbuffer_peek.h"
#include "ringbuffer_remove.h"
#include "ringbuffer_init.h"
#include "ringbuffer_add.h"
#include "ringbuffer_get.h"
#include "MQTTClientInit.h"
#include "MQTTSubackData.h"
#include "MQTTMessage.h"
#include "msg_send_receive.h"
#include "msg_reply.h"
#include "msg_receive.h"
#include "emcute_reg.h"
#include "emcute_type_str.h"
#include "emcute_sub.h"
#include "emcute_con.h"
#include "emcute_cb_t.h"
#include "emcute_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STORAGE_NAMESPACE "storage"

static void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) setting state in NVS!\n", esp_err_to_name(err));
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing changes in NVS!\n", esp_err_to_name(err));
    }
    nvs_close(my_handle);
}

static void update_device_state_periodically(void) {
    while (1) {
        int device_state = rand() % 2;
        store_device_state(device_state);
        sleep(1);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_secure_init();
    if (err != ESP_OK) {
        printf("Error: (%s) initializing flash!\n", esp_err_to_name(err));
        return err;
    }
    update_device_state_periodically();
    return 0;
}