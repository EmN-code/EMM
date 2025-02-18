#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(timer_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>


void my_timer_handler(struct k_timer *dummy){
	LOG_INF("Timer triggered!\n");
}
K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

int main(void)
{
	k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));

	return 0;
}
