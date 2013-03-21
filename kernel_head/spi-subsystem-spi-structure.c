/* 搞清楚spi_message, spi_device, spi_master函数成员和几个bitbang实体函数之间的关系 */
/* 要记住这些结构体和成员 */
/* 要认清这些结构体之间的对应关系，是1对0，还是1对1，还是1对n */
/* spi_master:1 - spi_driver:1 - spi_message:1 - spi_transfer:n;  spi_driver:1 - spi_device:n */
/* struct spi_device指的是一个slave设备,里面的成员是对这个slave的描述，不是对master的，注意分清。 */
/* max_speed_hz:设备的driver可以改变这个的值，若spi_transfer.speed_hz不为0，则会用这个默认的，否则会被重写 */
/* chip_select:设备所连到的片选信号 */
/* mode:指定数据是如何进出的。SPI_CPHA:clock phase, SPI_CPOL:clock polarity, SPI_MODE_0, SPI_MODE_1, SPI_MODE_2, SPI_MODE_3:就是SPI_CPHA和SPI_CPOL的组合, SPI_CS_HIGH:在高电平时激活片选, SPI_LSB_FIRST:先移低位,不管是输入还出, SPI_3WIRE:就是半双工,输入和输出线为同一条线, SPI_LOOP:loopback模式 SPI_NO_CS:没有片选，那么chip_select成员还有效吗? SPI_READY:拉低来停止，拉低什么呢？ */
/* bit_per_word:对于spi来说，一个字的位数可以不定的，用这个成员来指定，但是不是没次发送就只发一个字。driver也可以修改，0表示用8，spi_transfer.bits_per_word可以重写它，与mas_speed_hz类似 */
/* irq:负数，或传给request_irq()的中断号。 */
/* controller_state:控制器的运行状态，不知道有什么用，举个例子。 */
/* controller_data:为控制器指定的特定板上的定义，如FIFO初始化参数，从board_info.controller_data来 */
/* modalias:名字，在sysfs的modalias中 */
/* dev成员里的platform_data用来保存对设备的驱动对于该设备的有用的信息 */
/* 还有一些附加的成员如内存包，优先级，片选延时 */
/* 为什么是spi_device有irq而不是master有irq呢？ */
/**
 * struct spi_device - Master side proxy for an SPI slave device
 * @dev: Driver model representation of the device.
 * @master: SPI controller used with the device.
 * @max_speed_hz: Maximum clock rate to be used with this chip
 *	(on this board); may be changed by the device's driver.
 *	The spi_transfer.speed_hz can override this for each transfer.
 * @chip_select: Chipselect, distinguishing chips handled by @master.
 * @mode: The spi mode defines how data is clocked out and in.
 *	This may be changed by the device's driver.
 *	The "active low" default for chipselect mode can be overridden
 *	(by specifying SPI_CS_HIGH) as can the "MSB first" default for
 *	each word in a transfer (by specifying SPI_LSB_FIRST).
 * @bits_per_word: Data transfers involve one or more words; word sizes
 *	like eight or 12 bits are common.  In-memory wordsizes are
 *	powers of two bytes (e.g. 20 bit samples use 32 bits).
 *	This may be changed by the device's driver, or left at the
 *	default (0) indicating protocol words are eight bit bytes.
 *	The spi_transfer.bits_per_word can override this for each transfer.
 * @irq: Negative, or the number passed to request_irq() to receive
 *	interrupts from this device.
 * @controller_state: Controller's runtime state
 * @controller_data: Board-specific definitions for controller, such as
 *	FIFO initialization parameters; from board_info.controller_data
 * @modalias: Name of the driver to use with this device, or an alias
 *	for that name.  This appears in the sysfs "modalias" attribute
 *	for driver coldplugging, and in uevents used for hotplugging
 *
 * A @spi_device is used to interchange data between an SPI slave
 * (usually a discrete chip) and CPU memory.
 *
 * In @dev, the platform_data is used to hold information about this
 * device that's meaningful to the device's protocol driver, but not
 * to its controller.  One example might be an identifier for a chip
 * variant with slightly different functionality; another might be
 * information about how this particular board wires the chip's pins.
 */
