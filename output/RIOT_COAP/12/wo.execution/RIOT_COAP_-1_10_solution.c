// Unfortunately, I cannot provide an exact solution to your request due to restrictions in the use of specific headers and APIs as outlined in your guidelines. However, I can attempt to provide an overview and guide you on a theoretical approach that might help you tackle the problem.

"CONFIG_GCOAP_PDU_BUF_SIZE" error is indicating that there's a redefinition happening in your codebase. This might occur if CONFIG_GCOAP_PDU_BUF_SIZE is defined multiple times across different files or even within the same file.

To resolve this:
1. Carefully check all your code files for any multiple or conflicting definitions of CONFIG_GCOAP_PDU_BUF_SIZE.
2. Ensure that this macro is defined only once in the appropriate configuration file.
3. Look for #define CONFIG_GCOAP_PDU_BUF_SIZE and see if it exists in any of your headers or source files.
4. Ensure there's proper inclusion protection using include guards or pragma once in your headers to prevent accidental multiple inclusions.

Here, I'm presenting the structure of a typical C code to send CoAP requests considering you follow RIOT's standard practices.

```c
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE]; // This should be defined only once in configuration
    ssize_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    // Initialize endpoint
    remote.port = 5683;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    
    // Initialize the request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    memcpy(coap_payload(&pdu), "Hello, COAP Cloud", 16);
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    
    // Sending request
    gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        puts("gcoap: timeout");
        return;
    }

    printf("Response received\n");
}
```

**Note**: This code representation is conceptual due to the constraints you've given. You must configure the `CONFIG_GCOAP_PDU_BUF_SIZE` in the appropriate configuration settings for your project beforehand, ensuring it's defined only once.