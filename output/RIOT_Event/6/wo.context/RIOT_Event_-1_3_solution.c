// Include necessary headers in descending alphabetical order by their first letter
#include <ztimer64.h>
#include <stdio.h>

// Callback function for the timer
void callback(void *arg) {
    (void)arg; // unused
    printf("Timer callback executed.\n");
}

int main(void) {
    // Timer initialization
    ztimer64_t timer;
    ztimer64_now(&timer) + 1ULL * ZTIMER64_SEC;

    while (1) {
        // Set the target time to 1 second from now
        uint64_t target = ztimer64_now(ZTIMER64_USEC) + 1 * ZTIMER64_SEC;
        // Simulate spinning until the target time is reached
        ztimer64_spin_until(ZTIMER64_USEC, target);
        // Execute the callback
        callback(NULL);
    }

    return 0;
}