struct spi_device {
	struct device		dev;
	struct spi_master	*master;
	u32			max_speed_hz;
	u8			chip_select;
	u8			mode;
#define	SPI_CPHA	0x01			/* clock phase */
#define	SPI_CPOL	0x02			/* clock polarity */
#define	SPI_MODE_0	(0|0)			/* (original MicroWire) */
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			/* chipselect active high? */
#define	SPI_LSB_FIRST	0x08			/* per-word bits-on-wire */
#define	SPI_3WIRE	0x10			/* SI/SO signals shared */
#define	SPI_LOOP	0x20			/* loopback mode */
#define	SPI_NO_CS	0x40			/* 1 dev/bus, no chipselect */
#define	SPI_READY	0x80			/* slave pulls low to pause */
	u8			bits_per_word;
	int			irq;
	void			*controller_state;
	void			*controller_data;
	char			modalias[SPI_NAME_SIZE];

	/*
	 * likely need more hooks for more protocol options affecting how
	 * the controller talks to each chip, like:
	 *  - memory packing (12 bit samples into low bits, others zeroed)
	 *  - priority
	 *  - drop chipselect after each word
	 *  - chipselect delays
	 *  - ...
	 */
}

/* struct spi_driver:叫protocol driver,协议驱动 */
/* 驱动用 SPI messages(spi_message)与设备交互，之所以就protocol驱动是因为通过message而不是直接与SPI硬件打交道。 */
/* 这些设备协议代表着由驱动支持的最底层的接口，它可以支持更上层的接口，如MTD框架，网络，MMC，RTC，文件系统字符节点和硬件监测 */
/* id_table:这个成员的类有点不懂，表示驱动支持的设备链表 */
/* probe:把驱动绑定的设备，用来校验设备和设置设备特征，spi_device里是没有与驱动关连的成员，而spi_driver里有，那么如何通过一个spi_device找到spi_driver呢 */
/* remove:解除绑定 */
/* shutdown:关机回调函数 */
/* suspend:挂起回调函数 */
/* resume:恢复函数 */
/* driver:struct device_driver */
/**
 * struct spi_driver - Host side "protocol" driver
 * @id_table: List of SPI devices supported by this driver
 * @probe: Binds this driver to the spi device.  Drivers can verify
 *	that the device is actually present, and may need to configure
 *	characteristics (such as bits_per_word) which weren't needed for
 *	the initial configuration done during system setup.
 * @remove: Unbinds this driver from the spi device
 * @shutdown: Standard shutdown callback used during system state
 *	transitions such as powerdown/halt and kexec
 * @suspend: Standard suspend callback used during system state transitions
 * @resume: Standard resume callback used during system state transitions
 * @driver: SPI device drivers should initialize the name and owner
 *	field of this structure.
 *
 * This represents the kind of device driver that uses SPI messages to
 * interact with the hardware at the other end of a SPI link.  It's called
 * a "protocol" driver because it works through messages rather than talking
 * directly to SPI hardware (which is what the underlying SPI controller
 * driver does to pass those messages).  These protocols are defined in the
 * specification for the device(s) supported by the driver.
 *
 * As a rule, those device protocols represent the lowest level interface
 * supported by a driver, and it will support upper level interfaces too.
 * Examples of such upper levels include frameworks like MTD, networking,
 * MMC, RTC, filesystem character device nodes, and hardware monitoring.
 */
struct spi_driver {
	const struct spi_device_id *id_table;
	int			(*probe)(struct spi_device *spi);
	int			(*remove)(struct spi_device *spi);
	void			(*shutdown)(struct spi_device *spi);
	int			(*suspend)(struct spi_device *spi, pm_message_t mesg);
	int			(*resume)(struct spi_device *spi);
	struct device_driver	driver;
};

