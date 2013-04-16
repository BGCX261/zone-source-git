#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/moduleparam.h>

#include <mach/omapl138_fpga.h>

//////////////////////////////////////////////////////////
#define FPGA_IOCTL_MAGIC 'y'
/* in/out */
#define FPGA_IOCTL_ALL_DI _IOR(FPGA_IOCTL_MAGIC, 0x30, fpga_di_t)
#define FPGA_IOCTL_ALL_DO _IOW(FPGA_IOCTL_MAGIC, 0x31, fpga_do_t)
#define FPGA_IOCTL_ALL_AI _IOR(FPGA_IOCTL_MAGIC, 0x32, fpga_ai_t)
#define FPGA_IOCTL_ALL_CNT _IOR(FPGA_IOCTL_MAGIC, 0x33, fpga_cnt_t)
#define FPGA_IOCTL_ALL_TI _IOR(FPGA_IOCTL_MAGIC, 0x34, fpga_ti_t)
#define FPGA_IOCTL_ALL_AO _IOW(FPGA_IOCTL_MAGIC, 0x35, fpga_ao_t)
#define FPGA_IOCTL_ALL_IN _IOR(FPGA_IOCTL_MAGIC, 0x36, struct fpga_ram)

/* conf */
#define FPGA_IOCTL_CONF_DI _IOW(FPGA_IOCTL_MAGIC, 0x37, fpga_di_cnf_t)
#define FPGA_IOCTL_CONF_DO _IOW(FPGA_IOCTL_MAGIC, 0x38, fpga_do_cnf_t)
#define FPGA_IOCTL_CONF_CNT _IOW(FPGA_IOCTL_MAGIC, 0x39, fpga_cnt_cnf_t)
#define FPGA_IOCTL_CONF_AI _IOW(FPGA_IOCTL_MAGIC, 0x3A, fpga_ai_cnf_t)
#define FPGA_IOCTL_CONF_TI _IOW(FPGA_IOCTL_MAGIC, 0x3B, fpga_ti_cnf_t)

/* conf ao */
#define FPGA_IOCTL_CONF_AO_GAIN_A _IOW(FPGA_IOCTL_MAGIC, 0x3C, fpga_ao_gain_a_t)
#define FPGA_IOCTL_CONF_AO_GAIN_B _IOW(FPGA_IOCTL_MAGIC, 0x3D, fpga_ao_gain_b_t)
#define FPGA_IOCTL_CONF_AO_SCE _IOW(FPGA_IOCTL_MAGIC, 0x3E, fpga_ao_sce_t)
#define FPGA_IOCTL_CONF_AO_CLEAR _IOW(FPGA_IOCTL_MAGIC, 0x3F, fpga_ao_clear_t)
#define FPGA_IOCTL_CONF_AO_OFFSET_A _IOW(FPGA_IOCTL_MAGIC, 0x40, fpga_ao_offset_a_t)
#define FPGA_IOCTL_CONF_AO_OFFSET_B _IOW(FPGA_IOCTL_MAGIC, 0x41, fpga_ao_offset_b_t)

#define FPGA_IOCTL_CONF_AO_ZERO_0 _IOW(FPGA_IOCTL_MAGIC, 0x42, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_ZERO_1 _IOW(FPGA_IOCTL_MAGIC, 0x43, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_ZERO_2 _IOW(FPGA_IOCTL_MAGIC, 0x44, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_ZERO_3 _IOW(FPGA_IOCTL_MAGIC, 0x45, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_ZERO_4 _IOW(FPGA_IOCTL_MAGIC, 0x46, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_ZERO_5 _IOW(FPGA_IOCTL_MAGIC, 0x47, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_ZERO_6 _IOW(FPGA_IOCTL_MAGIC, 0x48, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_ZERO_7 _IOW(FPGA_IOCTL_MAGIC, 0x49, fpga_ao_gain_t)

