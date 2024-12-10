The following solution addresses the issue by ensuring the function call has the correct argument types for `gcoap_req_send`, as required by RIOT's network APIs. The error indicates that the fourth argument is incompatible due to type mismatch. We'll ensure the correct type for `gcoap_resp_handler_t` is used in the request.

We will include the required header files and create a simple CoAP POST request to the provided server.

Imports:
- `assert.h` for error checking.
- `arpa/inet.h` for IP address manipulation.
- `net/gcoap.h` for CoAP functionalities.
- `net/sock/udp.h` for UDP networking.

Code:

```c
#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "assert.h"
#include "arpa/inet.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; /* Ignore unused parameter warning */
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout");
        return -1;
    }
    puts("Response received");
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = EMQX_PORT;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    assert(result == 1);

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    assert(len != 0);

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    result = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    assert(result >= 0);

    // Main loop to keep the program running
    while (1) {
        // The program can conduct other tasks here, if needed
        // Delay or yield can also be included to avoid busy waiting
    }
}
```

This code sets up a CoAP POST request with the payload "Hello, COAP Cloud" to be sent to the server at the specified IP address and port. The payload is loaded into the PDU and sent over a UDP connection using the RIOT operating system's GCoAP interface. The response handler `_resp_handler` checks if a timeout occurred or if a response has been received.