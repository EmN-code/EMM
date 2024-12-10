#include <thread.h>  
#include <msg.h>  
#include <xtimer.h>  
#include <stdio.h>  

#define THREAD_STACKSIZE_DEFAULT 1024  
char stack_low[THREAD_STACKSIZE_DEFAULT];  
char stack_high[THREAD_STACKSIZE_DEFAULT];  

msg_t msg_queue[8];  

void *thread_low(void *arg) {  
    (void)arg;  
    msg_t msg;  
    while (1) {  
        msg_receive(&msg);  
        printf("Received in low priority thread: type %lu\n", (unsigned long)msg.type);  
    }  
    return NULL;  
}  

void *thread_high(void *arg) {  
    (void)arg;  
    msg_t msg;  
    msg.type = 1;  
    while (1) {  
        printf("Sending from high priority thread: type %lu\n", (unsigned long)msg.type);  
        msg_send(&msg, 0);  
        xtimer_sleep(1);  
    }  
    return NULL;  
}  

int main(void) {  
    msg_init_queue(msg_queue, 8);  
    thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1, 0, thread_low, NULL, "thread_low");  
    thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 2, 0, thread_high, NULL, "thread_high");  
    while (1) {}  
    return 0;  
}