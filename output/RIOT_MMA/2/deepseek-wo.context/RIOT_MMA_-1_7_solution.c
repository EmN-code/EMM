
#include "architecture.h"
#include "stddef.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "evtimer_mbox.h"
#include "freertos/task.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "thread.h"
#include "net/gcoap.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "freertos/FreeRTOS.h"
#include "net/sock/udp.h"
#include "assert.h"
#include "string.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "sys/socket.h"
#include "mbox.h"
#include "nvs_flash.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "periph/rtc.h"
#include "timex.h"
#include "nvs.h"
#include "stdio.h"
#include "periph/i2c.h"
#include "stdbool.h"
#include "esp_system.h"
#include "msg.h"
#include "ztimer.h"
#include "event/periodic_callback.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void accelerometer_task(void *arg)
{
    (void)arg;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    xTaskCreate(accelerometer_task, "accelerometer_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}
