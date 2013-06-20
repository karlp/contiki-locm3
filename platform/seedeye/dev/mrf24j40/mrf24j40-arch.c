/*
 * Copyright (c) 2013, Karl Palsson <karlp@tweak.net.au>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \file   mrf24j40-arch.c
 * 
 * \brief  MRF24J40 arch driver for seedeye platform
 * \author Karl Palsson <karlp@tweak.net.au>
 */

#include "dev/mrf24j40.h"
#include "mrf24j40-arch.h"

#include <pic32_spi.h>
#include <pic32_irq.h>

void mrf24j40_arch_wake_pin(int val)
{
	MRF24J40_WAKE = val;
}

/**
 * @param val logical value for asserting reset.
 * FIXME - make this proper doxygen style!
 */
void mrf24j40_arch_hard_reset(int val)
{
	if (val) {
		MRF24J40_HARDRESET_LOW();
	} else {
		MRF24J40_HARDRESET_HIGH();
	}
}

int mrf24j40_arch_init(void)
{
	/* Set the IO pins direction */
	MRF24J40_PINDIRECTION_INIT();

	/* Set interrupt registers and reset flags */
	MRF24J40_INTERRUPT_INIT(6, 3);

	int rc = MRF24J40_SPI_PORT_INIT(10000000, SPI_DEFAULT);
	if (rc < 0) {
		return rc;
	}
	return 0;
}

int mrf24j40_arch_is_irq_enabled(void)
{
	return MRF24J40_INTERRUPT_ENABLE_STAT();
}

void mrf24j40_arch_irq_enable(void)
{
	MRF24J40_INTERRUPT_ENABLE_SET();
}

void mrf24j40_arch_irq_disable(void)
{
	MRF24J40_INTERRUPT_ENABLE_CLR();
}

void mrf24j40_arch_select(void)
{
	MRF24J40_CSn_LOW();
}

void mrf24j40_arch_deselect(void)
{
	MRF24J40_CSn_HIGH();
}

MRF24J40_ISR()
{
	mrf24j40_irq_handler();
	MRF24J40_INTERRUPT_FLAG_CLR();
}

void mrf24j40_arch_spi_write(uint8_t *data, int length) {
	  MRF24J40_SPI_PORT_WRITE(data, length);
}

void mrf24j40_arch_spi_read(uint8_t *data, int length) {
	  MRF24J40_SPI_PORT_READ(data, length);
}