/* The following code implements a simple RIOT application on an ESP32 microcontroller. 
   It creates two threads and changes their priorities at runtime.
   We include all necessary headers and ensure no name conflicts or redefinitions occur. 
*/

#include <ztimer.h> // For time functions
#include <xtimer.h> // XTIMER is a high-accuracy timer for RIOT 
#include <timex.h>  // For time conversion macros and functions.
#include <time_units.h> // working with time units
#include <thread_arch.h>  // Architecture specific thread definitions
#include <thread_config.h> // For thread configuration related macros
#include <thread.h> // RIOT's threading library 
#include <stdio.h> // For standard input/output functions
#include <stddef.h> // define several useful types and macros
#include <stdatomic.h> // For atomic operations
#include <stdlib.h> // General utilities: memory management, program utilities, string conversions, random numbers
#include <stdbool.h> // For boolean type support
#include <unistd.h> // for POSIX operating system API
#include <paho_mqtt.h> // MQTT related network functions
#include <MQTTClient.h> // MQTT client functions
#include <kernel_defines.h> // For kernel definitions
#include <architecture.h>  // generic architecture specific includes


#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN_DEFAULT)

static char thread_stack_1[THREAD_STACKSIZE_MAIN];  
static char thread_stack_2[THREAD_STACKSIZE_MAIN];

static void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running at priority %d\n", (int) thread_get_priority(thread_getpid()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

static void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running at priority %d\n", (int) thread_get_priority(thread_getpid()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    printf("Starting thread test\n");

    thread1_pid = thread_create(thread_stack_1, sizeof(thread_stack_1), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(thread_stack_2, sizeof(thread_stack_2), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Let threads run for a brief period
    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing priorities\n");

    kernel_pid_t main_pid = thread_getpid();
    thread_get_priority(&main_pid);

    // Change thread priorities
    thread_create(thread1_stack, sizeof(thread_stack_1), THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    thread_create(thread2_stack, sizeof(thread_stack_2), THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    // let the threads run again
    ztimer_sleep(ZTIMER_MSEC, 5000);
    
    return 0;
}

