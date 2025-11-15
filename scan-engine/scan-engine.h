#pragma once
#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-network/shiba-network.h"
#include <string.h>

// These functions will handle collecting responses and doing the general
// scanning portion needed for smap, as well as IP address validation,
// including the type of ip address passed in

/* validates an ip, whether v4 or v6. returns 4 on IPv4 success,
 * 6 on IPv6 success, or 1 if invalid */
extern int is_valid_ip(const char* target);
