#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(RTC_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>
#include <time.h>
#include <sys/time.h>


int main(void){

	time_t t;
	struct tm *gmt, *area;

	while(1){
		k_sleep(K_SECONDS(1));
		t = time(NULL);
		area = localtime(&t);
		printk("Local time is: %s", asctime(area));
		gmt = gmtime(&t);
		printk("GMT is: %s", asctime(gmt));
	}
	
	return 0;
}
