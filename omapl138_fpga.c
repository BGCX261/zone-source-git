/* by yj */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/irq.h>

#include <mach/mux.h>
#include <mach/common.h>

#include "fpga_program.h"

#define FPGA_GPIO_BASE_ADDR 0x01E26010UL
#define GPIO_BANKS_SIZE 40
#define DIR_OFFSET      0
#define OUT_DATA_OFFSET 4
#define SET_DATA_OFFSET 8
#define CLR_DATA_OFFSET 12
#define IN_DATA_OFFSET  16

#define NCONFIG_LOW_DELAY 50

#define PIN_DIR_WRITE 0
#define PIN_DIR_READ 1

#define MSG(format, args...) \
  printk("%s:%d" format "\n", __FUNCTION__, __LINE__,  ##args)

#define MUX_CFG(soc, desc, muxreg, mode_offset, mode_mask, mux_mode, dbg) \
	[soc##_##desc] = {													\
		.name =  #desc,													\
		.debug = dbg,													\
		.mux_reg_name = "PINMUX"#muxreg,								\
		.mux_reg = PINMUX(muxreg),										\
		.mask_offset = mode_offset,										\
		.mask = mode_mask,												\
		.mode = mux_mode,												\
	},

static void __iomem *gpio_base;

/* enum fpga_config_pin_index{ */
/* 	NCONFIG,					/\* wr *\/ */
/* 	NSTATUS,					/\* rd *\/ */
/* 	CONF_DONE,					/\* rd/wr *\/ */
/* 	DCLK,						/\* wr *\/ */
/* 	DATA0,						/\* wr *\/ */
/* 	USER_IO,					/\* rd *\/ */
/* 	INIT_DONE,					/\* rd *\/ */
/* }; */

enum fpga_pin_mux_index{
	/* FPGA gpio CONFIG*/
	OMAPL138_FPGA_GPIO_NCONFIG,
	OMAPL138_FPGA_GPIO_NSTATUS,
	OMAPL138_FPGA_GPIO_CONF_DONE,
	OMAPL138_FPGA_GPIO_DCLK,
	OMAPL138_FPGA_GPIO_DATA0,
	/* OMAPL138_FPGA_GPIO_USER_IO, */
	OMAPL138_FPGA_GPIO_INIT_DONE,

	OMAPL138_FPGA_FPGA_RESET,
	/* OMAPL138_FPGA_UP_RESETN, */
	/* OMAPL138_FPGA_RESETOUTN, */
	OMAPL138_FPGA_FPGA_PIN_Z,
	OMAPL138_FPGA_CRC_ERROR,

	/* FPGA gpio ADDR */
	OMAPL138_FPGA_GPIO_ADDR_0,
	OMAPL138_FPGA_GPIO_ADDR_1,
	OMAPL138_FPGA_GPIO_ADDR_2,
	OMAPL138_FPGA_GPIO_ADDR_3,
	OMAPL138_FPGA_GPIO_ADDR_4,
	OMAPL138_FPGA_GPIO_ADDR_5,
	OMAPL138_FPGA_GPIO_ADDR_6,
	OMAPL138_FPGA_GPIO_ADDR_7,
	OMAPL138_FPGA_GPIO_ADDR_8,
	OMAPL138_FPGA_GPIO_ADDR_9,
	OMAPL138_FPGA_GPIO_ADDR_10,
	OMAPL138_FPGA_GPIO_ADDR_11,
	OMAPL138_FPGA_GPIO_ADDR_12,
	OMAPL138_FPGA_GPIO_ADDR_13,
	OMAPL138_FPGA_GPIO_ADDR_14,
	OMAPL138_FPGA_GPIO_ADDR_15,

	/* FPGA gpio DATA */
	OMAPL138_FPGA_GPIO_DATA_0,
	OMAPL138_FPGA_GPIO_DATA_1,
	OMAPL138_FPGA_GPIO_DATA_2,
	OMAPL138_FPGA_GPIO_DATA_3,
	OMAPL138_FPGA_GPIO_DATA_4,
	OMAPL138_FPGA_GPIO_DATA_5,
	OMAPL138_FPGA_GPIO_DATA_6,
	OMAPL138_FPGA_GPIO_DATA_7,
	OMAPL138_FPGA_GPIO_DATA_8,
	OMAPL138_FPGA_GPIO_DATA_9,
	OMAPL138_FPGA_GPIO_DATA_10,
	OMAPL138_FPGA_GPIO_DATA_11,
	OMAPL138_FPGA_GPIO_DATA_12,
	OMAPL138_FPGA_GPIO_DATA_13,
	OMAPL138_FPGA_GPIO_DATA_14,
	OMAPL138_FPGA_GPIO_DATA_15,
	OMAPL138_FPGA_GPIO_DATA_16,
	OMAPL138_FPGA_GPIO_DATA_17,
	OMAPL138_FPGA_GPIO_DATA_18,
	OMAPL138_FPGA_GPIO_DATA_19,
	OMAPL138_FPGA_GPIO_DATA_20,
	OMAPL138_FPGA_GPIO_DATA_21,
	OMAPL138_FPGA_GPIO_DATA_22,
	OMAPL138_FPGA_GPIO_DATA_23,
	OMAPL138_FPGA_GPIO_DATA_24,
	OMAPL138_FPGA_GPIO_DATA_25,
	OMAPL138_FPGA_GPIO_DATA_26,
	OMAPL138_FPGA_GPIO_DATA_27,
	OMAPL138_FPGA_GPIO_DATA_28,
	OMAPL138_FPGA_GPIO_DATA_29,
	OMAPL138_FPGA_GPIO_DATA_30,
	OMAPL138_FPGA_GPIO_DATA_31
};

/* 端口多功能配置 */
/* 要按照enum fpga_pin_mux_index顺序配置 */
static const struct mux_config fpga_pins[] = {
	/* PINMUX4:所以是4，PINMUX4_15_12:所以是12，8h:Selects Function GP1[4],所以是8 */
	MUX_CFG(OMAPL138, FPGA_GPIO_NCONFIG,  12, 28, 15, 8, false) /* PINMUX12_31_28:8:gp5[0] */
	MUX_CFG(OMAPL138, FPGA_GPIO_NSTATUS, 12, 12, 15, 8, false)	/* PINMUX12_15_12:8:gp5[4] */
	MUX_CFG(OMAPL138, FPGA_GPIO_CONF_DONE, 12, 16, 15, 8, false)	/* PINMUX12_19_16:8:gp5[3] */
	/* MUX_CFG(OMAPL138, FPGA_GPIO_DCLK,     4, 24, 15, 8, false) /\* gp1[1] *\/ /\* gp1[2]是串口2的 *\/ */
	/* PINMUX19_31_28 */
	MUX_CFG(OMAPL138, FPGA_GPIO_DCLK, 12, 8, 15, 8, false) /* PINMUX12_11_8:8:gp5[5] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA0, 12, 4, 15, 8, false) /* PINMUX12_7_4:8:gp5[6] */ /* gp1[3]是串口2的 */
	/* MUX_CFG(OMAPL138, FPGA_GPIO_USER_IO,  0, 16, 15, 8, false) /\* gp0[11] *\/ */
	MUX_CFG(OMAPL138, FPGA_GPIO_INIT_DONE, 12, 0, 15, 8, false) /* PINMUX12_3_0:8:gp5[7] */

	MUX_CFG(OMAPL138, FPGA_FPGA_RESET, 18, 24, 15, 8, false)		 /* PINMUX18_27_24:8:GP8[11] */
	/* MUX_CFG(OMAPL138, FPGA_UP_RESETN, 4, 12, 15, 8, false) /\* gp1[4] *\/ */
	/* MUX_CFG(OMAPL138, FPGA_RESETOUTN, 4, 12, 15, 8, false) /\* gp1[4] *\/ */
	MUX_CFG(OMAPL138, FPGA_FPGA_PIN_Z, 19, 4, 15, 8, false) /* PINMUX19_7_4:8:gp8[8] */
	MUX_CFG(OMAPL138, FPGA_CRC_ERROR, 18, 20, 15, 8, false) /* PINMUX18_23_20:8:gp8[12] */

	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_0, 10, 28, 15, 8, false) /* PINMUX10_31_28:8:gp4[0] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_1, 10, 24, 15, 8, false) /* PINMUX10_27_24:8:gp4[1] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_2, 10, 20, 15, 8, false) /* PINMUX10_23_20:8:gp4[2] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_3, 10, 16, 15, 8, false) /* PINMUX10_19_16:8:gp4[3] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_4, 10, 12, 15, 8, false) /* PINMUX10_15_12:8:gp4[4] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_5, 10, 8, 15, 8, false) /* PINMUX10_11_8:8:gp4[5] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_6, 10, 4, 15, 8, false) /* PINMUX10_7_4:8:gp4[6] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_7, 10, 0, 15, 8, false) /* PINMUX10_3_0:8:gp4[7] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_8, 11, 28, 15, 8, false) /* PINMUX11_31_28:8:fgp5[8] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_9, 11, 24, 15, 8, false) /* PINMUX11_27_24:8:gp5[9] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_10, 11, 20, 15, 8, false) /* PINMUX11_23_20:8:gp5[10] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_11, 11, 16, 15, 8, false) /* PINMUX11_19_16:8:gp5[11] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_12, 11, 12, 15, 8, false) /* PINMUX11_15_12:8:gp5[12] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_13, 11, 8, 15, 8, false) /* PINMUX11_11_8:8:gp5[13] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_14, 11, 4, 15, 8, false) /* PINMUX11_7_4:8:gp5[14] */
	MUX_CFG(OMAPL138, FPGA_GPIO_ADDR_15, 11, 0, 15, 8, false) /* PINMUX11_3_0:8:gp5[15] */

	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_0, 19, 24, 15, 8, false) /* PINMUX19_27_24:8:gp6[0] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_1, 19, 20, 15, 8, false) /* PINMUX19_23_20:8:gp6[1] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_2, 19, 16, 15, 8, false) /* PINMUX19_19_16:8:gp6[2] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_3, 19, 12, 15, 8, false) /* PINMUX19_15_12:8:gp6[3] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_4, 19, 8, 15, 8, false) /* PINMUX19_11_8:8:gp6[4] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_5, 16, 4, 15, 8, false) /* PINMUX16_7_4:8:gp6[5] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_6, 14, 4, 15, 8, false) /* PINMUX14_7_4:8:gp6[6] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_7, 14, 0, 15, 8, false) /* PINMUX14_3_0:8:gp6[7] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_8, 8, 28, 15, 8, false) /* PINMUX8_31_28:8:gp3[0] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_9, 8, 24, 15, 8, false) /* PINMUX8_27_24:8:gp3[1] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_10, 8, 20, 15, 8, false) /* PINMUX8_23_20:8:gp3[2] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_11, 8, 16, 15, 8, false) /* PINMUX8_19_16:8:gp3[3] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_12, 8, 12, 15, 8, false) /* PINMUX8_15_12:8:gp3[4] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_13, 8, 8, 15, 8, false) /* PINMUX8_11_8:8:gp3[5] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_14, 8, 4, 15, 8, false) /* PINMUX8_7_4:8:gp3[6] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_15, 8, 0, 15, 8, false) /* PINMUX8_3_0:8:gp3[7] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_16, 18, 4, 15, 8, false) /* PINMUX18_7_4:8:gp7[0] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_17, 18, 0, 15, 8, false) /* PINMUX18_3_0:8:gp7[1] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_18, 17, 28, 15, 8, false) /* PINMUX17_31_28:8:gp7[2] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_19, 17, 24, 15, 8, false) /* PINMUX17_27_24:8:gp7[3] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_20, 17, 20, 15, 8, false) /* PINMUX17_23_20:8:gp7[4] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_21, 17, 16, 15, 8, false) /* PINMUX17_19_16:8:gp7[5] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_22, 17, 12, 15, 8, false) /* PINMUX17_15_12:8:gp7[6] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_23, 17, 8, 15, 8, false) /* PINMUX17_11_8:8:gp7[7] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_24, 17, 4, 15, 8, false) /* PINMUX17_7_4:8:gp7[8] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_25, 17, 0, 15, 8, false) /* PINMUX17_3_0:8:gp7[9] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_26, 16, 28, 15, 8, false) /* PINMUX16_31_28:8:gp7[10] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_27, 16, 24, 15, 8, false) /* PINMUX16_27_24:8:gp7[11] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_28, 16, 20, 15, 8, false) /* PINMUX16_23_20:8:gp7[12] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_29, 16, 16, 15, 8, false) /* PINMUX16_19_16:8:gp7[13] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_30, 16, 12, 15, 8, false) /* PINMUX16_15_12:8:gp7[14] */
	MUX_CFG(OMAPL138, FPGA_GPIO_DATA_31, 16, 8, 15, 8, false) /* PINMUX16_11_8:8:gp7[15] */
};

