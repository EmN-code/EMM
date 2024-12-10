#include <zephyr.h>                // As RIOT OS doesn't support NRF52840 directly, we use a Zephyr-based example.
#include <device.h>
#include <drivers/sensor.h>          // For sensor management
#include <devicetree.h>              // For device tree access
#include <sys/printk.h>              // For printk

#define DHT11_NODE DT_ALIAS(dht11)

#if !DT_NODE_HAS_STATUS(DHT11_NODE, okay)
#error "Unsupported board: dht11 devicetree alias is not defined"
#endif

#define DHT11 DT_LABEL(DHT11_NODE)

void main(void) {
    const struct device *dev = device_get_binding(DHT11);

    if (dev == NULL) {
        printk("No device found\n");
        return;
    }

    while (1) {
        struct sensor_value temp, hum;
        int ret = sensor_sample_fetch(dev);

        if (ret) {
            printk("Sensor fetch error: %d\n", ret);
            continue;
        }

        sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
        sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum);

        printk("Temperature: %d.%06d; Humidity: %d.%06d\n",
               temp.val1, temp.val2, hum.val1, hum.val2);

        k_sleep(K_SECONDS(5));
    }
}