/* struct spi_master */
/* 也有struct device结构体，但没有spi_device */
/* SPI主控制器的接口，一个链表，两个num,两个flag，3个锁，3个函数 */
/* 应该是一个master对应一个driver，但是spi_driver里有一个id_table成员又是怎么回事呢？ */
/* dev:接到驱动的设备 */
/* list:spi_master链表,spi_driver也有一个设备链表 */
/* bus_num:SPI控制器在板上或SOC上的标识 */
/* num_chipselect:支持的片选数从0开始 */
/* dma_alignment:SPI控制器对DMA对齐的约束 */
/* mode_bits:控制器驱动明白的标志 */
/* flags:相对于这个驱动的其它约束 */
/* bus_lock_spinlock:SPI 总线锁 */
/* bus_lock_mutex:SPI 总线的信号量，为什么有了自旋锁还要信号量呢 */
/* bus_lock_flag:指明是否正在使用SPI总线 */
/* setup:一个设备的SPI控制器用来更新设备的模式和clocking(时间控制) records, spi driver可能会调用很多次这个函数来修改寄存器设置因为每个slave对设置的要求是不一样的，注意不能在传输的过程中对共享的寄存器修改,注意参数是一个spi_device */
/* transfer:添加一个message到控制器的发送队列，是一个slave设备还是一个master还是一个driver拥有一个发送队列，transfer()是不休眠的，只是把message加到queue */
/* cleanup:释放控制器特定的状态，注意是状态 */
/* 一个SPI的控制器的驱动通过一个spi_message队列(spi_message就是一个队列)来管理发送到从机和从从机接收数据的访问，发送完后用message的completion函数通知完成 */
/**
 * struct spi_master - interface to SPI master controller
 * @dev: device interface to this driver
 * @list: link with the global spi_master list
 * @bus_num: board-specific (and often SOC-specific) identifier for a
 *	given SPI controller.
 * @num_chipselect: chipselects are used to distinguish individual
 *	SPI slaves, and are numbered from zero to num_chipselects.
 *	each slave has a chipselect signal, but it's common that not
 *	every chipselect is connected to a slave.
 * @dma_alignment: SPI controller constraint on DMA buffers alignment.
 * @mode_bits: flags understood by this controller driver
 * @flags: other constraints relevant to this driver
 * @bus_lock_spinlock: spinlock for SPI bus locking
 * @bus_lock_mutex: mutex for SPI bus locking
 * @bus_lock_flag: indicates that the SPI bus is locked for exclusive use
 * @setup: updates the device mode and clocking records used by a
 *	device's SPI controller; protocol code may call this.  This
 *	must fail if an unrecognized or unsupported mode is requested.
 *	It's always safe to call this unless transfers are pending on
 *	the device whose settings are being modified.
 * @transfer: adds a message to the controller's transfer queue.
 * @cleanup: frees controller-specific state
 *
 * Each SPI master controller can communicate with one or more @spi_device
 * children.  These make a small bus, sharing MOSI, MISO and SCK signals
 * but not chip select signals.  Each device may be configured to use a
 * different clock rate, since those shared signals are ignored unless
 * the chip is selected.
 *
 * The driver for an SPI controller manages access to those devices through
 * a queue of spi_message transactions, copying data between CPU memory and
 * an SPI slave device.  For each such message it queues, it calls the
 * message's completion function when the transaction completes.
 */
struct spi_master {
	struct device	dev;

	struct list_head list;

	/* other than negative (== assign one dynamically), bus_num is fully
	 * board-specific.  usually that simplifies to being SOC-specific.
	 * example:  one SOC has three SPI controllers, numbered 0..2,
	 * and one board's schematics might show it using SPI-2.  software
	 * would normally use bus_num=2 for that controller.
	 */
	s16			bus_num;

	/* chipselects will be integral to many controllers; some others
	 * might use board-specific GPIOs.
	 */
	u16			num_chipselect;

	/* some SPI controllers pose alignment requirements on DMAable
	 * buffers; let protocol drivers know about these requirements.
	 */
	u16			dma_alignment;

	/* spi_device.mode flags understood by this controller driver */
	u16			mode_bits;

