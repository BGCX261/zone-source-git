/* 原来spi_device->controller_state里放的是spi_bitbang_cs结构体 */
/* 一个spi_device对应一个spi_bitbang_cs因为没这device的控制时序是一定的且互相之间是不一样的 */
/* 从spi_bitbang_setup这个函数可以看出spi_master.dev.p.driver_data是spi_bitbang,也就是说
   spi_bitbang和spi_mastar之间的关系是一一对应的 */
/*
 * FIRST PART (OPTIONAL):  word-at-a-time spi_transfer support.
 * Use this for GPIO or shift-register level hardware APIs.
 *
 * spi_bitbang_cs is in spi_device->controller_state, which is unavailable
 * to glue code.  These bitbang setup() and cleanup() routines are always
 * used, though maybe they're called from controller-aware code.
 *
 * chipselect() and friends may use use spi_device->controller_data and
 * controller registers as appropriate.
 *
 *
 * NOTE:  SPI controller pins can often be used as GPIO pins instead,
 * which means you could use a bitbang driver either to get hardware
 * working quickly, or testing for differences that aren't speed related.
 */
/* txrx_bufs成员会在spi_bitbang_setup_transfer 里会根椐bits_per_word 被设置成bitbang_txrx_8/16/32函数 */
struct spi_bitbang_cs {
	unsigned	nsecs;	/* (clock cycle time)/2 */
	u32		(*txrx_word)(struct spi_device *spi, unsigned nsecs,
					u32 word, u8 bits);
	unsigned	(*txrx_bufs)(struct spi_device *,
					u32 (*txrx_word)(
						struct spi_device *spi,
						unsigned nsecs,
						u32 word, u8 bits),
					unsigned, struct spi_transfer *);
};

/* txrx_word有4个数组对应CPOL和CPHA的组合，这四个成员要根据spi_device的成员的mode来选择合适的元素赋给spi_bitbang_cs.txrx_word,
   而spi_bitbang_cs.txrx_bufs根据spi_tranfser.bits_per_word的值会赋予bitbang_txrx_8或bitbang_txrx_16或bitbang_txrx_32,
   但是为什么txrx_bufs里有一个txrx_word的函数参数呢 ?其实spi_bitbang_cs.txrx_bufs是spi_bitbang_cs.txrx_word的一个封装，因为
   spi_bitbang_cs.txrx_buf是bitbang_txrx_8/16/32其中的一个，而在spi_bitbang_bufs（默认的bitbang.txrx_bufs函数）函数里调用
   spi_bitbang_cs.txrx_buf时会传以spi_bitbang_cs.txrx_word，而spi_bitbang_cs.txrx_word是会被赋予spi_bitbang.txrx_word的
   其中一个。
*/
/* spi_bitbang.txrx_bufs就是最终调用spi_bitbang_cs里的txrx_bufs，就看当前要传送那个spi_bitbang_cs的数据了 */
/* spi_bitbang.setup_transfer并不一定是spi_bitbang_setup_transfer函数但spi_gpio.spi_bitbang.setup_tranfer就是 */
struct spi_bitbang {
	struct workqueue_struct	*workqueue;
	struct work_struct	work;

	spinlock_t		lock;
	struct list_head	queue;
	u8			busy;
	u8			use_dma;
	u8			flags;		/* extra spi->mode support */

	struct spi_master	*master;

	/* setup_transfer() changes clock and/or wordsize to match settings
	 * for this transfer; zeroes restore defaults from spi_device.
	 */
	int	(*setup_transfer)(struct spi_device *spi,
			struct spi_transfer *t);

	void	(*chipselect)(struct spi_device *spi, int is_on);
#define	BITBANG_CS_ACTIVE	1	/* normally nCS, active low */
#define	BITBANG_CS_INACTIVE	0

	/* txrx_bufs() may handle dma mapping for transfers that don't
	 * already have one (transfer.{tx,rx}_dma is zero), or use PIO
	 */
	int	(*txrx_bufs)(struct spi_device *spi, struct spi_transfer *t);

	/* txrx_word[SPI_MODE_*]() just looks like a shift register */
	u32	(*txrx_word[4])(struct spi_device *spi,
			unsigned nsecs,
			u32 word, u8 bits);
};

