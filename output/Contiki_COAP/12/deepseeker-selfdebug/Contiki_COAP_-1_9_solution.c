
#include <stdio.h>
#include "coap-blocking-request.h"
#include "net/ipv6/addr.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"
#include "net/rpl/rpl-timers.h"
#include "net/rpl/rpl-dag.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/rpl/rpl-mrhof.h"
#include "net/rpl/rpl-of0.h"
#include "net/rpl/rpl-tsch.h"
#include "net/rpl/rpl-icmp6.h"
#include "net/rpl/rpl-ext-header.h"
#include "net/