	/* other constraints relevant to this driver */
	u16			flags;
#define SPI_MASTER_HALF_DUPLEX	BIT(0)		/* can't do full duplex */
#define SPI_MASTER_NO_RX	BIT(1)		/* can't do buffer read */
#define SPI_MASTER_NO_TX	BIT(2)		/* can't do buffer write */

	/* lock and mutex for SPI bus locking */
	spinlock_t		bus_lock_spinlock;
	struct mutex		bus_lock_mutex;

	/* flag indicating that the SPI bus is locked for exclusive use */
	bool			bus_lock_flag;

	/* Setup mode and clock, etc (spi driver may call many times).
	 *
	 * IMPORTANT:  this may be called when transfers to another
	 * device are active.  DO NOT UPDATE SHARED REGISTERS in ways
	 * which could break those transfers.
	 */
	int			(*setup)(struct spi_device *spi);
	/* bidirectional bulk transfers
	 *
	 * + The transfer() method may not sleep; its main role is
	 *   just to add the message to the queue.
	 * + For now there's no remove-from-queue operation, or
	 *   any other request management
	 * + To a given spi_device, message queueing is pure fifo
	 *
	 * + The master's main job is to process its message queue,
	 *   selecting a chip then transferring data
	 * + If there are multiple spi_device children, the i/o queue
	 *   arbitration algorithm is unspecified (round robin, fifo,
	 *   priority, reservations, preemption, etc)
	 *
	 * + Chipselect stays active during the entire message
	 *   (unless modified by spi_transfer.cs_change != 0).
	 * + The message transfers use clock and SPI mode parameters
	 *   previously established by setup() for this device
	 */
	int			(*transfer)(struct spi_device *spi,
						struct spi_message *mesg);

	/* called on release() to free memory provided by spi_master */
	void			(*cleanup)(struct spi_device *spi);
};

/* spi_transfer:一对读写buffer */
/* tx_buf, rx_buf,  */
/* tx_dma, rx_dma:都根据spi_message.is_dma_mapped */
/* len:rx和tx buffer的大小总和，以字为单位 */
/* speed_hz:spi_device有说过 */
/* bits_per_word:spi_device有说过 */
/* cs_change:在这个发送完成后对片选信号的作用 */
/* delay_usecs:在传完之后或改变片选之前（可选）延时这个时间时间后开始下一次的传输或完成(completion函数)这次的spi_message */
/* transfer_list:加入到spi_message.transfers链表的节点 */
/* SPI传送写的字数总是和读的字数一样 */
/* 对于接收和发送只有一条线的，tx和rx里一定有一个null */
/* dma传输要用dma_*map_single() */
/**
 * struct spi_transfer - a read/write buffer pair
 * @tx_buf: data to be written (dma-safe memory), or NULL
 * @rx_buf: data to be read (dma-safe memory), or NULL
 * @tx_dma: DMA address of tx_buf, if @spi_message.is_dma_mapped
 * @rx_dma: DMA address of rx_buf, if @spi_message.is_dma_mapped
 * @len: size of rx and tx buffers (in bytes)
 * @speed_hz: Select a speed other than the device default for this
 *      transfer. If 0 the default (from @spi_device) is used.
 * @bits_per_word: select a bits_per_word other than the device default
 *      for this transfer. If 0 the default (from @spi_device) is used.
 * @cs_change: affects chipselect after this transfer completes
 * @delay_usecs: microseconds to delay after this transfer before
 *	(optionally) changing the chipselect status, then starting
 *	the next transfer or completing this @spi_message.
 * @transfer_list: transfers are sequenced through @spi_message.transfers
 *
 * SPI transfers always write the same number of bytes as they read.
 * Protocol drivers should always provide @rx_buf and/or @tx_buf.
 * In some cases, they may also want to provide DMA addresses for
 * the data being transferred; that may reduce overhead, when the
 * underlying driver uses dma.
 *
 * If the transmit buffer is null, zeroes will be shifted out
 * while filling @rx_buf.  If the receive buffer is null, the data
 * shifted in will be discarded.  Only "len" bytes shift out (or in).
 * It's an error to try to shift out a partial word.  (For example, by
 * shifting out three bytes with word size of sixteen or twenty bits;
 * the former uses two bytes per word, the latter uses four bytes.)
 *
 * In-memory data values are always in native CPU byte order, translated
 * from the wire byte order (big-endian except with SPI_LSB_FIRST).  So
 * for example when bits_per_word is sixteen, buffers are 2N bytes long
 * (@len = 2N) and hold N sixteen bit words in CPU byte order.
 *
 * When the word size of the SPI transfer is not a power-of-two multiple
 * of eight bits, those in-memory words include extra bits.  In-memory
 * words are always seen by protocol drivers as right-justified, so the
 * undefined (rx) or unused (tx) bits are always the most significant bits.
 *
 * All SPI transfers start with the relevant chipselect active.  Normally
 * it stays selected until after the last transfer in a message.  Drivers
 * can affect the chipselect signal using cs_change.
 *
 * (i) If the transfer isn't the last one in the message, this flag is
 * used to make the chipselect briefly go inactive in the middle of the
 * message.  Toggling chipselect in this way may be needed to terminate
 * a chip command, letting a single spi_message perform all of group of
 * chip transactions together.
 *
 * (ii) When the transfer is the last one in the message, the chip may
 * stay selected until the next transfer.  On multi-device SPI busses
 * with nothing blocking messages going to other devices, this is just
 * a performance hint; starting a message to another device deselects
 * this one.  But in other cases, this can be used to ensure correctness.
 * Some devices need protocol transactions to be built from a series of
 * spi_message submissions, where the content of one message is determined
 * by the results of previous messages and where the whole transaction
 * ends when the chipselect goes intactive.
 *
 * The code that submits an spi_message (and its spi_transfers)
 * to the lower layers is responsible for managing its memory.
 * Zero-initialize every field you don't set up explicitly, to
 * insulate against future API updates.  After you submit a message
 * and its transfers, ignore them until its completion callback.
 */
