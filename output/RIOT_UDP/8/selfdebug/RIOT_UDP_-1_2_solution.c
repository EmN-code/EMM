# Prefix: Corrected RIOT program to send device state to a UDP server every 5 seconds

## Description
This program initializes a UDP connection and sends a message to a specified server every 5 seconds to report whether the device is in a working state or not. The previous errors included missing braces, incorrect function calls, and malformed strings which are now corrected.

## Imports
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "net/af.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"
```

## Code Block
```c
define SERVER_IP   "47.102.103.1"
define SERVER_PORT 20001
define WORKING_STATE_MSG "Device is working"
define NOT_WORKING_STATE_MSG "Device is not working"

#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

static bool is_working = true;  // example device state

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char *msg = is_working ? WORKING_STATE_MSG : NOT_WORKING_STATE_MSG;
        ssize_t res = sock_udp_send(&sock, msg, strlen(msg), &server);

        if (res < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Sent: %s\n", msg);
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    sock_udp_close(&sock);
    return 0;
}
```

### Explanation
1. **Header Files:** The inclusion of necessary header files such as `net/sock/udp.h`, `net/ipv6/addr.h`, and `xtimer.h`, allows us to create a UDP socket, handle IP address conversions, and set timers for periodic message sending.
2. **UDP Socket Setup:** Initializes a UDP socket to the specified server IP and port using the `sock_udp_create()` function.
3. **Loop for Sending Messages:** The server message sends the device's working status every 5 seconds using `sock_udp_send()`, reporting success or an error message depending on the result.
4. **Timing:** Utilizes RIOT's `xtimer` capabilities to pause execution between sends, achieving periodic transmission.