#define FPGA_IOCTL_CONF_AO_GAIN_0 _IOW(FPGA_IOCTL_MAGIC, 0x4A, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_GAIN_1 _IOW(FPGA_IOCTL_MAGIC, 0x4B, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_GAIN_2 _IOW(FPGA_IOCTL_MAGIC, 0x4C, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_GAIN_3 _IOW(FPGA_IOCTL_MAGIC, 0x4D, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_GAIN_4 _IOW(FPGA_IOCTL_MAGIC, 0x4E, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_GAIN_5 _IOW(FPGA_IOCTL_MAGIC, 0x4F, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_GAIN_6 _IOW(FPGA_IOCTL_MAGIC, 0x50, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_GAIN_7 _IOW(FPGA_IOCTL_MAGIC, 0x51, fpga_ao_gain_t)
#define FPGA_IOCTL_CONF_AO_FORM _IOW(FPGA_IOCTL_MAGIC, 0x52, fpga_ao_form_t)

/* #define FPGA_IOCTL_TEST _IOW(FPGA_IOCTL_MAGIC, , struct fpga_ram) */

//////////////////////////////////////////////////////////

#define AI_PORT_NUM  8
#define AI_PORT_SIZE (AI_PORT_NUM/2)
#define TI_PORT_NUM 12
#define TI_PORT_SIZE (TI_PORT_NUM/2)
#define CNT_PORT_NUM 4
#define CNT_PORT_SIZE (CNT_PORT_NUM)
#define AO_PORT_NUM  8
#define AO_PORT_SIZE (AO_PORT_NUM)
#define DI_PORT_NUM  24
#define DI_WORDS     1
#define DO_PORT_NUM  40
#define DO_WORDS     2

#define DO_CONF_WORDS 2

typedef u32 BOOLEAN;

typedef u32 fpga_di_t;
typedef u32 fpga_di_cnf_t;
typedef u32 fpga_do_t;
typedef u32 fpga_do_cnf_t;
typedef u32 fpga_cnt_t;
typedef u32 fpga_cnt_cnf_t;
typedef u32 fpga_ai_t;
typedef u32 fpga_ai_cnf_t;
typedef u32 fpga_ti_t;
typedef u32 fpga_ti_cnf_t;
typedef BOOLEAN fpga_ao_gain_a_t;
typedef BOOLEAN fpga_ao_gain_b_t;
typedef BOOLEAN fpga_ao_sce_t;
typedef BOOLEAN fpga_ao_clear_t;
typedef u16 fpga_ao_offset_a_t;
typedef u16 fpga_ao_offset_b_t;
typedef u16 fpga_ao_zero_t;
typedef u16 fpga_ao_gain_t;
typedef BOOLEAN fpga_ao_form_t;

typedef u16 fpga_ao_t;

struct fpga_ram{
	fpga_di_t di_raw[DI_WORDS];
	fpga_cnt_t cnt_raw[CNT_PORT_SIZE];
	fpga_ai_t ai_raw[AI_PORT_SIZE];
	fpga_ti_t ti_raw[TI_PORT_SIZE];
};

//////////////////////////////////////////////////////////
#define FPGA_ADD_AO_CONFIGURE_REG 0x80

#define FPGA_ADD_TI_READ_01 0x10
#define FPGA_ADD_TI_READ_23 0x11
#define FPGA_ADD_TI_READ_45 0x12
#define FPGA_ADD_TI_READ_67 0x13
#define FPGA_ADD_TI_READ_89 0x14
#define FPGA_ADD_TI_READ_1011 0x15

#define FPGA_ADD_AI_READ_01 0x20
#define FPGA_ADD_AI_READ_23 0x21
#define FPGA_ADD_AI_READ_45 0x22
#define FPGA_ADD_AI_READ_67 0x23

#define FPGA_ADD_CNT_READ_0 0x40
#define FPGA_ADD_CNT_READ_1 0x41
#define FPGA_ADD_CNT_READ_2 0x42
#define FPGA_ADD_CNT_READ_3 0x43

