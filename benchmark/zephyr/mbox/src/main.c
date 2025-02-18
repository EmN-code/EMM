#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(mbox_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>

#define MBOX_SIZE			4
#define TIMEOUT_MS			(100)
// #define TRIGGER_TIMEOUT_MS	(TIMEOUT_MS + TIMEOUT_MS * MBOX_SIZE)

K_MBOX_DEFINE(my_mbox);

void sender_thread(void *dummy1, void *dummy2, void *dummy3)
{
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	for(uint32_t i=0; i <MBOX_SIZE; i++){
		uint32_t message=i;
		struct k_mbox_msg msg={
			.info = 0,
			.tx_data = &message,
			.size = sizeof(message),
			.tx_target_thread = K_ANY
		};
		k_mbox_put(&my_mbox, &msg, K_FOREVER);
		k_sleep(K_MSEC(TIMEOUT_MS));
	}
}
K_THREAD_DEFINE(sender_tid, CONFIG_MAIN_STACK_SIZE, sender_thread, NULL, NULL, NULL, CONFIG_MAIN_THREAD_PRIORITY-1, 0, 0);

int main(void){
	struct k_mbox_msg recv_msg;
	char buffer[100];
	int failed = 0;
	for(uint32_t i=0;i<MBOX_SIZE;i++){
		recv_msg.info = 100;
		recv_msg.rx_source_thread = K_ANY;
		recv_msg.size = 100;
		k_mbox_get(&my_mbox, &recv_msg, buffer, K_FOREVER);
		if(i!=*(uint32_t *)buffer){
			failed = 1;
			break;
		}
	}
	if(!failed){
		LOG_INF("Test successful.");
	}else{
		LOG_ERR("Test failed.");
	}
	return 0;
}
