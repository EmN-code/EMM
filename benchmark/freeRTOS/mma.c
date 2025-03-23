#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

#define TEST_I2C_PORT I2C_NUM_0
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define DATA_LENGTH 7


void mma_task(void *pvParameters){
    
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = TEST_I2C_PORT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));
    
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x1C,
        .scl_speed_hz = 100000,
    };
    
    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
    ESP_ERROR_CHECK(i2c_master_probe(bus_handle, 0x1C, -1));
    uint8_t* data_rd = (uint8_t*)malloc(DATA_LENGTH);
    while(1){
        ESP_ERROR_CHECK(i2c_master_receive(dev_handle, data_rd, DATA_LENGTH, -1));
        printf("Data read: %d,%d,%d,%d,%d,%d,%d\n", data_rd[0],data_rd[1],data_rd[2],data_rd[3],data_rd[4],data_rd[5],data_rd[6]);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}


void app_main(void)
{
    xTaskCreate(mma_task, "mma_task", 4096, NULL, 5, NULL);
}