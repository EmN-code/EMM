#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(thread_communication_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>

#ifndef TEST_EXECUTION_NUM
#define TEST_EXECUTION_NUM  (10)
#endif

K_MSGQ_DEFINE(sendq, sizeof(int *), 10, 4);
K_MSGQ_DEFINE(recq, sizeof(int *), 10, 4);

static int counter1 =0;
static int counter2 =0;

void sender_thread(void *dummy1, void *dummy2, void *dummy3)
{
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	int success =1;
	int *data1 = &counter1;
	int *data2 = NULL;
	for(int i=0; i < TEST_EXECUTION_NUM; i++){
		k_msgq_put(&sendq, &data1, K_FOREVER);
		k_msgq_get(&recq, &data2, K_FOREVER);
		if(	(*data2 == NULL) ||
			(&counter1 != data1) ||
			(counter1 != *data1) ||
			(counter1 != *data2) 
		){
			success = 0;
			break;
		}
	}
	if(success){
		LOG_INF("Test successful.");
	}else{
		LOG_ERR("Test failed.");
	}
}

void receiver_thread(void *dummy1, void *dummy2, void *dummy3)
{
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);
	int *data1 = NULL;
	int *data2 = &counter2;
	for(int i =0; i < TEST_EXECUTION_NUM; i++){
		k_msgq_get(&sendq, &data1, K_FOREVER);
		++*data1;
		++*data2;
		LOG_INF("Incremented counters to %d and %d\n", *data1, *data2);
		k_msgq_put(&recq, &data2, K_FOREVER);
	}
}
K_THREAD_DEFINE(sender_tid, CONFIG_MAIN_STACK_SIZE, sender_thread, NULL, NULL, NULL, CONFIG_MAIN_THREAD_PRIORITY-1, 0, 0);
K_THREAD_DEFINE(receiver_tid, CONFIG_MAIN_STACK_SIZE, receiver_thread, NULL, NULL, NULL, CONFIG_MAIN_THREAD_PRIORITY-2, 0, 0);