const short omapl138_fpga_gpio_config[] __initdata = {
	 OMAPL138_FPGA_GPIO_NCONFIG,
	 OMAPL138_FPGA_GPIO_NSTATUS,
	 OMAPL138_FPGA_GPIO_CONF_DONE,
	 OMAPL138_FPGA_GPIO_DCLK,
	 OMAPL138_FPGA_GPIO_DATA0,
	 /* OMAPL138_FPGA_GPIO_USER_IO, */
	 OMAPL138_FPGA_GPIO_INIT_DONE,
	 OMAPL138_FPGA_FPGA_RESET,
	 /* OMAPL138_FPGA_UP_RESETN, */
	 /* OMAPL138_FPGA_RESETOUTN, */
	 OMAPL138_FPGA_FPGA_PIN_Z,
	 OMAPL138_FPGA_CRC_ERROR,
	 -1
 };

 const short omapl138_fpga_gpio_addr[] __initdata = {
	 OMAPL138_FPGA_GPIO_ADDR_0,
	 OMAPL138_FPGA_GPIO_ADDR_1,
	 OMAPL138_FPGA_GPIO_ADDR_2,
	 OMAPL138_FPGA_GPIO_ADDR_3,
	 OMAPL138_FPGA_GPIO_ADDR_4,
	 OMAPL138_FPGA_GPIO_ADDR_5,
	 OMAPL138_FPGA_GPIO_ADDR_6,
	 OMAPL138_FPGA_GPIO_ADDR_7,
	 OMAPL138_FPGA_GPIO_ADDR_8,
	 OMAPL138_FPGA_GPIO_ADDR_9,
	 OMAPL138_FPGA_GPIO_ADDR_10,
	 OMAPL138_FPGA_GPIO_ADDR_11,
	 OMAPL138_FPGA_GPIO_ADDR_12,
	 OMAPL138_FPGA_GPIO_ADDR_13,
	 OMAPL138_FPGA_GPIO_ADDR_14,
	 OMAPL138_FPGA_GPIO_ADDR_15,
	 -1
};

