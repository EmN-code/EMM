#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/cord_ep.h>
#include <net/coap.h>
#include <net/netutils.h>
#include <xtimer.h>
#include <thread.h>

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define SENDER_STACK_SIZE (THREAD_STACKSIZE_MAIN / 2)

static char sender_stack[SENDER_STACK_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote;
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    char addr_str[IPV6_ADDR_MAX_STR_LEN];

    /* parse destination address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_ADDR) == NULL) {
        puts("Error: unable to parse destination address");
        return NULL;
    }
    remote.port = COAP_SERVER_PORT;

    uint8_t buf[128];
    coap_pkt_t pdu;
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_TYPE_CON);
    coap_pkt_set_path(&pdu, COAP_URI_PATH);
    coap_hdr_set_code(&pdu, COAP_CODE_201);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE));
    pdu.payload_len = sizeof(MESSAGE);

    while (1) {
        ssize_t res = sock_udp_send(NULL, &pdu.hdr, pdu.hdr_len + pdu.payload_len, &remote);
        if (res < 0) {
            printf("CoAP request error: %d\n", res);
        } else {
            puts("CoAP POST request sent.");
        }
        xtimer_sleep(5);  // send every 5 seconds
    }

    return NULL;
}

int main(void) {
    puts("RIOT CoAP client application\n");

    /* start the CoAP sender thread */
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender_thread, NULL, "sender");

    /* main thread spins to keep the program running */
    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}
