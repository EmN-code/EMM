#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <kernel_defines.h>

// Placeholder for user-created callback function
void my_event_callback(void *arg) {
    printf("Event callback triggered!\n");
    // Custom logic for the callback
}

// Thread function to simulate periodic event callback
void *periodic_event_thread(void *arg) {
    printf("Starting periodic event thread...\n");
    while(1) {
        my_event_callback(NULL);
        sleep(1);  // Sleep for 1 second between callbacks
    }
}

int main(void) {
    // Dummy data for stack allocation
    char thread_stack[KERNEL_CONF_STACKSIZE_MAIN];
    
    printf("Initializing thread for periodic events...\n");
    
    // Create and start the thread to simulate an event timer
    thread_create(thread_stack, sizeof(thread_stack), PRIORITY_MAIN-1,
                  0, periodic_event_thread, NULL, "periodic_event");
    
    // Enter main thread loop
    while(1) {
        // Main thread logic
        sleep(10);  // Sleep for demonstration purposes
    }
    return 0;
}