#define FPGA_ADD_AO_WRITE_0 FPGA_ADD_AO_CONFIGURE_REG+0x8
#define FPGA_ADD_AO_WRITE_1 FPGA_ADD_AO_CONFIGURE_REG+0x9
#define FPGA_ADD_AO_WRITE_2 FPGA_ADD_AO_CONFIGURE_REG+0xa
#define FPGA_ADD_AO_WRITE_3 FPGA_ADD_AO_CONFIGURE_REG+0xb
#define FPGA_ADD_AO_WRITE_4 FPGA_ADD_AO_CONFIGURE_REG+0xc
#define FPGA_ADD_AO_WRITE_5 FPGA_ADD_AO_CONFIGURE_REG+0xd
#define FPGA_ADD_AO_WRITE_6 FPGA_ADD_AO_CONFIGURE_REG+0xe
#define FPGA_ADD_AO_WRITE_7 FPGA_ADD_AO_CONFIGURE_REG+0xf

#define FPGA_ADD_DI_READ_0023 0x30

#define FPGA_ADD_DO_WRITE_0019 0x60
#define FPGA_ADD_DO_WRITE_2039 0x61

////////////////////////////////////////////////////////////////////

#define FPGA_ADD_TI_CONF 0
#define FPGA_ADD_AI_HS_CONF 0x2
#define FPGA_ADD_AI_CONF 0x3
#define FPGA_ADD_DI_CONF 0x4
#define FPGA_ADD_DO_CONF_0019 0x5
#define FPGA_ADD_DO_CONF_2039 0x6
#define FPGA_ADD_CNT_CONF 0x7

#define FPGA_ADD_AO_CONF_GAIN_A FPGA_ADD_AO_CONFIGURE_REG
#define FPGA_ADD_AO_CONF_GAIN_B FPGA_ADD_AO_CONFIGURE_REG
#define FPGA_ADD_AO_CONF_SCE    FPGA_ADD_AO_CONFIGURE_REG
#define FPGA_ADD_AO_CONF_FORM 0xA0
#define FPGA_ADD_AO_CONF_CLEAR  0xA1 /* 与FPGA文档对应 */
#define FPGA_ADD_AO_CONF_OFFSET_A FPGA_ADD_AO_CONFIGURE_REG+3
#define FPGA_ADD_AO_CONF_OFFSET_B FPGA_ADD_AO_CONFIGURE_REG+4

#define FPGA_ADD_AO_CONF_ZERO_0 FPGA_ADD_AO_CONFIGURE_REG+0x10
#define FPGA_ADD_AO_CONF_ZERO_1 FPGA_ADD_AO_CONFIGURE_REG+0x11
#define FPGA_ADD_AO_CONF_ZERO_2 FPGA_ADD_AO_CONFIGURE_REG+0x12
#define FPGA_ADD_AO_CONF_ZERO_3 FPGA_ADD_AO_CONFIGURE_REG+0x13
#define FPGA_ADD_AO_CONF_ZERO_4 FPGA_ADD_AO_CONFIGURE_REG+0x14
#define FPGA_ADD_AO_CONF_ZERO_5 FPGA_ADD_AO_CONFIGURE_REG+0x15
#define FPGA_ADD_AO_CONF_ZERO_6 FPGA_ADD_AO_CONFIGURE_REG+0x16
#define FPGA_ADD_AO_CONF_ZERO_7 FPGA_ADD_AO_CONFIGURE_REG+0x17

#define FPGA_ADD_AO_CONF_GAIN_0 FPGA_ADD_AO_CONFIGURE_REG+0x18
#define FPGA_ADD_AO_CONF_GAIN_1 FPGA_ADD_AO_CONFIGURE_REG+0x19
#define FPGA_ADD_AO_CONF_GAIN_2 FPGA_ADD_AO_CONFIGURE_REG+0x1A
#define FPGA_ADD_AO_CONF_GAIN_3 FPGA_ADD_AO_CONFIGURE_REG+0x1B
#define FPGA_ADD_AO_CONF_GAIN_4 FPGA_ADD_AO_CONFIGURE_REG+0x1C
#define FPGA_ADD_AO_CONF_GAIN_5 FPGA_ADD_AO_CONFIGURE_REG+0x1D
#define FPGA_ADD_AO_CONF_GAIN_6 FPGA_ADD_AO_CONFIGURE_REG+0x1E
#define FPGA_ADD_AO_CONF_GAIN_7 FPGA_ADD_AO_CONFIGURE_REG+0x1F

