#include "host/ble_sm.h"
#include "host/ble_store.h"
#include "host/ble_uuid.h"
#include "host/ble_l2cap.h"
#include "host/ble_hs_mbuf.h"
#include "host/ble_hs_id.h"
#include "host/ble_hs_hci.h"
#include "cpu_conf.h"
#include "core/threads.h"
#include "core/msg.h"
#include "core/include/mma8x5x.h"

#define MMA8X5X_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static kernel_pid_t sensor_thread_pid;
static char sensor_thread_stack[MMA8X5X_STACK_SIZE];

typedef struct {
    i2c_t i2c;
    uint16_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void *sensor_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (mma8x5x_read(&dev, &data) == 0) {
            printf("Accel X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            printf("Failed to read from sensor\n");
        }

        msg_receive(&msg);
    }

    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    sensor_thread_pid = thread_create(sensor_thread_stack, sizeof(sensor_thread_stack), 
                                      THREAD_PRIORITY_MAIN - 1, 0, sensor_thread, NULL, "sensor thread");

    while (1) {
        thread_sleep(5000000);
        msg_t msg;
        msg_send(&msg, sensor_thread_pid);
    }

    return 0;
}