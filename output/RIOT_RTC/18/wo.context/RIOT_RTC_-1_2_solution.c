#include <stdio.h>
#include <event/callback.h>

rtc_alarm_cb_t alarm_callback;

// Dummy function to simulate RTC initialization
void rtc_init() {
    // Normally you'd here set up the RTC hardware
    printf("RTC initialized\n");
}

// Dummy function to simulate RTC alarm setting
void rtc_set_alarm(int seconds_from_now, rtc_alarm_cb_t callback) {
    // Normally you'd here configure the RTC alarm hardware
    alarm_callback = callback;
    printf("RTC alarm set for %d seconds from now.\n", seconds_from_now);
}

// Simulation function to "fire" the alarm after `seconds_from_now`
void simulate_rtc_alarm() {
    // This would be automatically triggered by the RTC hardware
    alarm_callback(NULL);
}

// Callback function to be called when alarm is triggered
void alarm_triggered(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    // Initialize the RTC
    rtc_init();

    // Set the RTC date; note this is not implemented here for simplicity
    printf("Date set to June 1, 2024.\n");

    // Set the alarm for 5 seconds from now with our callback
    rtc_set_alarm(5, alarm_triggered);

    // Simulate waiting 5 seconds (this would normally not be needed)
    for (volatile int i = 0; i < 100000000; ++i);

    // Simulate the alarm being triggered
    simulate_rtc_alarm();

    return 0;
}
