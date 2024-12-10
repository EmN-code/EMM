
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_PARTITION DT_NODELABEL(storage_partition)

struct nvs_fs fs;

void init_nvs(void) {
    struct flash_area *fa;
    int rc;

    rc = flash_area_open(DT_FLASH_AREA_ID(STORAGE_PARTITION), &fa);
    if (rc) {
        printk("Failed to open flash area\n");
        return;
    }

    fs.flash_device = (const struct device *)fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = DT_PROP(STORAGE_PARTITION, erase_block_size);
    fs.sector_count = DT_PROP(STORAGE_PARTITION, size) / fs.sector_size;

    rc = nvs_init(&fs, fs.flash_device);
    if (rc) {
        printk("Failed to init NVS\n");
        return;
    }

    flash_area_close(fa);
}

void read_mma8x5x(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device MMA8X5X not ready\n");
        return;
    }

    struct sensor_value accel[3];
    int rc;

    rc = sensor_sample_fetch(dev);
    if (rc) {
        printk("Failed to fetch sample\n");
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Failed to get channel\n");
        return;
    }

    printk("Accel: x=%d.%06d, y=%d.%06d, z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        printk("Failed to write to NVS\n");
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