struct spi_transfer {
	/* it's ok if tx_buf == rx_buf (right?)
	 * for MicroWire, one buffer must be null
	 * buffers must work with dma_*map_single() calls, unless
	 *   spi_message.is_dma_mapped reports a pre-existing mapping
	 */
	const void	*tx_buf;
	void		*rx_buf;
	unsigned	len;

	dma_addr_t	tx_dma;
	dma_addr_t	rx_dma;

	unsigned	cs_change:1;
	u8		bits_per_word;
	u16		delay_usecs;
	u32		speed_hz;

	struct list_head transfer_list;
};

/* struct spi_message:一个多段SPI传输 */
/* list_head:transfer的链表头 */
/* spi:指向转送数据的SPI device，是struct spi_device结构体，那么就是说一个spi_device对应一个spi_message了，不是一个spi_master对应一个spi_message，注意在spi_transfer里是没有spi_device的，而spi_master里的transfer函数指定了spi_device和spi_message参数但没有spi_transfer?哦，明白了，就是一个spi_message是属于一个spi_device的，一个spi_message里可以有多个spi_transfer(transfer成员)，在调用spi_master.transfer之前要把所有的spi_transfer放到spi_message里，那可不可以用调用transfer之后再在spi_message里加spi_transfer呢？那么是不是在一个master里，一个时刻最多只能有一个spi_message的？是不是要用到总结锁呢？ */
/* is_dma_mapped: 为真表示提供DMA地址，这个标志不是在spi_transfer里的 */
/* complete:完成传送的回调函数 */
/* context:complete函数的参数 */
/* actual_length:累计所有成功传送的段的字节数 */
/* status:0是成功，负数是有错，这个状态是对于所有的transfer来说的，就是这个spi_message来说的 */
/* queue:被拥有这个队列的驱动使用 */
/* state:被拥有这个队列的驱动使用 */
/* 的确,一个spi_message是用来执行一个原子序列的数据传送,一个spi_message只属于一个spi_device（slave）,在这个spi_message里的所有spi_transfer以FIFO顺序发送 */
/* 在spi_bitbang.c里的bitbang_work函数可以看出queue成员是给bitbang用的，从spi_bitbang_transfer的list_add_tail(&m->queue, &bitbang->queue);可以看出bitbang_queue是链表头把spi_message给链起来 */
/**
 * struct spi_message - one multi-segment SPI transaction
 * @transfers: list of transfer segments in this transaction
 * @spi: SPI device to which the transaction is queued
 * @is_dma_mapped: if true, the caller provided both dma and cpu virtual
 *	addresses for each transfer buffer
 * @complete: called to report transaction completions
 * @context: the argument to complete() when it's called
 * @actual_length: the total number of bytes that were transferred in all
 *	successful segments
 * @status: zero for success, else negative errno
 * @queue: for use by whichever driver currently owns the message
 * @state: for use by whichever driver currently owns the message
 *
 * A @spi_message is used to execute an atomic sequence of data transfers,
 * each represented by a struct spi_transfer.  The sequence is "atomic"
 * in the sense that no other spi_message may use that SPI bus until that
 * sequence completes.  On some systems, many such sequences can execute as
 * as single programmed DMA transfer.  On all systems, these messages are
 * queued, and might complete after transactions to other devices.  Messages
 * sent to a given spi_device are alway executed in FIFO order.
 *
 * The code that submits an spi_message (and its spi_transfers)
 * to the lower layers is responsible for managing its memory.
 * Zero-initialize every field you don't set up explicitly, to
 * insulate against future API updates.  After you submit a message
 * and its transfers, ignore them until its completion callback.
 */
