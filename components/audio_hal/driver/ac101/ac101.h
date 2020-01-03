/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __AC101_H__
#define __AC101_H__

#include "esp_types.h"
#include "audio_hal.h"
#include "driver/i2c.h"
#include "esxxx_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* AC101 address */
#define AC101_ADDR 0x1a  /*!< 0x22:CE=1;0x20:CE=0*/

/* ES8388 register */
#define ES8388_CONTROL1         0x00
#define ES8388_CONTROL2         0x01

#define ES8388_CHIPPOWER        0x02

#define ES8388_ADCPOWER         0x03
#define ES8388_DACPOWER         0x04

#define ES8388_CHIPLOPOW1       0x05
#define ES8388_CHIPLOPOW2       0x06

#define ES8388_ANAVOLMANAG      0x07

#define ES8388_MASTERMODE       0x08
/* ADC */
#define ES8388_ADCCONTROL1      0x09
#define ES8388_ADCCONTROL2      0x0a
#define ES8388_ADCCONTROL3      0x0b
#define ES8388_ADCCONTROL4      0x0c
#define ES8388_ADCCONTROL5      0x0d
#define ES8388_ADCCONTROL6      0x0e
#define ES8388_ADCCONTROL7      0x0f
#define ES8388_ADCCONTROL8      0x10
#define ES8388_ADCCONTROL9      0x11
#define ES8388_ADCCONTROL10     0x12
#define ES8388_ADCCONTROL11     0x13
#define ES8388_ADCCONTROL12     0x14
#define ES8388_ADCCONTROL13     0x15
#define ES8388_ADCCONTROL14     0x16
/* DAC */
#define ES8388_DACCONTROL1      0x17
#define ES8388_DACCONTROL2      0x18
#define ES8388_DACCONTROL3      0x19
#define ES8388_DACCONTROL4      0x1a
#define ES8388_DACCONTROL5      0x1b
#define ES8388_DACCONTROL6      0x1c
#define ES8388_DACCONTROL7      0x1d
#define ES8388_DACCONTROL8      0x1e
#define ES8388_DACCONTROL9      0x1f
#define ES8388_DACCONTROL10     0x20
#define ES8388_DACCONTROL11     0x21
#define ES8388_DACCONTROL12     0x22
#define ES8388_DACCONTROL13     0x23
#define ES8388_DACCONTROL14     0x24
#define ES8388_DACCONTROL15     0x25
#define ES8388_DACCONTROL16     0x26
#define ES8388_DACCONTROL17     0x27
#define ES8388_DACCONTROL18     0x28
#define ES8388_DACCONTROL19     0x29
#define ES8388_DACCONTROL20     0x2a
#define ES8388_DACCONTROL21     0x2b
#define ES8388_DACCONTROL22     0x2c
#define ES8388_DACCONTROL23     0x2d
#define ES8388_DACCONTROL24     0x2e
#define ES8388_DACCONTROL25     0x2f
#define ES8388_DACCONTROL26     0x30
#define ES8388_DACCONTROL27     0x31
#define ES8388_DACCONTROL28     0x32
#define ES8388_DACCONTROL29     0x33
#define ES8388_DACCONTROL30     0x34

#define CHIP_AUDIO_RS		0x00
#define PLL_CTRL1			0x01
#define PLL_CTRL2			0x02
#define SYSCLK_CTRL			0x03
#define MOD_CLK_ENA			0x04
#define MOD_RST_CTRL		0x05
#define I2S_SR_CTRL			0x06
#define I2S1LCK_CTRL		0x10
#define I2S1_SDOUT_CTRL		0x11
#define I2S1_SDIN_CTRL		0x12
#define I2S1_MXR_SRC		0x13
#define I2S1_VOL_CTRL1		0x14
#define I2S1_VOL_CTRL2		0x15
#define I2S1_VOL_CTRL3		0x16
#define I2S1_VOL_CTRL4		0x17
#define I2S1_MXR_GAIN		0x18
#define ADC_DIG_CTRL		0x40
#define ADC_VOL_CTRL		0x41
#define HMIC_CTRL1			0x44
#define HMIC_CTRL2			0x45
#define HMIC_STATUS			0x46
#define DAC_DIG_CTRL		0x48
#define DAC_VOL_CTRL		0x49
#define DAC_MXR_SRC			0x4c
#define DAC_MXR_GAIN		0x4d
#define ADC_APC_CTRL		0x50
#define ADC_SRC				0x51
#define ADC_SRCBST_CTRL		0x52
#define OMIXER_DACA_CTRL	0x53
#define OMIXER_SR			0x54
#define OMIXER_BST1_CTRL	0x55
#define HPOUT_CTRL			0x56
#define SPKOUT_CTRL			0x58
#define AC_DAC_DAPCTRL		0xa0
#define AC_DAC_DAPHHPFC 	0xa1
#define AC_DAC_DAPLHPFC 	0xa2
#define AC_DAC_DAPLHAVC 	0xa3
#define AC_DAC_DAPLLAVC 	0xa4
#define AC_DAC_DAPRHAVC 	0xa5
#define AC_DAC_DAPRLAVC 	0xa6
#define AC_DAC_DAPHGDEC 	0xa7
#define AC_DAC_DAPLGDEC 	0xa8
#define AC_DAC_DAPHGATC 	0xa9
#define AC_DAC_DAPLGATC 	0xaa
#define AC_DAC_DAPHETHD 	0xab
#define AC_DAC_DAPLETHD 	0xac
#define AC_DAC_DAPHGKPA 	0xad
#define AC_DAC_DAPLGKPA 	0xae
#define AC_DAC_DAPHGOPA 	0xaf
#define AC_DAC_DAPLGOPA 	0xb0
#define AC_DAC_DAPOPT   	0xb1
#define DAC_DAP_ENA     	0xb5