#define DAC8228_AB 13
#define DAC8228_LD 12
#define DAC8228_RST 11
#define DAC8228_PDA 10
#define DAC8228_PDB 9
#define DAC8228_SCE 8
#define DAC8228_GBF 7
#define DAC8228_GAIN_A 6
#define DAC8228_GAIN_B 5

static u16 dac8228_config_reg = ((1<<DAC8228_AB) | (1<<DAC8228_LD) |
								 (0<<DAC8228_RST) | (0<<DAC8228_PDA) |
								 (0<<DAC8228_PDB) | (0<<DAC8228_SCE) |
								 (0<<DAC8228_GBF) | (1<<DAC8228_GAIN_A) |
								 (1<<DAC8228_GAIN_B));
#define ADS8568_WRITE_EN 31
#define ADS8568_READ_EN 30
#define ADS8568_CLKSEL 29
#define ADS8568_CLKOUT 28
#define ADS8568_BUSY_INT 27
#define ADS8568_BUSYPOL 26
#define ADS8568_STBY 25
#define ADS8568_RANGE_A 24
#define ADS8568_RANGE_B 23
#define ADS8568_PD_B 22
#define ADS8568_RANGE_C 21
#define ADS8568_PD_C 20
#define ADS8568_RANGE_D 19
#define ADS8568_PD_D 18
#define ADS8568_REFEN 15
#define ADS8568_REFBUF 14
#define ADS8568_VREF 13
#define ADS8568_D9 9
#define ADS8568_D8 8
#define ADS8568_D7 7
#define ADS8568_D6 6
#define ADS8568_D5 5
#define ADS8568_D4 4
#define ADS8568_D3 3
#define ADS8568_D2 2
#define ADS8568_D1 1
#define ADS8568_D0 0

/* static u32 ads8568_cofig_reg = ((0<<ADS8568_WRITE_EN) | (0<<ADS8568_READ_EN) | */
/* 								(0<<ADS8568_CLKSEL) | (0<<ADS8568_CLKOUT) | */
/* 								(0<<ADS8568_BUSY_INT) | (0<<ADS8568_BUSYPOL) | */
/* 								(0<<ADS8568_STBY) | (0<<ADS8568_RANGE_A) | */
/* 								(0<<ADS8568_RANGE_B) | (0<<ADS8568_PD_B) | */
/* 								(0<<ADS8568_RANGE_C) | (0<<ADS8568_PD_C) | */
/* 								(0<<ADS8568_RANGE_D) | (0<<ADS8568_PD_D) | */
/* 								(0<<ADS8568_REFEN) | (0<<ADS8568_REFBUF) | */
/* 								(0<<ADS8568_VREF) | (0<<ADS8568_D9) | */
/* 								(0<<ADS8568_D8) | (0<<ADS8568_D7) | */
/* 								(0<<ADS8568_D6) | (0<<ADS8568_D5) | */
/* 								(0<<ADS8568_D4) | (0<<ADS8568_D3) | */
/* 								(0<<ADS8568_D2) | (0<<ADS8568_D1)); */

////////////////////////////////////////////////////////////////

#define FPGA_MAJOR 0
#define FPGA_MINOR 0

int fpga_major = FPGA_MAJOR;
int fpga_minor = FPGA_MINOR;

module_param(fpga_major, int, S_IRUGO);
module_param(fpga_minor, int, S_IRUGO);

u16 fpga_add_do_conf[DO_CONF_WORDS] = {
	FPGA_ADD_DO_CONF_0019,
	FPGA_ADD_DO_CONF_2039
};

u16 fpga_add_ti_read[TI_PORT_SIZE] = {
	FPGA_ADD_TI_READ_01,
	FPGA_ADD_TI_READ_23,
	FPGA_ADD_TI_READ_45,
	FPGA_ADD_TI_READ_67,
	FPGA_ADD_TI_READ_89,
	FPGA_ADD_TI_READ_1011
};