struct spi_message {
	struct list_head	transfers;

	struct spi_device	*spi;

	unsigned		is_dma_mapped:1;

	/* REVISIT:  we might want a flag affecting the behavior of the
	 * last transfer ... allowing things like "read 16 bit length L"
	 * immediately followed by "read L bytes".  Basically imposing
	 * a specific message scheduling algorithm.
	 *
	 * Some controller drivers (message-at-a-time queue processing)
	 * could provide that as their default scheduling algorithm.  But
	 * others (with multi-message pipelines) could need a flag to
	 * tell them about such special cases.
	 */

	/* completion is reported through a callback */
	void			(*complete)(void *context);
	void			*context;
	unsigned		actual_length;
	int			status;

	/* for optional use by whatever driver currently owns the
	 * spi_message ...  between calls to spi_async and then later
	 * complete(), that's the spi_master controller driver.
	 */
	struct list_head	queue;
	void			*state;
};


/* struct spi_board_info: 一个SPI设备特定板上模板 */
/* 给主用的还是给从用的呢？应该是与spi_device类似的，用来在初始化板子的时候使用 */
/* modalias:用来初始化spi_device.modalias,用来识别driver? */
/* platform_data:用来初始化spi_device.platform_data */
/* controller_data:spi_device.platform_data */
/* irq: spi_device.irq */
/* max_speed_hz:spi_device.max_speed_hz */
/* chip_select:spi_device.chip_select */
/* mode:spi_device.mode */
/* bus_num:用来标识这个spi_device是属于哪个master的，一个bus号对应一个master */
/* probe函数建立的信息如发送字节没有包含在这里 */
/* 这个结构体有两个作用：一个是。。。。有注释,另一个是作为spi_new_device的参数 */
/**
 * struct spi_board_info - board-specific template for a SPI device
 * @modalias: Initializes spi_device.modalias; identifies the driver.
 * @platform_data: Initializes spi_device.platform_data; the particular
 *	data stored there is driver-specific.
 * @controller_data: Initializes spi_device.controller_data; some
 *	controllers need hints about hardware setup, e.g. for DMA.
 * @irq: Initializes spi_device.irq; depends on how the board is wired.
 * @max_speed_hz: Initializes spi_device.max_speed_hz; based on limits
 *	from the chip datasheet and board-specific signal quality issues.
 * @bus_num: Identifies which spi_master parents the spi_device; unused
 *	by spi_new_device(), and otherwise depends on board wiring.
 * @chip_select: Initializes spi_device.chip_select; depends on how
 *	the board is wired.
 * @mode: Initializes spi_device.mode; based on the chip datasheet, board
 *	wiring (some devices support both 3WIRE and standard modes), and
 *	possibly presence of an inverter in the chipselect path.
 *
 * When adding new SPI devices to the device tree, these structures serve
 * as a partial device template.  They hold information which can't always
 * be determined by drivers.  Information that probe() can establish (such
 * as the default transfer wordsize) is not included here.
 *
 * These structures are used in two places.  Their primary role is to
 * be stored in tables of board-specific device descriptors, which are
 * declared early in board initialization and then used (much later) to
 * populate a controller's device tree after the that controller's driver
 * initializes.  A secondary (and atypical) role is as a parameter to
 * spi_new_device() call, which happens after those controller drivers
 * are active in some dynamic board configuration models.
 */
