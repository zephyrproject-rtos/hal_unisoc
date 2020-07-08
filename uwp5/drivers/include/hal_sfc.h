/*
 * Copyright (c) 2018, UNISOC Incorporated
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MARLIN3_HAL_SFC_H
#define __MARLIN3_HAL_SFC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>
#include <arch/arm/aarch32/exc.h>
#include <irq.h>

#include "uwp_hal.h"

	typedef struct SPIFLASH_ExtCfg {
		int voltage;
		uint32_t desity;
		uint32_t reserved1;
		uint32_t reserved2;
		uint32_t reserved3;
		uint32_t reserved4;
		uint32_t reserved5;
	} *Spiflash_ExtCfg_PRT;

	typedef struct nor_flash_config_s {
		uint32_t bank_num;
		uint32_t sect_num;
		uint32_t file_sect_num;
		uint32_t sect_size;
		uint32_t start_addr;
		uint32_t efs_start_addr;
		uint32_t flash_size;
		uint32_t fixnv_addr;
		uint32_t prodinfo_addr;
		uint32_t mmi_res;
		uint32_t umem_addr;
		uint32_t umem_size;
		uint32_t spload_addr;
		uint32_t ps_addr;
	} NOR_FLASH_CONFIG_T, *NOR_FLASH_CONFIG_PTR;

	typedef struct DFILE_CONFIG_Tag {
		uint32_t magic_first;
		uint32_t magic_second;
		uint32_t image_addr;
		uint32_t res_addr;
		uint32_t nv_addr;
		uint32_t dsp_addr;
		uint32_t reserved2;
		uint32_t ext[24];
		uint32_t magic_end;
	} DFILE_CONFIG_T;

	struct spi_flash_region {
		unsigned int count;
		unsigned int size;
	};

	typedef enum READ_CMD_TYPE_E_TAG {
		READ_SPI = 0,
		READ_SPI_FAST,
		READ_SPI_2IO,
		READ_SPI_4IO,
		READ_QPI_FAST,
		READ_QPI_4IO,
	} READ_CMD_TYPE_E;

	struct spi_flash {
		uint32_t cs;

		const char *name;
		uint32_t size;
		uint32_t page_size;
		uint32_t sector_size;
		uint32_t dummy_bytes;
		uint8_t work_mode;
		uint8_t support_4addr;
		int spi_rw_mode;

		int (*read_noxip) (struct spi_flash * flash, uint32_t address,
				uint8_t * buf, uint32_t buf_size, READ_CMD_TYPE_E type);
		int (*read) (struct spi_flash * flash, uint32_t offset, uint32_t * buf,
				uint32_t dump_len, READ_CMD_TYPE_E type);
		int (*write) (struct spi_flash * flash, uint32_t offset, uint32_t len,
				const void *buf);
		int (*read_sec_noxip) (struct spi_flash * flash, uint8_t * buf,
				uint32_t buf_size, READ_CMD_TYPE_E type);
		int (*read_sec) (struct spi_flash * flash, uint32_t offset, uint32_t * buf,
				uint32_t dump_len, READ_CMD_TYPE_E type);
		int (*write_sec) (struct spi_flash * flash, uint32_t offset, uint32_t len,
				const void *buf);
		int (*erase) (struct spi_flash * flash, uint32_t offset, uint32_t len);
		int (*erase_chip) (struct spi_flash * flash);

		int (*reset) (void);
		int (*suspend) (struct spi_flash * flash);
		int (*resume) (struct spi_flash * flash);
		int (*wren)(struct spi_flash *flash);
		int (*lock) (struct spi_flash * flash, uint32_t offset, uint32_t len);
		int (*unlock) (struct spi_flash * flash, uint32_t offset, uint32_t len);
		int (*set_4io) (struct spi_flash * flash, uint32_t op);
		int (*set_qpi) (struct spi_flash * flash, uint32_t op);

		int (*set_encrypt) (uint32_t op);

		void *priv;
	};

	struct spi_flash_spec_s {
		uint16_t id_manufacturer;
		uint16_t table_num;
		struct spi_flash_params *table;
	};

	struct spi_flash_params {
		uint16_t idcode1;
		uint16_t idcode2;
		uint16_t page_size;
		uint16_t sector_size;
		uint16_t nr_sectors;
		uint16_t nr_blocks;
		uint16_t support_qpi;
		uint16_t read_freq_max;
		uint16_t dummy_clocks;
		const char *name;
	};

	struct spi_flash_struct {
		struct spi_flash flash;
		const struct spi_flash_params *params;
	};

	void uwp_spi_xip_init(void);

	__ramfunc void spiflash_select_xip(uint32_t op);

	__ramfunc void spiflash_set_clk(void);

	__ramfunc int uwp_spi_flash_init(struct spi_flash *flash,
			struct spi_flash_params **params);

	void spi_flash_free(struct spi_flash *flash);

	void uwp_spi_dump(uint32_t arg_in);

static ALWAYS_INLINE unsigned int irq_lock_primask(void)
{
	unsigned int key;

	__asm__ volatile("mrs %0, PRIMASK;"
		"cpsid i"
		: "=r" (key)
		:
		: "memory");

	return key;
}

static ALWAYS_INLINE void irq_unlock_primask(unsigned int key)
{
	if (key) {
		return;
	}
	__asm__ volatile("cpsie i" : : : "memory");
}

#ifdef __cplusplus
}
#endif

#endif