const short omapl138_fpga_gpio_data[] __initdata = {
	/* FPGA gpio DATA */
	OMAPL138_FPGA_GPIO_DATA_0,
	OMAPL138_FPGA_GPIO_DATA_1,
	OMAPL138_FPGA_GPIO_DATA_2,
	OMAPL138_FPGA_GPIO_DATA_3,
	OMAPL138_FPGA_GPIO_DATA_4,
	OMAPL138_FPGA_GPIO_DATA_5,
	OMAPL138_FPGA_GPIO_DATA_6,
	OMAPL138_FPGA_GPIO_DATA_7,
	OMAPL138_FPGA_GPIO_DATA_8,
	OMAPL138_FPGA_GPIO_DATA_9,
	OMAPL138_FPGA_GPIO_DATA_10,
	OMAPL138_FPGA_GPIO_DATA_11,
	OMAPL138_FPGA_GPIO_DATA_12,
	OMAPL138_FPGA_GPIO_DATA_13,
	OMAPL138_FPGA_GPIO_DATA_14,
	OMAPL138_FPGA_GPIO_DATA_15,
	OMAPL138_FPGA_GPIO_DATA_16,
	OMAPL138_FPGA_GPIO_DATA_17,
	OMAPL138_FPGA_GPIO_DATA_18,
	OMAPL138_FPGA_GPIO_DATA_19,
	OMAPL138_FPGA_GPIO_DATA_20,
	OMAPL138_FPGA_GPIO_DATA_21,
	OMAPL138_FPGA_GPIO_DATA_22,
	OMAPL138_FPGA_GPIO_DATA_23,
	OMAPL138_FPGA_GPIO_DATA_24,
	OMAPL138_FPGA_GPIO_DATA_25,
	OMAPL138_FPGA_GPIO_DATA_26,
	OMAPL138_FPGA_GPIO_DATA_27,
	OMAPL138_FPGA_GPIO_DATA_28,
	OMAPL138_FPGA_GPIO_DATA_29,
	OMAPL138_FPGA_GPIO_DATA_30,
	OMAPL138_FPGA_GPIO_DATA_31,
	-1
};

