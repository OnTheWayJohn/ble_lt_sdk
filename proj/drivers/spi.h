
#pragma once

#include "../common/types.h"

void spi_write(u8 d,u8 ncs);
u8 spi_read(u8 ncs);

typedef void (*spi_callback_func)(u8 *);


