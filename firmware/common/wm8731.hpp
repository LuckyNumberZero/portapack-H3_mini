/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __WM8731_H__
#define __WM8731_H__

#include <cstdint>

#include "i2c_pp.hpp"
#include "wm8731.hpp"

#include "utility.hpp"
#include "volume.hpp"

namespace wolfson {
namespace wm8731 {

constexpr volume_range_t headphone_gain_range { -121.0_dB, 6.0_dB };
constexpr volume_range_t line_in_gain_range { -34.5_dB, 12.0_dB };

enum class ADCSource {
	Line = 0,
	Microphone = 1,
};

using address_t = uint8_t;
using reg_t = uint16_t;

constexpr size_t reg_count = 10;

enum class Register : address_t {
	LeftLineIn = 0x00,
	RightLineIn = 0x01,
	LeftHeadphoneOut = 0x02,
	RightHeadphoneOut = 0x03,
	AnalogAudioPathControl = 0x04,
	DigitalAudioPathControl = 0x05,
	PowerDownControl = 0x06,
	DigitalAudioInterfaceFormat = 0x07,
	SamplingControl = 0x08,
	ActiveControl = 0x09,
};

struct LeftLineIn {
	reg_t linvol	: 5;
	reg_t reserved0	: 2;
	reg_t linmute	: 1;
	reg_t lrinboth	: 1;
	reg_t reserved1	: 7;
};

static_assert(sizeof(LeftLineIn) == sizeof(reg_t), "LeftLineIn type wrong size");

struct RightLineIn {
	reg_t rinvol	: 5;
	reg_t reserved0	: 2;
	reg_t rinmute	: 1;
	reg_t rlinboth	: 1;
	reg_t reserved1	: 7;
};

static_assert(sizeof(RightLineIn) == sizeof(reg_t), "RightLineIn type wrong size");

struct LeftHeadphoneOut {
	reg_t lhpvol	: 7;
	reg_t lzcen		: 1;
	reg_t lrhpboth	: 1;
	reg_t reserved0	: 7;
};

static_assert(sizeof(LeftHeadphoneOut) == sizeof(reg_t), "LeftHeadphoneOut type wrong size");

struct RightHeadphoneOut {
	reg_t rhpvol	: 7;
	reg_t rzcen		: 1;
	reg_t rlhpboth	: 1;
	reg_t reserved0	: 7;
};

static_assert(sizeof(RightHeadphoneOut) == sizeof(reg_t), "RightHeadphoneOut type wrong size");

struct AnalogAudioPathControl {
	reg_t micboost	: 1;
	reg_t mutemic	: 1;
	reg_t insel		: 1;
	reg_t bypass	: 1;
	reg_t dacsel	: 1;
	reg_t sidetone	: 1;
	reg_t sideatt	: 2;
	reg_t reserved0	: 8;
};

static_assert(sizeof(AnalogAudioPathControl) == sizeof(reg_t), "AnalogAudioPathControl type wrong size");

struct DigitalAudioPathControl {
	reg_t adchpd	: 1;
	reg_t deemp		: 2;
	reg_t dacmu		: 1;
	reg_t hpor		: 1;
	reg_t reserved0	: 11;
};

static_assert(sizeof(DigitalAudioPathControl) == sizeof(reg_t), "DigitalAudioPathControl type wrong size");

struct PowerDownControl {
	reg_t lineinpd	: 1;
	reg_t micpd		: 1;
	reg_t adcpd		: 1;
	reg_t dacpd		: 1;
	reg_t outpd		: 1;
	reg_t oscpd		: 1;
	reg_t clkoutpd	: 1;
	reg_t poweroff	: 1;
	reg_t reserved0	: 8;
};

static_assert(sizeof(PowerDownControl) == sizeof(reg_t), "PowerDownControl type wrong size");

struct DigitalAudioInterfaceFormat {
	reg_t format	: 2;
	reg_t iwl		: 2;
	reg_t lrp		: 1;
	reg_t lrswap	: 1;
	reg_t ms		: 1;
	reg_t bclkinv	: 1;
	reg_t reserved0	: 8;
};

static_assert(sizeof(DigitalAudioInterfaceFormat) == sizeof(reg_t), "DigitalAudioInterfaceFormat type wrong size");

struct SamplingControl {
	reg_t usb_normal	: 1;
	reg_t bosr			: 1;
	reg_t sr			: 4;
	reg_t clkidiv2		: 1;
	reg_t clkodiv2		: 1;
	reg_t reserved0		: 8;
};

static_assert(sizeof(SamplingControl) == sizeof(reg_t), "SamplingControl type wrong size");

struct ActiveControl {
	reg_t active	: 1;
	reg_t reserved0	: 15;
};

static_assert(sizeof(ActiveControl) == sizeof(reg_t), "ActiveControl type wrong size");

struct Reset {
	reg_t reset		: 9;
	reg_t reserved0	: 7;
};

static_assert(sizeof(Reset) == sizeof(reg_t), "Reset type wrong size");

struct Register_Type {
	LeftLineIn					left_line_in;
	RightLineIn					right_line_in;
	LeftHeadphoneOut			left_headphone_out;
	RightHeadphoneOut			right_headphone_out;
	AnalogAudioPathControl		analog_audio_path_control;
	DigitalAudioPathControl		digital_audio_path_control;
	PowerDownControl			power_down_control;
	DigitalAudioInterfaceFormat	digital_audio_interface_format;
	SamplingControl				sampling_control;
	ActiveControl				active_control;
};

static_assert(sizeof(Register_Type) == reg_count * sizeof(reg_t), "Register_Type wrong size");

struct RegisterMap {
	constexpr RegisterMap(
		Register_Type values
	) : r(values)
	{
	}