/* 这个函数设计有点意思，把发送和接收都用同一个函数，且放到一个while循环里 */
static unsigned bitbang_txrx_8(
							   struct spi_device	*spi,
							   u32			(*txrx_word)(struct spi_device *spi,
														 unsigned nsecs,
														 u32 word, u8 bits),
							   unsigned		ns,
							   struct spi_transfer	*t
							   );
/* 与上一个函数类似，只是bits的不同 */
static unsigned bitbang_txrx_16(
								struct spi_device	*spi,
								u32			(*txrx_word)(struct spi_device *spi,
														 unsigned nsecs,
														 u32 word, u8 bits),
								unsigned		ns,
								struct spi_transfer	*t
								);

static unsigned bitbang_txrx_32(
								struct spi_device	*spi,
								u32			(*txrx_word)(struct spi_device *spi,
														 unsigned nsecs,
														 u32 word, u8 bits),
								unsigned		ns,
								struct spi_transfer	*t
								);

		
/* 主要是设置：spi_bitbang_cs的txrx_bufs和nsecs */
int spi_bitbang_setup_transfer(struct spi_device *spi, struct spi_transfer *t);

/* 为什么会在这个函数里当总线不忙时要不激活呢？原来有注释：
   NOTE we _need_ to call chipselect() early, ideally with adapter
   setup, unless the hardware defaults cooperate to avoid confusion
   between normal (active low) and inverted chipselects.
*/
/* 做4件事：分配spi_bitbang_cs,
   设置spi_bitbang_cs.txrx_word,
   调用spi_bitbang.setup_transfer，
   调用spi_bitbang.chipselect */
/* 在davinci_spi.c里的spi_bitbang.setup_transfer不使用这个默认的spi_bitbang_setup函数，spi_bitbang.txrx_bufs也同理，也不使用默认的,

   bitbang->master->transfer = spi_bitbang_transfer,
   bitbang->txrx_bufs = spi_bitbang_bufs;
   bitbang->setup_transfer = spi_bitbang_setup_transfer;
   bitbang->master->setup = spi_bitbang_setup;
   bitbang->master->cleanup = spi_bitbang_cleanup;
   这些spi_bitbang*函数就是默认的，在spi_bitbang_start设置，而spi_bitbang_start在davinci_spi_probe里被调用。
*/
int spi_bitbang_setup(struct spi_device *spi);

/* 从这个函数可以看出bitbang.queue是spi_message的链表,而spi_message里的queue做为链表的节点 */
/* 而这个函数会把queue中所有的spi_message给发送完 */
/* 这个函数怎么和spi_bitbang_cs一点关系都没有呢? */
/* 主要就是调用了bitbang.txrx_bufs, bitbang.chipselect, spi_transfer.delay_usecs */
static void bitbang_work(struct work_struct *work);
/**
 * spi_bitbang_transfer - default submit to transfer queue
 */
/* 仅仅把数据加到队列里而已 */
int spi_bitbang_transfer(struct spi_device *spi, struct spi_message *m);


/**
 * spi_bitbang_start - start up a polled/bitbanging SPI master driver
 * @bitbang: driver handle
 *
 * Caller should have zero-initialized all parts of the structure, and then
 * provided callbacks for chip selection and I/O loops.  If the master has
 * a transfer method, its final step should call spi_bitbang_transfer; or,
 * that's the default if the transfer routine is not initialized.  It should
 * also set up the bus number and number of chipselects.
 *
 * For i/o loops, provide callbacks either per-word (for bitbanging, or for
 * hardware that basically exposes a shift register) or per-spi_transfer
 * (which takes better advantage of hardware like fifos or DMA engines).
 *
 * Drivers using per-word I/O loops should use (or call) spi_bitbang_setup,
 * spi_bitbang_cleanup and spi_bitbang_setup_transfer to handle those spi
 * master methods.  Those methods are the defaults if the bitbang->txrx_bufs
 * routine isn't initialized.
 *
 * This routine registers the spi_master, which will process requests in a
 * dedicated task, keeping IRQs unblocked most of the time.  To stop
 * processing those requests, call spi_bitbang_stop().
 */
/* 从这个函数可以看出bitbang.chipselect好像是没有默认的 */
/* 这函数就是初始化spi_bitbang的 */
int spi_bitbang_start(struct spi_bitbang *bitbang);

/**
 * spi_bitbang_stop - stops the task providing spi communication
 */
int spi_bitbang_stop(struct spi_bitbang *bitbang);