struct fpga_config_pin{
	char *const name;
	unsigned short int bank_addr_offset;
	void *__iomem bank_base_addr;
	unsigned char pin_mask;		/* the offset of pin (0-31) */
	unsigned char dir;
};

#define PIN_INF(soc, desc, bank_offset, pinmask, direct) \
	[soc##_##desc] = {													\
		.name =  #desc,													\
		.bank_addr_offset = (bank_offset),								\
		.pin_mask = (pinmask),											\
		.dir = (direct),												\
	},

#define FPGA_GPIO_DIR_WR 0
#define FPGA_GPIO_DIR_RD 1

#define FPGA_DATA_DIR_WRITE FPGA_GPIO_DIR_WR
#define FPGA_DATA_DIR_READ  FPGA_GPIO_DIR_RD
/*size is 55*/
/* 0-output; 1-input */
/* bank_addr_offset : 0/1- 0; 2/3- 1; 4/5- 2; 6/7- 3; 8- 4 */
/* bank_addr_offset, pinmask, direct.  pinmask是从0开始的吗?bank_addr_offset是从0开始 */
struct fpga_config_pin fpga_cnf_pin[] = {
	PIN_INF(OMAPL138, FPGA_GPIO_NCONFIG, 2, 0+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_NSTATUS, 2, 4+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_CONF_DONE, 2, 3+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DCLK, 2, 5+16, FPGA_GPIO_DIR_WR) /* gp8[0] */
	PIN_INF(OMAPL138, FPGA_GPIO_DATA0, 2, 6+16, FPGA_GPIO_DIR_WR)
	/* PIN_INF(OMAPL138, FPGA_GPIO_USER_IO, 0, 4+16, FPGA_GPIO_DIR_WR) */
	PIN_INF(OMAPL138, FPGA_GPIO_INIT_DONE, 2, 7+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_FPGA_RESET, 4, 11, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_FPGA_PIN_Z, 4, 8, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_CRC_ERROR, 4, 12, FPGA_GPIO_DIR_RD)

	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_0, 2, 0, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_1, 2, 1, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_2, 2, 2, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_3, 2, 3, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_4, 2, 4, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_5, 2, 5, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_6, 2, 6, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_7, 2, 7, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_8, 2, 8+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_9, 2, 9+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_10, 2, 10+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_11, 2, 11+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_12, 2, 12+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_13, 2, 13+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_14, 2, 14+16, FPGA_GPIO_DIR_WR)
	PIN_INF(OMAPL138, FPGA_GPIO_ADDR_15, 2, 15+16, FPGA_GPIO_DIR_WR)

	/* bank_addr_offset : 0/1- 0; 2/3- 1; 4/5- 2; 6/7- 3; 8- 4 */
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_0, 3, 0, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_1, 3, 1, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_2, 3, 2, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_3, 3, 3, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_4, 3, 4, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_5, 3, 5, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_6, 3, 6, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_7, 3, 7, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_8, 1,  0+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_9, 1,  1+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_10, 1, 2+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_11, 1, 3+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_12, 1, 4+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_13, 1, 5+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_14, 1, 6+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_15, 1, 7+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_16, 3, 0 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_17, 3, 1 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_18, 3, 2 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_19, 3, 3 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_20, 3, 4 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_21, 3, 5 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_22, 3, 6 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_23, 3, 7 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_24, 3, 8 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_25, 3, 9 +16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_26, 3, 10+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_27, 3, 11+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_28, 3, 12+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_29, 3, 13+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_30, 3, 14+16, FPGA_GPIO_DIR_RD)
	PIN_INF(OMAPL138, FPGA_GPIO_DATA_31, 3, 15+16, FPGA_GPIO_DIR_RD)
};

static void __iomem *pinmux_base;

/*
 * Sets the DAVINCI MUX register based on the table
 */