u16 fpga_add_ai_read[AI_PORT_SIZE] = {
	FPGA_ADD_AI_READ_01,
	FPGA_ADD_AI_READ_23,
	FPGA_ADD_AI_READ_45,
	FPGA_ADD_AI_READ_67
};

u16 fpga_add_cnt_read[CNT_PORT_SIZE] = {
	FPGA_ADD_CNT_READ_0,
	FPGA_ADD_CNT_READ_1,
	FPGA_ADD_CNT_READ_2,
	FPGA_ADD_CNT_READ_3
};

u16 fpga_add_ao_write[AO_PORT_SIZE] = {
	FPGA_ADD_AO_WRITE_0,
	FPGA_ADD_AO_WRITE_1,
	FPGA_ADD_AO_WRITE_2,
	FPGA_ADD_AO_WRITE_3,
	FPGA_ADD_AO_WRITE_4,
	FPGA_ADD_AO_WRITE_5,
	FPGA_ADD_AO_WRITE_6,
	FPGA_ADD_AO_WRITE_7

};

u16 fpga_add_di_read[DI_WORDS] = {
	FPGA_ADD_DI_READ_0023
};

u16 fpga_add_do_write[DO_WORDS] = {
	FPGA_ADD_DO_WRITE_0019,
	FPGA_ADD_DO_WRITE_2039
};

ssize_t fpga_read(struct file *fp, char __user *buf, size_t size, loff_t *offset)
{
	return 0;
}

ssize_t fpga_write (struct file *fp, const char __user *buf, size_t size, loff_t *offset)
{
	return -EINVAL;
}

static inline int __get_ti(fpga_ti_t __user *const dest)
{
	int i;
	fpga_ti_t source[TI_PORT_SIZE];

	for (i = 0; i < TI_PORT_SIZE; i++)
		fpga_read32(fpga_add_ti_read[i], source+i);

	if(__copy_to_user(dest, source, sizeof(fpga_ti_t)*TI_PORT_SIZE))
		return -EFAULT;

	return 0;
}

/* 读所有温度 */
static inline int get_ti(fpga_ti_t __user *const dest)
{
	if (!access_ok(VERIFY_WRITE, dest, sizeof(fpga_ti_t)*TI_PORT_SIZE)
		|| __get_ti(dest))
		return -EFAULT;
	
	return 0;
}

static int put_ti_conf(fpga_ti_cnf_t __user *const source)
{
	fpga_ti_cnf_t dest = 0;

	if (copy_from_user(&dest, source, sizeof(fpga_ti_cnf_t)))
		return -EFAULT;
	fpga_write32(FPGA_ADD_TI_CONF, dest);

	return 0;
}

static inline int __get_ai(fpga_ai_t __user *const dest)
{
	int i;
	fpga_ai_t source[AI_PORT_SIZE];

	for (i = 0; i < AI_PORT_SIZE; i++)
		fpga_read32(fpga_add_ai_read[i], source+i);

	if(__copy_to_user(dest, source, sizeof(fpga_ai_t)*AI_PORT_SIZE))
		return -EFAULT;

	return 0;
}

static int get_ai(fpga_ai_t __user *const dest)
{
	if (!access_ok(VERIFY_WRITE, dest, sizeof(fpga_ai_t)*AI_PORT_SIZE)
		|| __get_ai(dest))
		return -EFAULT;

	return 0;
}

#define AI_HW 0
#define AI_SW 1
static int put_ai_conf(fpga_ai_cnf_t __user *const source)
{
	fpga_ai_cnf_t dest = 0;

	fpga_write32(FPGA_ADD_AI_HS_CONF, AI_SW);
	if (copy_from_user(&dest, source, sizeof(fpga_ai_cnf_t)))
		return -EFAULT;
	fpga_write32(FPGA_ADD_AI_CONF, dest);
	fpga_write32(FPGA_ADD_AI_HS_CONF, AI_HW);

	return 0;
}

