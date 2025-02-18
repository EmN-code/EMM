#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(IRQ_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>

#define DELAY 10*USEC_PER_MSEC

static atomic_t a = ATOMIC_INIT(0);
static atomic_t a_during_isr = ATOMIC_INIT(0);
static atomic_t b = ATOMIC_INIT(0);
static atomic_t b_during_isr = ATOMIC_INIT(0);

void timer_callback(struct k_timer *dummy){
	atomic_set(&a_during_isr, atomic_get(&a));
	atomic_set(&b_during_isr, atomic_get(&b));
	LOG_INF("Timer expired\n");
}
K_TIMER_DEFINE(xt, timer_callback, NULL);

int main(void){
	printk("\nTest for irq_disable() / irq_enable()\n======================================\n\n");
	printk("Verifying IRQ state tracking works: \n");
	irq_enable(0);
	uint32_t state_a = irq_is_enabled(0);
	irq_disable(0);
	uint32_t state_b = irq_is_enabled(0);
	irq_enable(0);
	uint32_t state_c = irq_is_enabled(0);
	if ((state_a != 0) && (state_b == 0) && (state_c != 0)) {
        printk("[SUCCESS]\n");
    }else {
        printk("[FAILURE]\n");
    }
	printk("Verifying test works: \n");
	atomic_set(&a, 1);
	k_timer_start(&xt, K_USEC(DELAY/2), K_NO_WAIT);
	k_busy_wait(DELAY);
	atomic_set(&b, 1);
	if((atomic_get(&a) == atomic_get(&a_during_isr)) && (atomic_get(&b) != atomic_get(&b_during_isr))){
		printk("[SUCCESS]\n");
	}else{
		printk("[FAILURE]\n");
	}
	printk("Test result: \n");
	unsigned int state = irq_lock();
	atomic_set(&a, 2);
	k_timer_start(&xt, K_USEC(DELAY/2), K_NO_WAIT);
	k_busy_wait(DELAY);
	atomic_set(&b, 2);
	irq_unlock(state);  // timer 会在unlock之后立即触发
	if((atomic_get(&a) == atomic_get(&a_during_isr)) && (atomic_get(&b) == atomic_get(&b_during_isr))){
		printk("[SUCCESS]\n");
	}else{
		printk("[FAILURE]\n");
	}
	return 0;
}
