#include "mma8x5x_params.h"
#include "dht_params.h"
#include <stddef.h>
#include "event/thread.h"
#include <stdatomic.h>
#include "paho_mqtt.h"
#include <stdbool.h>
#include "MQTTClient.h"
#include "dht.h"
#include "thread_config.h"
#include "time_units.h"
#include <unistd.h>
#include <stdlib.h>
#include "kernel_defines.h"

// RIOT specific and ESP32 related imports
#include "esp_log.h"
#include "freertos/task.h"
#include "esp_system.h"

// Define a stack size for the threads
#define THREAD_STACKSIZE 1024

// Define thread priorities
#define THREAD_PRIORITY_MAIN  (5U)
#define THREAD_PRIORITY_OTHER (6U)

// Declare the two thread tasks
void *thread_task_one(void *arg) {
    while (1) {
        // Simulating some work in thread one
        esp_log_level_set("Thread1", ESP_LOG_INFO);
        ESP_LOGI("Thread1", "Thread 1 is running");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return NULL;
}

void *thread_task_two(void *arg) {
    while (1) {
        // Simulating some work in thread two
        esp_log_level_set("Thread2", ESP_LOG_INFO);
        ESP_LOGI("Thread2", "Thread 2 is running");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return NULL;
}

int main(void) {
    char stack_one[THREAD_STACKSIZE];
    char stack_two[THREAD_STACKSIZE];

    // Create thread one
    kernel_pid_t thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_OTHER, 0, thread_task_one, NULL, "thread_one");

    // Create thread two
    kernel_pid_t thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN, 0, thread_task_two, NULL, "thread_two");

    // Change priority of thread two
    void *thread_two = thread_get(thread_two_pid);
    sched_change_priority(thread_two, THREAD_PRIORITY_OTHER);

    while(1) {
        // Main loop - does nothing, main work is done in threads
        esp_log_level_set("Main", ESP_LOG_INFO);
        ESP_LOGI("Main", "Main loop is running");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return 0;
}