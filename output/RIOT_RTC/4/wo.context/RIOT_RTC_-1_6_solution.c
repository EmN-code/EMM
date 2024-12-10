#include <stdio.h>
#include <stdint.h>
#include "esp_idf_rtc_gpio_pullup_en.h"
#include "esp_idf_rtc_gpio_pullup_dis.h"
#include "esp_idf_gpio_isr_handler_add.h"
#include "event_periodic_callback_init.h"
#include "driver/rtc_io.h"
#include "driver/rtc.h"
#include "driver/gpio.h"

#define ALARM_TIME 5

void IRAM_ATTR rtc_isr(void *arg) {
    printf("Alarm triggered!\n");
    rtc_hal_clear_alarm();
}

void configure_rtc_alarm() {
    rtc_hal_init();
    rtc_set_time(/* year, month, day, hour, min, sec */);
    rtc_set_alarm(ALARM_TIME);

    rtc_isr_register(rtc_isr, NULL);
    rtc_enable_alarm();
}

int main() {
    configure_rtc_alarm();
    while (1) {
        // Device remains idle or performs other tasks
        vTaskDelay(1000 / portTICK_PERIOD_MS); // sleep for 1 second
    }
    return 0;
}