int __init_or_module omapl138_cfg_reg(const unsigned long index)
{
	static DEFINE_SPINLOCK(mux_spin_lock);
	struct davinci_soc_info *soc_info = &davinci_soc_info;
	unsigned long flags;
	const struct mux_config *cfg;
	unsigned int reg_orig = 0, reg = 0;
	unsigned int mask, warn = 0;

	/* if (WARN_ON(!soc_info->pinmux_pins)) */
	/* 	return -ENODEV; */

	if (!pinmux_base) {
		pinmux_base = ioremap(soc_info->pinmux_base, SZ_4K);
		if (WARN_ON(!pinmux_base))
			return -ENOMEM;
	}

	/* if (index >= soc_info->pinmux_pins_num) { */
	/* 	printk(KERN_ERR "Invalid pin mux index: %lu (%lu)\n", */
	/* 	       index, soc_info->pinmux_pins_num); */
	/* 	dump_stack(); */
	/* 	return -ENODEV; */
	/* } */

	/* cfg = &soc_info->pinmux_pins[index]; */
	cfg = fpga_pins+index;

	if (cfg->name == NULL) {
		printk(KERN_ERR "No entry for the specified index\n");
		return -ENODEV;
	}

	/* Update the mux register in question */
	if (cfg->mask) {
		unsigned	tmp1, tmp2;

		spin_lock_irqsave(&mux_spin_lock, flags);
		reg_orig = __raw_readl(pinmux_base + cfg->mux_reg);

		mask = (cfg->mask << cfg->mask_offset);
		tmp1 = reg_orig & mask;
		reg = reg_orig & ~mask;

		tmp2 = (cfg->mode << cfg->mask_offset);
		reg |= tmp2;

		if (tmp1 != tmp2)
			warn = 1;

		__raw_writel(reg, pinmux_base + cfg->mux_reg);
		spin_unlock_irqrestore(&mux_spin_lock, flags);
	}

	if (warn) {
#ifdef CONFIG_DAVINCI_MUX_WARNINGS
		printk(KERN_WARNING "MUX: initialized %s\n", cfg->name);
#endif
	}

#ifdef CONFIG_DAVINCI_MUX_DEBUG
	if (cfg->debug || warn) {
		printk(KERN_WARNING "MUX: Setting register %s\n", cfg->name);
		printk(KERN_WARNING "	   %s (0x%08x) = 0x%08x -> 0x%08x\n",
		       cfg->mux_reg_name, cfg->mux_reg, reg_orig, reg);
	}
#endif

	/* MSG("cfg->name = %s", cfg->name); */
	return 0;
}

int __init_or_module omapl138_cfg_reg_list(const short pins[])
{
	int i, error = -EINVAL;

	if (pins)
		for (i = 0; pins[i] >= 0; i++) {
			error = omapl138_cfg_reg(pins[i]);
			if (error)
				break;
		}
	
	return error;
}

static void fpga_pin_config_dir_pin(int i)
{
	unsigned int tmp;
	struct fpga_config_pin *pin_config;
	void __iomem *dir_addr;

	pin_config = fpga_cnf_pin+i;
	dir_addr = pin_config->bank_base_addr + DIR_OFFSET;
	tmp = __raw_readl(dir_addr);
	if (pin_config->dir){
		tmp |= 1 << pin_config->pin_mask;
	}else{
		tmp &= ~(1 << pin_config->pin_mask);
	}
	writel(tmp, dir_addr);

}

static void fpga_pin_config_dir(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(fpga_cnf_pin); i++){
		fpga_pin_config_dir_pin(i);
	}
}

#define FPGA_DEBUG
int fpga_config_update_data(const unsigned char *const program_dat, size_t dat_size)
{
	void * __iomem nconfig_set = fpga_cnf_pin[OMAPL138_FPGA_GPIO_NCONFIG].bank_base_addr+SET_DATA_OFFSET;
	void * __iomem nconfig_clr = fpga_cnf_pin[OMAPL138_FPGA_GPIO_NCONFIG].bank_base_addr+CLR_DATA_OFFSET;
	void * __iomem nstatus_in = fpga_cnf_pin[OMAPL138_FPGA_GPIO_NSTATUS].bank_base_addr+IN_DATA_OFFSET;
	void * __iomem conf_done_in = fpga_cnf_pin[OMAPL138_FPGA_GPIO_CONF_DONE].bank_base_addr+IN_DATA_OFFSET;
	/* void * __iomem conf_done_set = fpga_cnf_pin[OMAPL138_FPGA_GPIO_CONF_DONE].bank_base_addr+SET_DATA_OFFSET; */
	void * __iomem dclk_set = fpga_cnf_pin[OMAPL138_FPGA_GPIO_DCLK].bank_base_addr+SET_DATA_OFFSET;
	void * __iomem dclk_clr = fpga_cnf_pin[OMAPL138_FPGA_GPIO_DCLK].bank_base_addr+CLR_DATA_OFFSET;
	void * __iomem data_set = fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA0].bank_base_addr+SET_DATA_OFFSET;
	void * __iomem data_clr = fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA0].bank_base_addr+CLR_DATA_OFFSET;
	/* void * __iomem user_io_in = fpga_cnf_pin[OMAPL138_FPGA_GPIO_USER_IO].bank_base_addr+IN_DATA_OFFSET; */
	void * __iomem init_done_in = fpga_cnf_pin[OMAPL138_FPGA_GPIO_INIT_DONE].bank_base_addr+IN_DATA_OFFSET;

	unsigned char nconfig_offset = fpga_cnf_pin[OMAPL138_FPGA_GPIO_NCONFIG].pin_mask;
	unsigned char nstatus_offset = fpga_cnf_pin[OMAPL138_FPGA_GPIO_NSTATUS].pin_mask;
	unsigned char conf_done_offset = fpga_cnf_pin[OMAPL138_FPGA_GPIO_CONF_DONE].pin_mask;
	unsigned char dclk_offset = fpga_cnf_pin[OMAPL138_FPGA_GPIO_DCLK].pin_mask;
	unsigned char data_offset = fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA0].pin_mask;
	/* unsigned char user_io_offset = fpga_cnf_pin[OMAPL138_FPGA_GPIO_USER_IO].pin_mask; */
	unsigned char init_done_offset = fpga_cnf_pin[OMAPL138_FPGA_GPIO_INIT_DONE].pin_mask;

	int tick = 3;
	int i=0,j=0;
	unsigned char tmp_data;
	unsigned long flags;

	local_irq_save(flags);
	/* startup */
	/* 在nCONFIG引脚上产生1个超过40μs的低脉冲，等待nSTATUS回应1个低脉冲以及CONF_DONE变低。 */
	__raw_writel(1 << nconfig_offset, nconfig_set);	/* nconfig set high */
	udelay(10);
	__raw_writel(1 << nconfig_offset, nconfig_clr);	/* nconfig set low */
	udelay(NCONFIG_LOW_DELAY);
