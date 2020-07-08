/*
 * Copyright (c) 2018, UNISOC Incorporated
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "uwp_hal.h"

#define GPIO_DIR_OUTPUT		1
#define GPIO_DIR_INPUT		0

#define GPIO_INT_DETECT_LEVEL	1
#define GPIO_INT_DETECT_EDGE	0

#define GPIO_TRIGGER_BOTH_EDGE		1
#define GPIO_TRIGGER_LEVEL_HIGH		2
#define GPIO_TRIGGER_LEVEL_LOW		3
#define GPIO_TRIGGER_HIGH_EDGE		4
#define GPIO_TRIGGER_LOW_EDGE		5

	struct uwp_gpio {
		uint32_t data;		/* data */
		uint32_t mask;		/* data mask */
		uint32_t dir;		/* data direction */
		uint32_t is;		/* interrupt sense */
		uint32_t ibe;		/* both edges interrup */
		uint32_t iev;		/* interrupt event */
		uint32_t ie;		/* interrupt enable */
		uint32_t ris;		/* raw interrupt status */
		uint32_t mis;		/* mask interrupt status */
		uint32_t ic;		/* interrupt clear */
		uint32_t inen;		/* input enable */
	};

#define UWP_GPIO(base) (volatile struct uwp_gpio *)base

	static inline void uwp_gpio_enable(uint32_t base,
			uint32_t pin_map)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		gpio->mask |= pin_map;
	}

	static inline void uwp_gpio_disable(uint32_t base,
			uint32_t pin_map)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		gpio->mask &= (~pin_map);
	}

	static inline void uwp_gpio_input_enable(uint32_t base,
			uint32_t pin_map)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		gpio->inen |= pin_map;
	}

	static inline void uwp_gpio_input_disable(uint32_t base,
			uint32_t pin_map)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		gpio->inen &= (~pin_map);
	}

	static inline void uwp_gpio_write(uint32_t base,
			uint32_t pin_map, uint32_t pin_value)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		if(pin_value)
			gpio->data |= pin_map;
		else
			gpio->data &= (~pin_map);
	}

	static inline uint32_t uwp_gpio_read(uint32_t base,
			uint32_t pin)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		return (gpio->data & pin);
	}

	static inline void uwp_gpio_set_dir(uint32_t base,
			uint32_t pin_map, uint32_t dir)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		if(dir == GPIO_DIR_OUTPUT)
			gpio->dir |= pin_map;
		else
			gpio->dir &= (~pin_map);
	}

	static inline void uwp_gpio_int_set_type(uint32_t base,
			uint32_t pin_map, uint32_t type)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		if (type == GPIO_TRIGGER_BOTH_EDGE) {
			gpio->is &= (~pin_map);
			gpio->ibe |= pin_map;
		} else if (type == GPIO_TRIGGER_HIGH_EDGE) {
			gpio->is &= (~pin_map);
			gpio->ibe &= (~pin_map);
			gpio->iev |= pin_map;
		} else if (type == GPIO_TRIGGER_LOW_EDGE) {
			gpio->is &= (~pin_map);
			gpio->ibe &= (~pin_map);
			gpio->iev &= (~pin_map);
		} else {
			gpio->is |= pin_map;
			gpio->ibe &= (~pin_map);

			if (type == GPIO_TRIGGER_LEVEL_HIGH)
				gpio->iev |= pin_map;
			else
				gpio->iev &= (~pin_map);
		}

	}

	static inline void uwp_gpio_int_enable(uint32_t base,
			uint32_t pin_map)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		gpio->ie |= pin_map;
	}

	static inline void uwp_gpio_int_disable(uint32_t base,
			uint32_t pin_map)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		gpio->ie &= (~pin_map);
	}

	static inline void uwp_gpio_int_clear(uint32_t base,
			uint32_t pin_map)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		gpio->ic |= pin_map;
	}

	static inline uint32_t uwp_gpio_int_status(uint32_t base, uint32_t bmask)
	{
		volatile struct uwp_gpio *gpio = UWP_GPIO(base);

		if(bmask)
			return gpio->mis;
		else
			return gpio->ris;
	}
#ifdef __cplusplus
}
#endif

#endif
