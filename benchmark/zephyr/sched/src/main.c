#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(sched_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>

void t2_func(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	while(1){
		LOG_INF("[t2] is running. The priority of [t2] is %d.\n", k_thread_priority_get(k_current_get()));
		k_sleep(K_SECONDS(1));
		// k_yield();
	}
	return;
}

void t3_func(void *dummy1, void *dummy2, void *dummy3){
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	
	while(1){
		LOG_INF("[t3] is running. The priority of [t3] is %d.\n", k_thread_priority_get(k_current_get()));
		k_sleep(K_SECONDS(1));
		// k_yield();
	}
}

K_THREAD_DEFINE(t2_id, CONFIG_MAIN_STACK_SIZE, t2_func, NULL, NULL, NULL, CONFIG_MAIN_THREAD_PRIORITY+2, 0, 0);
K_THREAD_DEFINE(t3_id, CONFIG_MAIN_STACK_SIZE, t3_func, NULL, NULL, NULL, CONFIG_MAIN_THREAD_PRIORITY+1, 0, 0);

int main(void)
{
	LOG_INF("[main] threads starting.\n");
	k_sleep(K_SECONDS(2));
	k_thread_priority_set(t3_id, CONFIG_MAIN_THREAD_PRIORITY+3);
	while(1){
		k_sleep(K_SECONDS(1));
	}
	
	return 0;
}
