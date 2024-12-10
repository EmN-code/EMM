
#include <stdio.h>
#include "periph/i2c.h"
#include "xtimer.h"

// MMA8452Q I2C 地址
#define MMA8452Q_ADDRESS 0x1D

// 寄存器地址
#define REG_CTRL_REG1 0x2A
#define REG_XYZ_DATA_CFG 0x0E
#define REG_OUT_X_MSB 0x01

// 初始化 MMA8452Q
void initMMA8452Q(i2c_t i2c) {
    // 将 MMA8452Q 设置为待机模式
    i2c_write_byte(i2c, MMA8452Q_ADDRESS, REG_CTRL_REG1, 0x00);
    // 设置量程（例如 +/- 2g）
    i2c_write_byte(i2c, MMA8452Q_ADDRESS, REG_XYZ_DATA_CFG, 0x00);
    // 激活 MMA8452Q
    i2c_write_byte(i2c, MMA8452Q_ADDRESS, REG_CTRL_REG1, 0x01);
}

// 读取加速度
void readAcceleration(i2c_t i2c, int *x, int *y, int *z) {
    uint8_t data[6];
    i2c_read_bytes(i2c, MMA8452Q_ADDRESS, REG_OUT_X_MSB, data, 6);

    int rawX = (data[0] << 8) | data[1];
    int rawY = (data[2] << 8) | data[3];
    int rawZ = (data[4] << 8) | data[5];

    // 处理 12 位数据
    *x = (rawX >> 4);
    *y = (rawY >> 4);
    *z = (rawZ >> 4);

    // 如果值大于 2047，则表示负值，需要处理
    if (*x > 2047) *x -= 4096;
    if (*y > 2047) *y -= 4096;
    if (*z > 2047) *z -= 4096;
}

int main(void) {
    i2c_t i2c = I2C_DEV(0);

    // 初始化 I2C 设备
    i2c_init(i2c);

    // 初始化 MMA8452Q
    initMMA8452Q(i2c);

    int x, y, z;
    while (1) {
        // 读取加速度数据
        readAcceleration(i2c, &x, &y, &z);

        // 打印加速度数据
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);

        // 延迟 5 秒
        xtimer_sleep(5);
    }

    return 0;
}
