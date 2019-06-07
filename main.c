#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <drm.h>
#include <xf86drm.h>
#include <xf86drmMode.h>


const char* DEVICE_NAME = "/dev/dri/card0";

int main()
{

    int fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0)
    {
        printf("open failed.\n");
        abort();
    }


    int io;

#if 0
    io = drmSetMaster(fd);
    if (io < 0)
    {
        printf("drmSetMaster failed.\n");
        abort();
    }
#endif

    io = drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
    if (io < 0)
    {
        printf("DRM_CLIENT_CAP_UNIVERSAL_PLANES failed.\n");
        abort();
    }

    // io = drmSetClientCap(fd, DRM_CLIENT_CAP_ATOMIC, 1);
    // if (io < 0)
    // {
    //     printf("DRM_CLIENT_CAP_ATOMIC failed.\n");
    //     abort();
    // }


    // Connector
    drmModeRes* res = drmModeGetResources(fd);
    if (!res)
    {
        printf("drmModeGetResources failed.\n");
        abort();
    }

    int connector_count = res->count_connectors;
    printf("connector_count=%d\n", connector_count);


    for (uint32_t connector_id = 0; connector_id < connector_count; ++connector_id)
    {
        // Modes
        drmModeConnector* connector = drmModeGetConnectorCurrent(fd, res->connectors[connector_id]);
        if (!connector)
        {
            printf("drmModeGetConnector failed.\n");
            abort();
        }

        int mode_count = connector->count_modes;
        printf("connector(%d) mode_count=%d\n", connector_id, mode_count);

        for (int mode_id = 0; mode_id < mode_count; ++mode_id)
        {
            drmModeModeInfo mode_info = connector->modes[mode_id];
#if 0
            typedef struct _drmModeModeInfo {
            uint32_t clock;
            uint16_t hdisplay, hsync_start, hsync_end, htotal, hskew;
            uint16_t vdisplay, vsync_start, vsync_end, vtotal, vscan;
            uint32_t vrefresh;
            uint32_t flags;
            uint32_t type;
            char name[DRM_DISPLAY_MODE_LEN];
        } drmModeModeInfo, *drmModeModeInfoPtr;
#endif

#if 0
            printf("Mode (%dx%d@%d)\n", mode_info.hdisplay, mode_info.vdisplay, mode_info.vrefresh);
            printf("clock=%d, hdisplay=%d, hsync_start=%d, hsync_end=%d, htotal=%d, hskew=%d\n", mode_info.clock, mode_info.hdisplay, mode_info.hsync_start, mode_info.hsync_end, mode_info.htotal, mode_info.hskew);
            printf("vdisplay=%d, vsync_start=%d, vsync_end=%d, vtotal=%d, vscan=%d, vrefresh=%d\n", mode_info.vdisplay, mode_info.vsync_start, mode_info.vsync_end, mode_info.vtotal, mode_info.vscan, mode_info.vrefresh);
            
            printf("flags=%010x ", mode_info.flags);
            if (mode_info.flags & DRM_MODE_FLAG_PHSYNC) printf("DRM_MODE_FLAG_PHSYNC ");
            if (mode_info.flags & DRM_MODE_FLAG_NHSYNC) printf("DRM_MODE_FLAG_NHSYNC ");
            if (mode_info.flags & DRM_MODE_FLAG_PVSYNC) printf("DRM_MODE_FLAG_PVSYNC ");
            if (mode_info.flags & DRM_MODE_FLAG_NVSYNC) printf("DRM_MODE_FLAG_NVSYNC ");
            if (mode_info.flags & DRM_MODE_FLAG_INTERLACE) printf("DRM_MODE_FLAG_INTERLACE ");
            if (mode_info.flags & DRM_MODE_FLAG_DBLSCAN) printf("DRM_MODE_FLAG_DBLSCAN ");
            printf("\n");
#else
            printf("\"%dx%d_%d\", ", mode_info.hdisplay, mode_info.vdisplay, mode_info.vrefresh);
            printf("%d, ", mode_info.clock);
            printf("%d, %d, %d, %d, ", mode_info.hdisplay, mode_info.hsync_start, mode_info.hsync_end, mode_info.htotal);
            printf("%d, %d, %d, %d, ", mode_info.vdisplay, mode_info.vsync_start, mode_info.vsync_end, mode_info.vtotal);

            if (mode_info.flags & DRM_MODE_FLAG_PHSYNC)
                printf("+hsync");
            else if (mode_info.flags & DRM_MODE_FLAG_NHSYNC)
                printf("-hsync");

            printf(" ");

            if (mode_info.flags & DRM_MODE_FLAG_PVSYNC)
                printf("+vsync");
            else if (mode_info.flags & DRM_MODE_FLAG_NVSYNC)
                printf("-vsync");

            printf("\n");
#endif

            printf("\n");
        }


        drmModeFreeConnector(connector);
    }


    drmModeFreeResources(res);


    // drmModeConnector* connectorPtr = libdrm.drmModeGetConnector(connector.Device.Handle, connector.ID);
    // if (connector == null)
    //     throw new InvalidOperationException("drmModeGetConnector failed.");

    // result = connectorPtr->count_modes;
    // libdrm.drmModeFreeConnector(connectorPtr);

    return 0;

}
