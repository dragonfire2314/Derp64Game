#include <ultra64.h>

#include "derp64.h"

u64 rdp_output[RDP_OUTPUT_LEN] __attribute__((aligned (16))); /* buffer for RDP DL */