static inline int __get_cnt(fpga_cnt_t __user *const dest)
{
	int i;
	fpga_cnt_t source[CNT_PORT_SIZE];

	for (i = 0; i < CNT_PORT_SIZE; i++)
		fpga_read32(fpga_add_cnt_read[i], source+i);

	if(__copy_to_user(dest, source, sizeof(fpga_cnt_t)*CNT_PORT_SIZE))
		return -EFAULT;

	return 0;
}

static int get_cnt(fpga_cnt_t __user *const dest)
{
	if (!access_ok(VERIFY_WRITE, dest, sizeof(fpga_cnt_t)*CNT_PORT_SIZE)
		|| __get_cnt(dest))
		return -EFAULT;

	return 0;
}

static int put_cnt_conf(fpga_cnt_cnf_t __user *const source)
{
	fpga_cnt_cnf_t dest = 0;

	if (copy_from_user(&dest, source, sizeof(fpga_cnt_cnf_t)))
		return -EFAULT;
	fpga_write32(FPGA_ADD_CNT_CONF, dest);

	return 0;
}

static inline int __get_di(fpga_di_t __user *const dest)
{
	int i;
	fpga_di_t source[DI_WORDS];

	for (i = 0; i < DI_WORDS; i++)
		fpga_read32(fpga_add_di_read[i], source+i);

	if (__copy_to_user(dest, source, sizeof(fpga_di_t)*DI_WORDS))
		return -EFAULT;

	return 0;
}

static int get_di(fpga_ai_t __user *const dest)
{
	if (!access_ok(VERIFY_WRITE, dest, sizeof(fpga_di_t)*DI_WORDS)
		|| __get_di(dest))
		return -EFAULT;

	return 0;
}

static int put_di_conf(fpga_di_cnf_t __user *const source)
{
	fpga_di_cnf_t dest = 0;

	if (copy_from_user(&dest, source, sizeof(fpga_di_cnf_t)))
		return -EFAULT;
	fpga_write32(FPGA_ADD_DI_CONF, dest);

	return 0;
}

static inline int __put_ao(fpga_ao_t __user *const source)
{
	int i;
	fpga_ao_t dest[AO_PORT_SIZE];

	if (__copy_from_user(dest, source, sizeof(fpga_ao_t)*AO_PORT_SIZE))
		return -EFAULT;

	for (i = 0; i < AO_PORT_SIZE; i++)
		fpga_write32(fpga_add_ao_write[i], dest[i]);

	return 0;
}

static int put_ao(fpga_ao_t __user *const source)
{
	if (!access_ok(VERIFY_READ, source, sizeof(fpga_ao_t)*AO_PORT_SIZE)
		|| __put_ao(source))
		return -EFAULT;

	return 0;
}

static inline int __put_do(fpga_do_t __user *const source)
{
	int i;
	fpga_do_t dest[DO_WORDS];

	if (__copy_from_user(dest, source, sizeof(fpga_do_t)*DO_WORDS))
		return -EFAULT;

	for (i = 0; i < DO_WORDS; i++)
		fpga_write32(fpga_add_do_write[i], dest[i]);

	return 0;
}

static int put_do(fpga_do_t __user *const source)
{
	if (!access_ok(VERIFY_READ, source, sizeof(fpga_do_t)*DO_WORDS)
		|| __put_do(source))
		return -EFAULT;

	return 0;
}

static int put_do_conf(fpga_do_cnf_t __user *const source)
{
	int i;
	fpga_do_cnf_t dest;

	if (copy_from_user(&dest, source, sizeof(fpga_do_cnf_t)*DO_CONF_WORDS))
		return -EFAULT;

	for (i = 0; i < DO_CONF_WORDS; i++)
		fpga_write32(fpga_add_do_conf[i], dest);

	return 0;
}

/* u8, u16, u32 */
#define PUT_CONF(type, arg, addr)							\
	({														\
		type __user *source = (type __user *)arg;			\
		type dest = 0;										\
															\
		if (copy_from_user(&dest, source, sizeof(type)))	\
			return -EFAULT;									\
		fpga_write32(addr, dest);							\
	})