#ifndef FPGA_DEBUG
	while(1){
		if (!(__raw_readl(nstatus_in) & (1 << nstatus_offset))
			&& !(__raw_readl(conf_done_in) & (1 << conf_done_offset)))
		/* if ((__raw_readl(nstatus_in) & (1 << nstatus_offset)) */
			/* && (__raw_readl(conf_done_in) & (1 << conf_done_offset))) */
			break;
		udelay(NCONFIG_LOW_DELAY);
		if (!tick--){
			printk(KERN_WARNING "FPGA config error: wait nstatus and conf_done low timeout!!\n");
			goto delay_timeout;
		}
	}
	/* 在nCONFIG跳高后40μs内nSTATUS也跳高，表明Cyclone可以开始配置了。 */
	udelay(10);
	__raw_writel(1 << nconfig_offset, nconfig_set);	/* nconfig set high */
	udelay(NCONFIG_LOW_DELAY);
	tick = 3;
	while(1){
		if (__raw_readl(nstatus_in) & (1 << nstatus_offset))
			break;
		udelay(NCONFIG_LOW_DELAY);
		if (!tick--){
			printk(KERN_WARNING "FPGA config error: wait nstatus high timeout!!\n");
			goto delay_timeout;
		}
	}
#endif

	/* 当出现上升沿时，芯片将一位数据通过DATA0引脚传入内部，直至所有数据都被移入。 */
	/* 一旦出现错误，nSTATUS被拉低，系统必须能识别这个信号，并重新启动配置过程 */
#define FPGA_CLK_TIME 1
	/* input configure data */
	for (i = 0; i < dat_size; i++){
		tmp_data = program_dat[i];
		for (j = 7; j >= 0; j--){
			__raw_writel(1 << dclk_offset, dclk_clr); /* clk low */
			if (tmp_data & (1 << j)){                /* data */
				__raw_writel(1 << data_offset, data_set);
			}else{
				__raw_writel(1 << data_offset, data_clr);
			}
			udelay(FPGA_CLK_TIME);
			__raw_writel(1 << dclk_offset, dclk_set); /* clk high, data in */
			udelay(FPGA_CLK_TIME);
#ifndef FPGA_DEBUG
			if (!(__raw_readl(nstatus_in) & (1 << nstatus_offset))){ /* nstatus */
				printk(KERN_WARNING "FPGA config error: update data error!!\n");
				goto data_error;
			}
#endif
		}
	}
	
	/* 在最后一个位时的时钟下降沿才会把conf_done拉高 */
	/* set conf_done pin high*/
	/* fpga_cnf_pin[OMAPL138_FPGA_GPIO_CONF_DONE].dir = PIN_DIR_WRITE; */
	/* fpga_pin_config_dir_pin(OMAPL138_FPGA_GPIO_CONF_DONE); */
	/* __raw_writel(1 << conf_done_offset, conf_done_set); */

	/* 配置数据全部正确移入芯片内部后，器件释放CONF_DONE，由外部将其拉高。此后需要继续给Cyclone提供6～20μs的时钟来初始化器件。 */
	/* 初始化完成后，INIT_DONE输出为高，表明器件进入用户状态，此后DCLK和DATA0都不应空接。 */
	/* 在这里还需要时钟 */
#define CONFIG_INIT_TIME 100
	for (j = 0; j < CONFIG_INIT_TIME; j++){
		__raw_writel(1 << dclk_offset, dclk_clr); /* clk low */
		udelay(FPGA_CLK_TIME);
		__raw_writel(1 << dclk_offset, dclk_set); /* clk high */
		udelay(FPGA_CLK_TIME);
		/* wait init_done and conf_done high */
		if ((__raw_readl(init_done_in) & (1 << init_done_offset)) &&
			(__raw_readl(conf_done_in) & (1 << conf_done_offset))) 
			break;
	}
	
#ifndef FPGA_DEBUG 
	if (j >= CONFIG_INIT_TIME){
		printk(KERN_WARNING "FPGA config error: wait init_done high timeout!!\n");
		goto delay_timeout;
	}
#endif
	
/* #ifndef FPGA_DEBUG  */
/* 	/\* wait init_done high *\/ */
/* 	tick = 3; */
/* 	while(1){ */
/* 		if ((__raw_readl(init_done_in) & (1 << init_done_offset))) */
/* 			break; */
/* 		udelay(NCONFIG_LOW_DELAY); */

