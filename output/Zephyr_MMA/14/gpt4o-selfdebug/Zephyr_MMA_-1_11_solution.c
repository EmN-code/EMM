#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

#define SLEEP_TIME_SECONDS 5

void main(void)
{
    const struct device *flash_dev;
    struct flash_pages_info info;
    uint32_t storage_offset = 0x00000000;
    uint8_t mma_data = 123; // Example data from MMA8x5x
    
    flash_dev = device_get_binding(DT_LABEL(DT_CHOSEN(zephyr_flash_controller)));
    if (!device_is_ready(flash_dev)) {
        printk("Flash device not ready\n");
        return;
    }

    /* Get flash page info */
    flash_get_page_info_by_offs(flash_dev, storage_offset, &info);

    while (1) {
        printk("Storing MMA8x5x data to flash memory\n");
        if (flash_write(flash_dev, storage_offset, &mma_data, sizeof(mma_data)) != 0) {
            printk("Flash write failed\n");
        }
        k_sleep(K_SECONDS(SLEEP_TIME_SECONDS));
    }
}