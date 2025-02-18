#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

int main(){
	const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
	uint32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_CONTROLLER;
	uint32_t i2c_cfg_tmp;
	unsigned char datas[7];
	// struct sensor_value accel[3];
	if (!device_is_ready(dev)) {
		printk("\nDevice %s is not ready\n", dev->name);
		return 0;
	}else{
		printk("\nDevice %s is ready\n", dev->name);
	}
	if (i2c_configure(dev, i2c_cfg)) {
		printk("I2C config failed\n");
		return 0;
	}
	if (i2c_get_config(dev, &i2c_cfg_tmp)) {
		printk("I2C get_config failed\n");
		return 0;
	}
	if (i2c_cfg != i2c_cfg_tmp) {
		printk("I2C get_config returned invalid config\n");
		return 0;
	}
	while(1){
		(void)memset(datas, 0, sizeof(datas)); 
		if(i2c_read(dev, datas, 7, 0x1D)){ // mma845x address is 0x1D
			printk("Fail to fetch sample from sensor.\n");
			return 0;
		}
		printk("axis raw data: %d %d %d %d %d %d %d\n",
					datas[0], datas[1], datas[2],
					datas[3], datas[4], datas[5], datas[6]);
		k_sleep(K_MSEC(200));
	}

	return 0;
}