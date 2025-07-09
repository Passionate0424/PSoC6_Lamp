#ifndef __DRV_SDCARD_H__
#define __DRV_SDCARD_H__

#include <rtthread.h>
#include <dfs_fs.h>
#include <drivers/mmcsd_core.h>
#include <drivers/gpt.h>
#include "cyhal_sdhc.h"

struct rthw_sdio
{
    struct rt_device parent;
    cyhal_sdhc_t sdhc_obj;           /**< Object for use with the SDHC HAL driver. */
    cyhal_sdhc_config_t sdhc_config; /**< Card configuration structure to be passed to the HAL driver. */
    const struct _cy_sdio_pin_config *pins_cfg;
    struct dfs_partition part;
    struct rt_device_blk_geometry geometry;
};

extern struct rthw_sdio *sdio;
int rt_hw_sdio_init(void);

#endif // __DRV_SDCARD_H__