typedef enum{
	SIMPLE_RATE_8000	= 0x0000,
	SIMPLE_RATE_11052	= 0x1000,
	SIMPLE_RATE_12000	= 0x2000,
	SIMPLE_RATE_16000	= 0x3000,
	SIMPLE_RATE_22050	= 0x4000,
	SIMPLE_RATE_24000	= 0x5000,
	SIMPLE_RATE_32000	= 0x6000,
	SIMPLE_RATE_44100	= 0x7000,
	SIMPLE_RATE_48000	= 0x8000,
	SIMPLE_RATE_96000	= 0x9000,
	SIMPLE_RATE_192000	= 0xa000,
} ac101_adda_fs_i2s1_t;

typedef enum{
	BCLK_DIV_1		= 0x0,
	BCLK_DIV_2		= 0x1,
	BCLK_DIV_4		= 0x2,
	BCLK_DIV_6		= 0x3,
	BCLK_DIV_8		= 0x4,
	BCLK_DIV_12		= 0x5,
	BCLK_DIV_16		= 0x6,
	BCLK_DIV_24		= 0x7,
	BCLK_DIV_32		= 0x8,
	BCLK_DIV_48		= 0x9,
	BCLK_DIV_64		= 0xa,
	BCLK_DIV_96		= 0xb,
	BCLK_DIV_128	= 0xc,
	BCLK_DIV_192	= 0xd,

} ac101_i2s1_bclk_div_t;

typedef enum{
	LRCK_DIV_16		=0x0,
	LRCK_DIV_32		=0x1,
	LRCK_DIV_64		=0x2,
	LRCK_DIV_128	=0x3,
	LRCK_DIV_256	=0x4,
} ac101_i2s1_lrck_div_t;

typedef enum {
    BIT_LENGTH_8_BITS = 0x00,
    BIT_LENGTH_16_BITS = 0x01,
    BIT_LENGTH_20_BITS = 0x02,
    BIT_LENGTH_24_BITS = 0x03,
} ac101_bits_length_t;

typedef enum {
    AC_MODE_MIN = -1,
    AC_MODE_SLAVE = 0x00,
    AC_MODE_MASTER = 0x01,
    AC_MODE_MAX,
} ac101_mode_sm_t;

typedef enum {
    AC101_MODULE_MIN = -1,
    AC101_MODULE_ADC = 0x01,
    AC101_MODULE_DAC = 0x02,
    AC101_MODULE_ADC_DAC = 0x03,
    AC101_MODULE_LINE = 0x04,
    AC101_MODULE_MAX
} ac101_module_t;

typedef enum{
	SRC_MIC1	= 1,
	SRC_MIC2	= 2,
	SRC_LINEIN	= 3,
} ac101_output_mixer_source_t;

typedef enum {
    GAIN_N45DB = 0,
    GAIN_N30DB = 1,
    GAIN_N15DB = 2,
    GAIN_0DB   = 3,
    GAIN_15DB  = 4,
    GAIN_30DB  = 5,
    GAIN_45DB  = 6,
    GAIN_60DB  = 7,
} ac101_output_mixer_gain_t;

/**
 * @brief Configure AC101 clock
 */
typedef struct {
	ac101_i2s1_bclk_div_t bclk_div;    /*!< bits clock divide */
	ac101_i2s1_lrck_div_t lclk_div;    /*!< WS clock divide */
} ac101_i2s_clock_t;

static int ac101_set_adc_dac_volume(int mode, int volume, int dot);
static esp_err_t i2c_example_master_read_slave(uint8_t DevAddr, uint8_t reg,uint8_t* data_rd, size_t size);
esp_err_t ac101_init(audio_hal_codec_config_t *cfg);
esp_err_t ac101_deinit(void);
esp_err_t ac101_config_fmt(ac101_module_t mod, es_i2s_fmt_t cfg);
esp_err_t ac101_i2s_config_clock(ac101_i2s_clock_t cfg);
esp_err_t ac101_set_bits_per_sample(ac101_module_t mode, es_bits_length_t bit_per_sample);
esp_err_t ac101_start(ac101_module_t mode);
esp_err_t ac101_stop(ac101_module_t mode);
esp_err_t ac101_set_voice_volume(int volume);
esp_err_t ac101_get_voice_volume(int *volume);
esp_err_t ac101_set_voice_mute(bool enable);
esp_err_t ac101_get_voice_mute(void);
esp_err_t ac101_set_mic_gain(es_mic_gain_t gain);
esp_err_t ac101_config_adc_input(es_adc_input_t input);
esp_err_t ac101_config_dac_output(es_dac_output_t output);
static esp_err_t ac101_write_reg(uint8_t slave_add, uint8_t reg_add, uint16_t data);
void ac101_read_all();
esp_err_t ac101_config_i2s(audio_hal_codec_mode_t mode, audio_hal_codec_i2s_iface_t *iface);
esp_err_t ac101_ctrl_state(audio_hal_codec_mode_t mode, audio_hal_ctrl_t ctrl_state);
void ac101_pa_power(bool enable);

#ifdef __cplusplus
}
#endif

#endif //__AC101_H__