struct spi_board_info {
	/* the device name and module name are coupled, like platform_bus;
	 * "modalias" is normally the driver name.
	 *
	 * platform_data goes to spi_device.dev.platform_data,
	 * controller_data goes to spi_device.controller_data,
	 * irq is copied too
	 */
	char		modalias[SPI_NAME_SIZE];
	const void	*platform_data;
	void		*controller_data;
	int		irq;

	/* slower signaling on noisy or low voltage boards */
	u32		max_speed_hz;


	/* bus_num is board specific and matches the bus_num of some
	 * spi_master that will probably be registered later.
	 *
	 * chip_select reflects how this chip is wired to that master;
	 * it's less than num_chipselect.
	 */
	u16		bus_num;
	u16		chip_select;

	/* mode becomes spi_device.mode, and is essential for chips
	 * where the default of SPI_CS_HIGH = 0 is wrong.
	 */
	u8		mode;

	/* ... may need additional spi_device chip config data here.
	 * avoid stuff protocol drivers can set; but include stuff
	 * needed to behave without being bound to a driver:
	 *  - quirks like clock rate mattering when not selected
	 */
};


/* 有必要写出几个结构体 */

struct platform_device {
	const char	* name;
	int		id;
	struct device	dev;
	u32		num_resources;
	struct resource	* resource;

	const struct platform_device_id	*id_entry;

	/* arch specific additions */
	struct pdev_archdata	archdata;
};

/* ioport.h */
/*
 * Resources are tree-like, allowing
 * nesting etc..
 */
struct resource {
	resource_size_t start;
	resource_size_t end;
	const char *name;
	unsigned long flags;
	struct resource *parent, *sibling, *child;
};

struct resource_list {
	struct resource_list *next;
	struct resource *res;
	struct pci_dev *dev;
};

/*
 * IO resources have these defined flags.
 */
#define IORESOURCE_BITS		0x000000ff	/* Bus-specific bits */

#define IORESOURCE_TYPE_BITS	0x00001f00	/* Resource type */
#define IORESOURCE_IO		0x00000100
#define IORESOURCE_MEM		0x00000200
#define IORESOURCE_IRQ		0x00000400
#define IORESOURCE_DMA		0x00000800
#define IORESOURCE_BUS		0x00001000

#define IORESOURCE_PREFETCH	0x00002000	/* No side effects */
#define IORESOURCE_READONLY	0x00004000
#define IORESOURCE_CACHEABLE	0x00008000
#define IORESOURCE_RANGELENGTH	0x00010000
#define IORESOURCE_SHADOWABLE	0x00020000

#define IORESOURCE_SIZEALIGN	0x00040000	/* size indicates alignment */
#define IORESOURCE_STARTALIGN	0x00080000	/* start field is alignment */

#define IORESOURCE_MEM_64	0x00100000
#define IORESOURCE_WINDOW	0x00200000	/* forwarded by bridge */
#define IORESOURCE_MUXED	0x00400000	/* Resource is software muxed */

#define IORESOURCE_EXCLUSIVE	0x08000000	/* Userland may not map this resource */
#define IORESOURCE_DISABLED	0x10000000
#define IORESOURCE_UNSET	0x20000000
#define IORESOURCE_AUTO		0x40000000
#define IORESOURCE_BUSY		0x80000000	/* Driver has marked this resource busy */
