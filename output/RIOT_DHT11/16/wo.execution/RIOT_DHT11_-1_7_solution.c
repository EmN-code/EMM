#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *dht_reader(void *arg) {
    while(1) {
        // Simulating a read. Replace with actual sensor read functions.
        int temperature = 25; // Dummy temperature value
        int humidity = 65; // Dummy humidity value
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        // Sleep for 5 seconds
        for (volatile int i = 0; i < 500000; i++); // Simulating delay
    }
    return NULL;
}

int main(void) {
    // Simulate threading
    char stack[2048];
    thread_create(stack, sizeof(stack), 10, 1, dht_reader, NULL, "dht_reader");

    // Main thread does not terminate in a RIOT application
    while (1) {
        ; // Do nothing, just keep the main thread alive
    }
    return 0;
}