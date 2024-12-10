#include "sys/socket.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "esp_system.h" // ESP32 specific header

#include "evtimer_mbox.h"  // Event timer with mbox
#include "evtimer_msg.h"   // Event timer with msg
#include "event/thread.h" // Thread handling for events
#include "netinet/in.h"   // Network functionalities
#include "arpa/inet.h"    // IP address conversion

#include "periph/gpio.h"  // GPIO handling
#include "stddef.h"       // Standard definitions
#include "stdatomic.h"    // Atomic operations
#include "stdbool.h"      // Boolean type
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mutex.h"

#include "unistd.h"
#include "net/emcute.h"
#include "periph/i2c.h"
#include "time_units.h"

#include "kernel_defines.h" // Kernel specifics

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t main_pid;

    // Initialize the event timer to use message-based events
    evtimer_init_msg(&evtimer);

    // Retrieve the current thread's PID
    main_pid = thread_getpid();

    // Initialize the event and configure the message
    event.msg.sender_pid = main_pid;
    evtimer_add(&evtimer, &event.event);

    // Periodic processing loop
    while (1) {
        // Here you would add code to handle the event when it's triggered
        // For example: printf("Event triggered\n");

        // Sleep or wait for the next event
    }

    return 0;
}