/* 		if (!tick--){ */
/* 			printk(KERN_WARNING "FPGA config error: wait init_done high timeout!!\n"); */
/* 			goto delay_timeout; */
/* 		} */
/* 	} */
/* #endif */

 /* config_success: */
	printk(KERN_NOTICE "FPGA configure completely, program size %d Byts.\n", i);
	local_irq_restore(flags);
	return 0;

 delay_timeout:
 data_error:
	printk(KERN_NOTICE "FPGA configure completely, program size %d Byts.\n", i);
	local_irq_restore(flags);
	return -EIO;
}
EXPORT_SYMBOL(fpga_config_update_data);


#define CONTINUOUS_DATA_PIN

#ifdef CONTINUOUS_DATA_PIN
/* void *__iomem fpga_addr_dir_reg; */
/* void *__iomem fpga_addr_out_reg; */
/* void *__iomem fpga_addr_set_reg; */
/* void *__iomem fpga_addr_clr_reg; */
void *__iomem fpga_addr_45dir_reg;
void *__iomem fpga_addr_45out_reg;

void *__iomem fpga_data_23dir_reg;
void *__iomem fpga_data_67dir_reg;
void *__iomem fpga_data_23out_reg;
void *__iomem fpga_data_67out_reg;
void *__iomem fpga_data_23in_reg;
void *__iomem fpga_data_67in_reg;
/* bank_addr_offset : 0/1- 0; 2/3- 1; 4/5- 2; 6/7- 3; 8- 4 */
/* 6[0-7],3[0-7],7[0-15] */
static inline void set_fpga_data_dir(int dir)
{
	u32 tmp;

    if (dir){
		tmp = __raw_readl(fpga_data_23dir_reg) | 0xff0000;
		__raw_writel(tmp, fpga_data_23dir_reg);
		tmp = __raw_readl(fpga_data_67dir_reg) | 0xffff00ff;
		__raw_writel(tmp, fpga_data_67dir_reg);
	}else{
		tmp = __raw_readl(fpga_data_23dir_reg) | (~0xff0000);
		__raw_writel(tmp, fpga_data_23dir_reg);
		tmp = __raw_readl(fpga_data_67dir_reg) | (~0xffff00ff);
		__raw_writel(tmp, fpga_data_67dir_reg);
	}
}

static inline void write_fpga_addr (u16 addr) /* gp4[0-7],gp5[8-16] */
{
	u32 tmp;
	tmp = __raw_readl(fpga_addr_45out_reg);
	tmp &= 0x00ffff00;
	tmp |= addr & 0xff;
	tmp |= (addr & (~0xf0ff)) << 16; /* 还不是16位的 */
	__raw_writel(tmp, fpga_addr_45out_reg);
}

static inline u32 get_fpga_data (void)
{
	u32 tmp1, tmp2;
	tmp1 = __raw_readl(fpga_data_23in_reg);
	tmp2 = __raw_readl(fpga_data_67in_reg);
	return (tmp2 & 0xffff00ff) | ((tmp1 & 0xff0000) >> 8);
}
static inline void set_fpga_data (u32 data)
{
	u32 tmp1, tmp2;

	tmp1 = __raw_readl(fpga_data_23out_reg);
	tmp1 &= 0xff00ffff;
	tmp1 |= (data & 0xff00) << 8;
	__raw_writel(tmp1, fpga_data_23out_reg);

	tmp2 = __raw_readl(fpga_data_67out_reg);
	tmp2 &= 0xff00;
	tmp2 |= (data & 0xffff00ff);
	__raw_writel(tmp2, fpga_data_67out_reg);
}
#else
#define GPIO_BANK_NUM 5
/* #define DEFINE_SPINLOCK(x)	spinlock_t x = __SPIN_LOCK_UNLOCKED(x) */
DEFINE_SPINLOCK(fpga_data_dir_spinlock);
void *__iomem fpga_data_dir_addr[GPIO_BANK_NUM];
u32 fpga_data_dir_mask[GPIO_BANK_NUM];					/* 1-相应的位被使用作数据端口 */
static inline void set_fpga_data_dir(int dir)
{
	int i = 0;
	u32 tmp;
	for(i = 0; i < GPIO_BANKS_SIZE; i++){
		tmp = __raw_readl(fpga_data_dir_addr[i]);
		if (dir)
			tmp |= fpga_data_dir_mask[i];
		else
			tmp &= ~fpga_data_dir_mask[i];
		__raw_writel(tmp, fpga_data_dir_addr[i]);
	}
}
static inline void write_fpga_addr (u16 addr) /* gp4[0-7],gp5[8-16] */
{}
static inline void set_fpga_data (void)
{}
#endif

#define SYNC_PIN OMAPL138_FPGA_GPIO_ADDR_12 /* ???? */

static inline void fpga_sync_write(void)
{
	/* MSG("begin"); */
	__raw_writel(1<<fpga_cnf_pin[SYNC_PIN].pin_mask,
				 fpga_cnf_pin[SYNC_PIN].bank_base_addr+CLR_DATA_OFFSET);
	ndelay(100);
	/* MSG("half"); */
	__raw_writel(1<<fpga_cnf_pin[SYNC_PIN].pin_mask,
				 fpga_cnf_pin[SYNC_PIN].bank_base_addr+SET_DATA_OFFSET);
	/* MSG("end"); */
}

static unsigned int fpga_data_cur_dir=FPGA_DATA_DIR_READ;