	union {
		Register_Type r;
		std::array<reg_t, reg_count> w;
	};
};

static_assert(sizeof(RegisterMap) == reg_count * sizeof(reg_t), "RegisterMap type wrong size");

constexpr RegisterMap default_after_reset { Register_Type {
	.left_line_in = {
		.linvol		= 0b10111,
		.reserved0	= 0b00,
		.linmute	= 0b1,
		.lrinboth	= 0b0,
		.reserved1 	= 0,
	},
	.right_line_in = {
		.rinvol		= 0b10111,
		.reserved0	= 0b00,
		.rinmute	= 0b1,
		.rlinboth	= 0b0,
		.reserved1 	= 0,
	},
	.left_headphone_out = {
		.lhpvol		= 0b1111001,
		.lzcen		= 0b0,
		.lrhpboth	= 0b0,
		.reserved0	= 0,
	},
	.right_headphone_out = {
		.rhpvol		= 0b1111001,
		.rzcen		= 0b0,
		.rlhpboth	= 0b0,
		.reserved0	= 0,
	},
	.analog_audio_path_control = {
		.micboost	= 0b0,
		.mutemic	= 0b1,
		.insel		= 0b0,
		.bypass		= 0b1,
		.dacsel		= 0b0,
		.sidetone	= 0b0,
		.sideatt	= 0b00,
		.reserved0	= 0,
	},
	.digital_audio_path_control = {
		.adchpd		= 0b0,
		.deemp		= 0b00,
		.dacmu		= 0b1,
		.hpor		= 0b0,
		.reserved0	= 0,
	},
	.power_down_control = {
		.lineinpd	= 0b1,
		.micpd		= 0b1,
		.adcpd		= 0b1,
		.dacpd		= 0b1,
		.outpd		= 0b1,
		.oscpd		= 0b0,
		.clkoutpd	= 0b0,
		.poweroff	= 0b1,
		.reserved0	= 0,
	},
	.digital_audio_interface_format = {
		.format		= 0b10,
		.iwl		= 0b10,
		.lrp		= 0b0,
		.lrswap		= 0b0,
		.ms			= 0b0,
		.bclkinv	= 0b0,
		.reserved0	= 0,
	},
	.sampling_control = {
		.usb_normal	= 0b0,
		.bosr		= 0b0,
		.sr			= 0b0000,
		.clkidiv2	= 0b0,
		.clkodiv2	= 0b0,
		.reserved0	= 0,
	},
	.active_control = {
		.active		= 0b0,
		.reserved0	= 0,
	},
} };

class WM8731 {
public:
	constexpr WM8731(
		I2C& bus,
		const I2C::address_t bus_address
	) : bus(bus),
		bus_address(bus_address)
	{
	}

