
#include "thread.h"
#include "stdio.h"
#include "unistd.h"

// Define a simple callback function
void periodic_callback(void) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    while (1) {
        // Call the periodic callback function
        periodic_callback();
        
        // Sleep for 1 second
        thread_sleep(1000000); // 1 second in microseconds
    }

    return 0;
}