#define MSG(format, args...) \
  printk("%s:%d" format "\n", __FUNCTION__, __LINE__,  ##args)

long fpga_unlocked_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	switch (cmd){
	case FPGA_IOCTL_ALL_IN:{
		/* MSG("FPGA_IOCTL_ALL_IN"); */
		struct fpga_ram *raw_data = (struct fpga_ram *)arg;
		if (!access_ok(VERIFY_WRITE, raw_data, sizeof(struct fpga_ram)))
			return -EFAULT;
		if (__get_ti(raw_data->ti_raw) || __get_ai(raw_data->ai_raw)
			|| __get_di(raw_data->di_raw) || __get_cnt(raw_data->cnt_raw))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_ALL_TI:{
		/* MSG("FPGA_IOCTL_ALL_TI"); */
		fpga_ti_t __user *ti_dat = (fpga_ti_t __user *)arg;
		if (get_ti(ti_dat))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_ALL_AI:{
		fpga_ai_t __user *ai_dat = (fpga_ai_t __user *)arg;
		if (get_ai(ai_dat))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_ALL_CNT:{
		fpga_cnt_t __user *cnt_dat = (fpga_cnt_t __user *)arg;
		if (get_cnt(cnt_dat))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_ALL_DI:{
		fpga_di_t __user *di_dat = (fpga_di_t __user *)arg;
		if (get_di(di_dat))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_ALL_AO:{
		fpga_ao_t __user *ao_dat = (fpga_ao_t __user *)arg;
		if (put_ao(ao_dat))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_ALL_DO:{
		fpga_do_t __user *do_dat = (fpga_do_t __user *)arg;
		if (put_do(do_dat))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_CONF_DI:{
		fpga_di_cnf_t __user *di_cnf = (fpga_di_cnf_t __user *)arg;
		if (put_di_conf(di_cnf))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_CONF_DO:{
		fpga_do_cnf_t __user *do_cnf = (fpga_do_cnf_t __user *)arg;
		if (put_do_conf(do_cnf))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_CONF_TI:{
		fpga_ti_cnf_t __user *ti_cnf = (fpga_ti_cnf_t __user *)arg;
		if (put_ti_conf(ti_cnf))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_CONF_AI:{
		fpga_ai_cnf_t __user *ai_cnf = (fpga_ai_cnf_t __user *)arg;
		if (put_ai_conf(ai_cnf))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_CONF_CNT:{
		fpga_cnt_cnf_t __user *cnt_cnf = (fpga_cnt_cnf_t __user *)arg;
		if (put_cnt_conf(cnt_cnf))
			return -EFAULT;
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_A:{
		if(arg)
			dac8228_config_reg |= 1<<DAC8228_GAIN_A;
		else
			dac8228_config_reg &= ~(1<<DAC8228_GAIN_A);
		fpga_write32(FPGA_ADD_AO_CONF_GAIN_A, dac8228_config_reg);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_B:{
		if(arg)
			dac8228_config_reg |= 1<<DAC8228_GAIN_B;
		else
			dac8228_config_reg &= ~(1<<DAC8228_GAIN_B);
		fpga_write32(FPGA_ADD_AO_CONF_GAIN_B, dac8228_config_reg);
		break;
	}
	case FPGA_IOCTL_CONF_AO_SCE:{
		if(arg)
			dac8228_config_reg |= 1<<DAC8228_SCE;
		else
			dac8228_config_reg &= ~(1<<DAC8228_SCE);
		fpga_write32(FPGA_ADD_AO_CONF_SCE, dac8228_config_reg);
		break;
	}
	case FPGA_IOCTL_CONF_AO_CLEAR:{
		PUT_CONF(fpga_ao_clear_t, arg, FPGA_ADD_AO_CONF_CLEAR);
		break;
	}
	case FPGA_ADD_AO_CONF_FORM:{
		PUT_CONF(fpga_ao_form_t, arg, FPGA_ADD_AO_CONF_FORM);
		break;
	}
	case FPGA_IOCTL_CONF_AO_OFFSET_A:{
		PUT_CONF(fpga_ao_offset_a_t, arg, FPGA_ADD_AO_CONF_OFFSET_A);
		break;
	}
	case FPGA_IOCTL_CONF_AO_OFFSET_B:{
		PUT_CONF(fpga_ao_offset_b_t, arg, FPGA_ADD_AO_CONF_OFFSET_B);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_0:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_0);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_1:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_1);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_2:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_2);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_3:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_3);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_4:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_4);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_5:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_5);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_6:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_6);
		break;
	}
	case FPGA_IOCTL_CONF_AO_ZERO_7:{
		PUT_CONF(fpga_ao_zero_t, arg, FPGA_ADD_AO_CONF_ZERO_7);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_0:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_0);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_1:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_1);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_2:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_2);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_3:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_3);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_4:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_4);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_5:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_5);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_6:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_6);
		break;
	}
	case FPGA_IOCTL_CONF_AO_GAIN_7:{
		PUT_CONF(fpga_ao_gain_t, arg, FPGA_ADD_AO_CONF_GAIN_7);
		break;
	}
	default :
		printk(KERN_WARNING "fpga: ERROR ioctl command : %u.\n", cmd);
		return -EINVAL;
		break;
	}

	return 0;
}