	void init() {
		reset();

		write(PowerDownControl {
			.lineinpd = 1,
			.micpd = 0,
			.adcpd = 0,
			.dacpd = 0,
			.outpd = 0,
			.oscpd = 1,
			.clkoutpd = 1,
			.poweroff = 0,
			.reserved0 = 0,
		});

		// write(SamplingControl {
		// 	.usb_normal = 0,
		// 	.bosr = 0,
		// 	.sr = 0,
		// 	.clkidiv2 = 0,
		// 	.clkodiv2 = 0,
		// 	.reserved0 = 0,
		// });

		write(DigitalAudioInterfaceFormat {
			.format = 2,
			.iwl = 0,
			.lrp = 0,
			.lrswap = 0,
			.ms = 0,
			.bclkinv = 0,
			.reserved0 = 0,
		});

		write(DigitalAudioPathControl {
			.adchpd = 0,
			.deemp = 0,
			.dacmu = 0,
			.hpor = 0,
			.reserved0 = 0,
		});

		write(AnalogAudioPathControl {
			.micboost = 1,	// Enable 20dB boost
			.mutemic = 0,	// Disable mute (unmute)
			.insel = 1,		// Microphone input to ADC
			.bypass = 0,
			.dacsel = 1,
			.sidetone = 0,
			.sideatt = 0,
			.reserved0 = 0,
		});

		write(ActiveControl {
			.active = 1,
			.reserved0 = 0,
		});

		set_line_in_volume(0.0_dB);
		headphone_mute();
	}

	void reset() {
		write(0x0f, 0);
	}

	void set_line_in_volume(const volume_t volume) {
		const auto normalized = line_in_gain_range.normalize(volume);
		auto n = normalized.centibel() / 15;

		write(LeftLineIn {
			.linvol = static_cast<reg_t>(n),
			.reserved0 = 0,
			.linmute = 0,
			.lrinboth = 1,
			.reserved1 = 0,
		});
	}

	void set_headphone_volume(const volume_t volume) {
		const auto normalized = headphone_gain_range.normalize(volume);
		auto n = normalized.centibel() / 10;

		write(LeftHeadphoneOut {
			.lhpvol = static_cast<reg_t>(n),
			.lzcen = 1,
			.lrhpboth = 1,
			.reserved0 = 0,
		});
	}

	void headphone_mute() {
		set_headphone_volume(headphone_gain_range.min);
	}

	// void microphone_mute(const bool mute) {
	// 	map.r.analog_audio_path_control.mutemic = (mute ? 0 : 1);
	// 	write(Register::AnalogAudioPathControl);
	// }

	// void set_adc_source(const ADCSource adc_source) {
	// 	map.r.analog_audio_path_control.insel = toUType(adc_source);
	// 	write(Register::AnalogAudioPathControl);
	// }

private:
	I2C& bus;
	const I2C::address_t bus_address;
	RegisterMap map { default_after_reset };

	void write(const Register reg) {
		write(toUType(reg), map.w[toUType(reg)]);
	}

	void write(const address_t reg_address, const reg_t value);

	void write(const LeftLineIn value) {
		map.r.left_line_in = value;
		write(Register::LeftLineIn);
	}

	void write(const RightLineIn value) {
		map.r.right_line_in = value;
		write(Register::RightLineIn);
	}

	void write(const LeftHeadphoneOut value) {
		map.r.left_headphone_out = value;
		write(Register::LeftHeadphoneOut);
	}

	void write(const RightHeadphoneOut value) {
		map.r.right_headphone_out = value;
		write(Register::RightHeadphoneOut);
	}

	void write(const AnalogAudioPathControl value) {
		map.r.analog_audio_path_control = value;
		write(Register::AnalogAudioPathControl);
	}

	void write(const DigitalAudioPathControl value) {
		map.r.digital_audio_path_control = value;
		write(Register::DigitalAudioPathControl);
	}

	void write(const PowerDownControl value) {
		map.r.power_down_control = value;
		write(Register::PowerDownControl);
	}

	void write(const DigitalAudioInterfaceFormat value) {
		map.r.digital_audio_interface_format = value;
		write(Register::DigitalAudioInterfaceFormat);
	}

	void write(const SamplingControl value) {
		map.r.sampling_control = value;
		write(Register::SamplingControl);
	}

	void write(const ActiveControl value) {
		map.r.active_control = value;
		write(Register::ActiveControl);
	}
};

} /* namespace wm8731 */
} /* namespace wolfson */

#endif/*__WM8731_H__*/