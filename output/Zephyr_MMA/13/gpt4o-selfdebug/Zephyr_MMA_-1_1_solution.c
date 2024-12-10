#include <zephyr.h>
#include <zephyr/device.h>
#include <drivers/sensor.h>
#include <drivers/nvs.h>
#include <storage/flash_map.h>

#define SLEEP_TIME_SECONDS 5
#define MMA8X5X_LABEL "MMA8X5X"
#define NVS_STORAGE_ID 1

static const struct device *sensor;
static struct nvs_fs nvs_fs;

void main(void) {
   int err;

   printk("ESP32 MMA8x5x Sensor and NVS Application\n");

   sensor = device_get_binding(MMA8X5X_LABEL);
   if (!sensor) {
       printk("Sensor device not found\n");
       return;
   }

   const struct flash_area *fa;
   err = flash_area_open(NVS_STORAGE_ID, &fa);
   if (err) {
       printk("Failed to open flash area\n");
       return;
   }

   nvs_fs.sector_size = fa->fa_size;
   nvs_fs.sector_count = 1U;
   nvs_fs.offset = fa->fa_off;

   err = nvs_init(&nvs_fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
   if (err) {
       printk("NVS initialization failed\n");
       return;
   }

   while (1) {
       struct sensor_value accel[3];

       err = sensor_sample_fetch(sensor);
       if (err) {
           printk("Failed to read sensor\n");
           k_sleep(K_SECONDS(SLEEP_TIME_SECONDS));
           continue;
       }

       err = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
       if (err) {
           printk("Failed to get sensor data\n");
           k_sleep(K_SECONDS(SLEEP_TIME_SECONDS));
           continue;
       }

       printk("Acceleration: X=%d, Y=%d, Z=%d\n", accel[0].val1, accel[1].val1, accel[2].val1);

       for (int i = 0; i < 3; i++) {
           err = nvs_write(&nvs_fs, i, &accel[i], sizeof(struct sensor_value));
           if (err < 0) {
               printk("Error writing NVS\n");
           }
       }

       k_sleep(K_SECONDS(SLEEP_TIME_SECONDS));
   }
}