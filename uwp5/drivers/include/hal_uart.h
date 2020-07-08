/*
 * Copyright (c) 2018, UNISOC Incorporated
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __HAL_UART_H
#define __HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#define MODE_UART	0
#define MODE_IRDA	1

#define MAX_TX_COUNT 0x7F
	enum {
		UART_RXF_FULL = 0,
		UART_TXF_EMPTY,
		UART_PARITY_ERR,
		UART_FRAME_ERR,
		UART_RXF_OVERRUN,
		UART_DSR_CHG,
		UART_CTS_CHG,
		UART_BRK_DTCT,
		UART_DSR,
		UART_CTS,
		UART_RTS,
		UART_RXD,
		UART_RXF_REALEMPTY,
		UART_TIME_OUT,
		UART_RXF_REALFULL,
		UART_TRANS_OVER,
	};

	typedef struct uart_txd {
		uint32_t txd	:8;
		uint32_t rsvd	:24;
	}txd_t;

	typedef struct uart_rxd {
		uint32_t rxd	:8;
		uint32_t rsvd	:24;
	}rxd_t;

	typedef struct uart_sts1 {
		uint32_t rxf_cnt		:7;
		uint32_t rsvd0			:1;
		uint32_t txf_cnt		:7;
		uint32_t rsvd1			:17;
	}sts1_t;

	typedef union uart_ctrl0 {
		uint32_t reg;

		struct {
			uint32_t odd_parity	:1;
			uint32_t parity_en		:1;
			uint32_t byte_len		:2;
			uint32_t stop_bit_num	:2;
			uint32_t rts_reg		:1;
			uint32_t send_brk_en	:1;
			uint32_t dtr_reg		:1;
			uint32_t ir_tx_iv		:1;
			uint32_t ir_rx_iv		:1;
			uint32_t ir_tx_en		:1;
			uint32_t ir_dplx		:1;
			uint32_t ir_wctl		:1;
			uint32_t rsvd0			:1;
			uint32_t mode_sel		:1;
			uint32_t rsvd1			:16;
		}bit;
	}ctrl0_t;

	typedef union uart_ctrl1 {
		uint32_t reg;

		struct {
			uint32_t rcv_hw_flow_thld	:7;
			uint32_t rcv_hw_flow_en	:1;
			uint32_t tx_hw_flow_en		:1;
			uint32_t rx_tout_thld		:5;
			uint32_t loop_back			:1;
			uint32_t dma_en			:1;
			uint32_t rsvd				:16;
		}bit;
	}ctrl1_t;

	typedef union uart_ctrl2 {
		uint32_t reg;
		struct {
			uint32_t rxf_full_thld		:7;
			uint32_t rsvd0				:1;
			uint32_t txf_empty_thld	:7;
			uint32_t rsvd1				:17;
		}bit;
	}ctrl2_t;

	typedef struct uart_cdk0 {
		uint32_t cdk0		:16;
		uint32_t rsvd		:16;
	}cdk0_t;

	typedef struct uart_dspwait {
		uint32_t dspwait			:4;
		uint32_t rx_dma_mod_sel	:1;
		uint32_t tx_dma_mod_sel	:1;
	}dspwait_t;

	struct uwp_uart {
		txd_t		txd;
		rxd_t		rxd;
		uint32_t		sts0;
		sts1_t		sts1;
		uint32_t		ien;
		uint32_t		iclr;
		ctrl0_t		ctrl0;
		ctrl1_t		ctrl1;
		ctrl2_t		ctrl2;
		cdk0_t		cdk0;
		uint32_t		sts2;
		dspwait_t	dspwait;
	};

static inline uint32_t uwp_uart_rx_ready(volatile struct uwp_uart *uart)
{
	return (uart->sts0 >> UART_RXF_FULL) & 0x1;
}

static inline uint32_t uwp_uart_tx_ready(volatile struct uwp_uart *uart)
{
	return (uart->sts0 >> UART_TXF_EMPTY) & 0x1;
}

static inline uint8_t uwp_uart_read(volatile struct uwp_uart *uart)
{
	return uart->rxd.rxd;
}

static inline void uwp_uart_write(volatile struct uwp_uart *uart,
		uint8_t c)
{
	uart->txd.txd = c;
}

static inline uint32_t uwp_uart_trans_over(volatile struct uwp_uart *uart)
{
	return uart->sts0 & BIT(UART_TRANS_OVER);
}

static inline void uwp_uart_set_cdk(volatile struct uwp_uart *uart,
		uint32_t cdk)
{
	uart->cdk0.cdk0 = cdk;
}

static inline void uwp_uart_set_stop_bit_num(volatile struct uwp_uart *uart,
		uint32_t stop_bit_num)
{
	uart->ctrl0.bit.stop_bit_num = stop_bit_num;
}

static inline void uwp_uart_set_byte_len(volatile struct uwp_uart *uart,
		uint32_t byte_len)
{
	uart->ctrl0.bit.byte_len = byte_len;
}

static inline void uwp_uart_init(volatile struct uwp_uart *uart)
{
	uart->ien = 0;
	uart->ctrl1.bit.rx_tout_thld = 1;
	uart->ctrl2.bit.rxf_full_thld = 1;
	uart->ctrl2.bit.txf_empty_thld = 64;
	/*for hardware flow control*/
	uart->ctrl1.bit.rcv_hw_flow_en = 1;
	uart->ctrl1.bit.rcv_hw_flow_thld = 50;
	uart->ctrl1.bit.tx_hw_flow_en = 1;
}

static inline void uwp_uart_int_enable(volatile struct uwp_uart *uart,
		uint32_t flags)
{
	uart->ien |= flags;
}

static inline void uwp_uart_int_disable(volatile struct uwp_uart *uart,
		uint32_t flags)
{
	uart->ien &= ~flags;
}

static inline void uwp_uart_int_clear(volatile struct uwp_uart *uart,
		uint32_t flags)
{
	uart->iclr |= flags;
}

static inline uint32_t uwp_uart_status(volatile struct uwp_uart *uart)
{
	return uart->sts0;
}

#ifdef __cplusplus
}
#endif

#endif