int fpga_open (struct inode *node, struct file *fp)
{
	printk(KERN_NOTICE "fpga_open\n");
	return 0;
}

static const struct file_operations fpga_file_operation = {
	.owner = THIS_MODULE,
	.open = fpga_open,
	.read = fpga_read,
	.write = fpga_write,
	.unlocked_ioctl = fpga_unlocked_ioctl,
};

struct cdev *fpga_cdev;

static int __init fpga_init(void)
{
	dev_t fpga_dev;

 	if (fpga_major){
 		fpga_dev = MKDEV(fpga_major, fpga_minor);
 		if(register_chrdev_region(fpga_dev, 1, "fpga"))
 			goto register_dev_err;
 	}else {
 		if(!alloc_chrdev_region(&fpga_dev, 0, 1, "fpga"))
 			fpga_major = MAJOR(fpga_dev);
 		else
 			goto alloc_chrdev_region_err;
 	}

 	if ((fpga_cdev = cdev_alloc())){
 		fpga_cdev->owner = THIS_MODULE;
 		fpga_cdev->ops = &fpga_file_operation;
 		if (cdev_add(fpga_cdev, fpga_dev, 1))
 			goto cdev_add_err;
 	}
 	else
 		goto cdev_alloc_err;

	printk(KERN_NOTICE "major:%d, minor:%d\n", MAJOR(fpga_dev), MINOR(fpga_dev));

 	return 0;

 cdev_add_err:
	printk(KERN_NOTICE "major:%d, minor:%d\n", MAJOR(fpga_dev), MINOR(fpga_dev));
	printk(KERN_NOTICE "cdev_add_err\n");
 	cdev_del(fpga_cdev);
 	goto cdev_alloc_err;
 	/* return -EFAULT; */
 alloc_chrdev_region_err:
 register_dev_err:
	printk(KERN_NOTICE "major:%d, minor:%d\n", MAJOR(fpga_dev), MINOR(fpga_dev));
	printk(KERN_NOTICE "register_dev_err, alloc_chrdev_region_err\n");
 	return -EFAULT;
 cdev_alloc_err:
	printk(KERN_NOTICE "major:%d, minor:%d\n", MAJOR(fpga_dev), MINOR(fpga_dev));
	printk(KERN_NOTICE "cdev_alloc_err\n");
 	unregister_chrdev_region(fpga_major, 1);
 	return 0;
}

static void __exit fpga_exit(void)
{
	printk(KERN_NOTICE "fpga_exit\n");
	cdev_del(fpga_cdev);
	unregister_chrdev_region(fpga_major, 1);
}

module_init(fpga_init);
module_exit(fpga_exit);
MODULE_LICENSE("Dual BSD/GPL");
