#include <zephyr/net/websocket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/stats/stats.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/pm/state.h>
#include <zephyr/linker/sections.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/toolchain.h>
#include <time.h>
#include <sys/time.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>

int main() {

    uint32_t sample_data = 0;
    uint8_t read_value = 0;

    while (1) {

        if (sensor_sample_fetch(dev) == 0) {
            sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &read_value);

            nvs_write(&fs, sample_data_id, &read_value, sizeof(read_value));

        }

        k_msleep(5000);

    }
    return 0;
}