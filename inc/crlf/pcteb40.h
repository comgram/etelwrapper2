/*
 *	Header file for pcteb kernel module
 *		(linux kernel 2.4 compatible)
 */

/*	New ioctl commands, with 'E' as magic number */
#define PCTEB_IOCTL_MAGIC 'E'

/* old ioctl commands, keep for compatibility reasons */
#define LMSHAPO_IOCTL_SET_PID		0
#define LMSHAPO_IOCTL_SEND_SIGUSR1	1
#define LMSHAPO_IOCTL_ENABLE_IRQ	2
#define LMSHAPO_IOCTL_DISABLE_IRQ	3
#define SHAPO_IOCTL_INIT_IRQ         _IOW(SHAPO_IOCTL_MAGIC, 0, int[2])
#define SHAPO_IOCTL_DESTROY_IRQ      _IO(SHAPO_IOCTL_MAGIC, 1)
#define SHAPO_IOCTL_WAIT_IRQ         _IOW(SHAPO_IOCTL_MAGIC, 2, int[2])
#define SHAPO_IOCTL_GENERATE_IRQ     _IO(SHAPO_IOCTL_MAGIC, 3)
#define SHAPO_IOCTL_GET_IRQ_COUNT    _IOWR(SHAPO_IOCTL_MAGIC, 4, int)
#define SHAPO_IOCTL_GPIO_READ        _IOWR(SHAPO_IOCTL_MAGIC, 5, int)
#define SHAPO_IOCTL_GPIO_WRITE       _IOW(SHAPO_IOCTL_MAGIC, 6, int[2])
#define SHAPO_IOCTL_XIL_SET_TYPE     _IOW(SHAPO_IOCTL_MAGIC, 7, int) 
#define SHAPO_IOCTL_XIL_CLEAR        _IO(SHAPO_IOCTL_MAGIC, 8)
#define SHAPO_IOCTL_XIL_DOWNLOAD     _IOW(SHAPO_IOCTL_MAGIC, 9, int[2])
#define SHAPO_IOCTL_XIL_GET_DONE     _IOR(SHAPO_IOCTL_MAGIC, 10, int)

/*	Data transfert by pointer :
 *		_IOW = write from user space to kernel
 *		_IOR = read  from user space to kernel
 */

#define PCTEB_IOCTL_GET_VERSION		_IOR (PCTEB_IOCTL_MAGIC, 11, int)
#define PCTEB_IOCTL_GET_DEVICE_ID	_IOWR(PCTEB_IOCTL_MAGIC, 12, int[6])
#define PCTEB_IOCTL_INIT_IRQ		_IOW (PCTEB_IOCTL_MAGIC, 13, int[9])
#define PCTEB_IOCTL_FREE_IRQ		_IOW (PCTEB_IOCTL_MAGIC, 14, int)
#define PCTEB_IOCTL_WAIT_IRQ		_IOW (PCTEB_IOCTL_MAGIC, 15, int)
#define PCTEB_IOCTL_GENERATE_IRQ	_IOW (PCTEB_IOCTL_MAGIC, 16, int)
#define PCTEB_IOCTL_GET_COUNT		_IOWR(PCTEB_IOCTL_MAGIC, 17, int[2])
#define PCTEB_IOCTL_READ_PCI		_IOWR(PCTEB_IOCTL_MAGIC, 18, int[4])
#define PCTEB_IOCTL_WRITE_PCI		_IOW (PCTEB_IOCTL_MAGIC, 19, int[4])

#define PCTEB_IOCTL_MAXNR 19
