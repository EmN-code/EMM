#include <string.h>
#include "msg.h"
#include "mutex.h"
#include "sys/socket.h"
#include "net/emcute.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t _main_pid;
static char _main_stack[THREAD_STACKSIZE_MAIN];

void *main_thread(void *arg) {
    (void)arg;
    
    puts("Initializing...");
    
    // Initialize the MMA8x5x accelerometer
    mma8x5x_init(&dev, &params);
    
    while (1) {
        // Read the accelerometer data every 5 seconds
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("Main function started\n");
    _main_pid = thread_create(_main_stack, sizeof(_main_stack), THREAD_PRIORITY_MAIN, 0,
                              main_thread, NULL, "main_thread");
    if (!_main_pid) {
        puts("Error: unable to create main thread");
        return 1;
    }
    
    // Start the main thread
    thread_yield();

    return 0;
}