#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(event_periodic_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>
#include <zephyr/sys/sys_heap.h>


#define HEAP_SIZE	256
static char heap_mem[HEAP_SIZE];
static struct sys_heap heap;
static struct k_heap my_heap;



void my_work_handler(struct k_work *work){
	LOG_INF("Hello from my_work_handler\n");
}
K_WORK_DEFINE(my_work, my_work_handler);

void my_timer_handler(struct k_timer *dummy){
	k_work_submit(&my_work);
}
K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

void print_sys_memory_stats(void){
	struct sys_memory_stats stats;
	sys_heap_runtime_stats_get(&heap, &stats);
	printk("Free heap size: %d bytes\n",stats.free_bytes);
	printk("Largest allocated block: %d bytes\n",stats.max_allocated_bytes); 
	printk("Allocated heap size: %d bytes\n",stats.allocated_bytes);
}

int main(void)
{
	// void *p;
	// sys_heap_init(&heap, heap_mem, HEAP_SIZE);
	// print_sys_memory_stats();
	// p = sys_heap_alloc(&heap, 150);
	// print_sys_memory_stats();
	k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));
	return 0;
}
