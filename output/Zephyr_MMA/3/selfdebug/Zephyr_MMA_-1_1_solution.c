#include <zephyr.h>
#include <drivers/sensor.h>
#include <nvs/nvs.h>
#include <device.h>
#include <stdio.h>
#include <zephyr/types.h>

#define STORAGE_PARTITION_LABEL storage
#define NVS_STORAGE_ID 1
#define CHECK_INTERVAL K_SECONDS(5)

void main(void) {
    const struct device *sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_fxos8700)));
    struct sensor_value accel;
    struct nvs_fs fs;
    int rc;
    if (sensor == NULL) {
        printf("No device found. Aborting.\n");
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(STORAGE_PARTITION_LABEL);
    fs.sector_size = FLASH_AREA_SIZE(STORAGE_PARTITION_LABEL);
    fs.sector_count = 3;
    fs.magic = NVS_MAGIC;
    rc = nvs_init(&fs, DT_CHOSEN(zephyr_flash_controller));
    if (rc) {
        printf("Flash Init failed\n");
        return;
    }
    
    while (1) {
        k_sleep(CHECK_INTERVAL);
        rc = sensor_sample_fetch(sensor);
        if (rc == 0) {
            rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &accel);
            if (rc != 0) {
                printf("Error reading sensor data\n");
                continue;
            }
            printf("Accel: X = %d, Y = %d, Z = %d\n", accel.val1, accel.val2, accel.val3);

            rc = nvs_write(&fs, NVS_STORAGE_ID, &accel, sizeof(accel));
            if (rc < 0) {
                printf("Failed to write to NVS\n");
            }
        } else {
            printf("Sensor fetch error\n");
        }
    }
}