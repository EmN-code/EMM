
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-debug.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-ds6-ext.h"
#include "net/ipv6/uip-ds6-ext-route.h"
#include "net/ipv6/uip-ds6-ext-nbr.h"
#include "net/ipv6/uip-ds6-ext-addr.h"
#include "net/ipv6/uip-ds6-ext-debug.h"
#include "net/ipv6/uip-ds6-ext-route-debug.h"
#include "net/ipv6/uip-ds6-ext-nbr-debug.h"
#include "net/ipv6/uip-ds6-ext-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug.h"
#include "net/ipv6/uip-ds6-ext-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route-nbr-addr-debug-route.h"
#include "ne