unsigned int fpga_read32(u16 addr, u32 *const dat)
{
	if (fpga_data_cur_dir != FPGA_DATA_DIR_READ){
		set_fpga_data_dir(FPGA_DATA_DIR_READ);
		fpga_data_cur_dir = FPGA_DATA_DIR_READ;
	}

	write_fpga_addr(addr);

	fpga_sync_write();

	ndelay(100);

	return get_fpga_data();
	/* (__raw_readl(fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_0]. */
	/* 					bank_base_addr+IN_DATA_OFFSET) & 0xff) */
	/* 	| (__raw_readl(fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_16]. */
	/* 				   bank_base_addr+IN_DATA_OFFSET) & 0xff00); /\* ???? it'necessery shift ? or mask*\/ */

	/* return 0; */
}
EXPORT_SYMBOL(fpga_read32);

int fpga_write32(u16 addr, u32 dat)
{
	/* unsigned int tmp; */

	if (fpga_data_cur_dir != FPGA_DATA_DIR_WRITE){
		set_fpga_data_dir(FPGA_DATA_DIR_WRITE);
		fpga_data_cur_dir = FPGA_DATA_DIR_WRITE;
	}

	write_fpga_addr(addr);

	/* ndelay(100); */

	set_fpga_data (dat);

	/* tmp = (__raw_readl(fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_0]. */
	/* 				   bank_base_addr+OUT_DATA_OFFSET) & 0xff00) | (dat & 0xff); /\* ???? *\/ */
	/* __raw_writel(tmp, fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_0]. */
	/* 			 bank_base_addr+OUT_DATA_OFFSET); */

	/* MSG("write half"); */

	/* tmp = (__raw_readl(fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_16]. */
	/* 				   bank_base_addr+OUT_DATA_OFFSET) & 0xff00) | (dat & 0xff00); /\* ???? *\/ */
	/* __raw_writel(tmp, fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_16]. */
	/* 			 bank_base_addr+OUT_DATA_OFFSET); */

	/* MSG("write end"); */

	fpga_sync_write();

	/* MSG("fpga_sync_write()"); */

	return 0;
}
EXPORT_SYMBOL(fpga_write32);

int __init program_fpga(void)
{
	int i;
	int ret;
	
	/* initilaize fpga */
	ret = (omapl138_cfg_reg_list(omapl138_fpga_gpio_config)
		   || omapl138_cfg_reg_list(omapl138_fpga_gpio_addr)
		   || omapl138_cfg_reg_list(omapl138_fpga_gpio_data));
	if (ret)
		pr_warning("%s: fpga muxpin configure failed: %d\n", __func__, ret);
	
	gpio_base = ioremap_nocache(FPGA_GPIO_BASE_ADDR, SZ_4K);
	if (!gpio_base){
		return -ENOMEM;
	}

	/* printk(KERN_NOTICE "FPGA gpio_base = %u. size is %d\n", (unsigned int)gpio_base, ARRAY_SIZE(fpga_cnf_pin)); */
	for (i = 0; i < ARRAY_SIZE(fpga_cnf_pin); i++){
		struct fpga_config_pin *pin_config = fpga_cnf_pin+i;
		pin_config->bank_base_addr = gpio_base +
			pin_config->bank_addr_offset * GPIO_BANKS_SIZE;
		/* printk(KERN_NOTICE "FPGA pin_config[%d].bank_base_addr = %u. bank_addr_offset = %d, name = %s\n", i, */
		/* 	   (unsigned int)pin_config->bank_base_addr, pin_config->bank_addr_offset, pin_config->name); */
	}
#ifdef CONTINUOUS_DATA_PIN
/* 6[0-7],3[0-7],7[0-15] */
/* bank_addr_offset : 0/1- 0; 2/3- 1; 4/5- 2; 6/7- 3; 8- 4 */
	fpga_addr_45dir_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_ADDR_0].
		bank_addr_offset * GPIO_BANKS_SIZE + DIR_OFFSET;
	fpga_addr_45out_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_ADDR_0].
		bank_addr_offset * GPIO_BANKS_SIZE + OUT_DATA_OFFSET;
	fpga_data_23dir_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_8].
		bank_addr_offset * GPIO_BANKS_SIZE + DIR_OFFSET;
	fpga_data_67dir_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_0].
		bank_addr_offset * GPIO_BANKS_SIZE + DIR_OFFSET;
	fpga_data_23out_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_8].
		bank_addr_offset * GPIO_BANKS_SIZE + OUT_DATA_OFFSET;
	fpga_data_67out_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_0].
		bank_addr_offset * GPIO_BANKS_SIZE + OUT_DATA_OFFSET;
	fpga_data_23in_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_8].
		bank_addr_offset * GPIO_BANKS_SIZE + IN_DATA_OFFSET;
	fpga_data_67in_reg = gpio_base + fpga_cnf_pin[OMAPL138_FPGA_GPIO_DATA_0].
		bank_addr_offset * GPIO_BANKS_SIZE + IN_DATA_OFFSET;
#endif

	fpga_pin_config_dir();

	if (fpga_config_update_data(fpga_program_dat,
								(size_t)ARRAY_SIZE(fpga_program_dat)) < 0){
		return -EIO;
	}

	printk(KERN_NOTICE "FPGA configure completely, program size %d.\n",
		   (size_t)ARRAY_SIZE(fpga_program_dat));

	return 0;
}
