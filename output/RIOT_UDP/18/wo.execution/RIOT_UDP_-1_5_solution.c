```

#include "thread.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "sys/socket.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "MQTTClient.h"

static char _main_stack[THREAD_STACKSIZE_MAIN];
static bool device_state = true; // "work" is true, "not work" is false

/* Function that updates the device state */
static char* get_device_state() {
    return device_state ? "work" : "not work";
}

static void udp_sender_thread(void) {
    /* Initiate UDP socket */
    sock_udp_ep_t server = { .port = 20001 };
    inet_aton("47.102.103.1", &server.addr.ipv4);
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_ADDR_ANY_NETIF;
    int res = sock_udp_create(&sock, &local, NULL, 0);

    if (res != 0) {
        printf("Error creating UDP socket
");
        return;
    }

    while (1) {
        /* Send device state every 5 seconds */
        const char *msg = get_device_state();
        ssize_t res = sock_udp_send(&sock, msg, strlen(msg), &server);
        if (res <= 0) {
            printf("Error sending data: %d
", res);
        } else {
            printf("Sent: %s
", msg);
        }
        xtimer_sleep(5);
    }

    sock_udp_close(&sock);
}

int main(void) {
    /* Optional: Initialize WiFi for ESP32 (a placeholder since the exact init can vary) */
    esp_wifi_start();
    esp_system_init();

    /* Start the UDP sender thread */
    thread_create(_main_stack, sizeof(_main_stack), THREAD_PRIORITY_MAIN - 1, 0, udp_sender_thread, NULL, "udp_sender");

    return 0;
}
```