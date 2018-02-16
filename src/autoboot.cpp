#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/sys_functions.h>
#include <utils/logger.h>
#include <fs/FSUtils.h>
#include <fs/CFile.hpp>
#include <fs/sd_fat_devoptab.h>
#include "autoboot.h"
#include "common/common.h"
#include "common/retain_vars.h"

uint64_t SM4SH_IDS[] = {
    0x0005000010144f00,
    0x0005000010145000,
    0x0005000010110e00
};

uint64_t MK8_IDS[] = {
    0x000500001010ec00,
    0x000500001010ed00,
    0x000500001010eb00
};

uint64_t SPLATOON_IDS[] = {
    0x0005000010176900,
    0x005000010176a000,
    0x0005000010162b00
};

uint64_t BOTW_IDS[] = {
    0x00050000101c9400,
    0x00050000101c9500,
    0x00050000101c9300
};

uint64_t NSMBU_IDS[] = {
    0x0005000010101d00,
    0x0005000010101e00,
    0x0005000010101c00,
    0x000500001014b700,
    0x000500001014b800
};

uint64_t MARIOMAKER_IDS[] = {
    0x000500001018dc00,
    0x000500001018dd00,
    0x000500001018db00
};

uint64_t checkTitlePresets(const char* buf) {
    int i;
    if (strncasecmp(buf, "sm4sh", 5) == 0) {
        for (i = 0; i < 3; i++) {
            if (SYSCheckTitleExists(SM4SH_IDS[i])) {
                return SM4SH_IDS[i];
            }
        }
    }
    if (strncasecmp(buf, "mk8", 3) == 0) {
        for (i = 0; i < 3; i++) {
            if (SYSCheckTitleExists(MK8_IDS[i])) {
                return MK8_IDS[i];
            }
        }
    }
    if (strncasecmp(buf, "splatoon", 8) == 0) {
        for (i = 0; i < 3; i++) {
            if (SYSCheckTitleExists(SPLATOON_IDS[i])) {
                return SPLATOON_IDS[i];
            }
        }
    }
    if (strncasecmp(buf, "botw", 4) == 0) {
        for (i = 0; i < 3; i++) {
            if (SYSCheckTitleExists(BOTW_IDS[i])) {
                return BOTW_IDS[i];
            }
        }
    }
    if (strncasecmp(buf, "nsmbu", 5) == 0) {
        for (i = 0; i < 5; i++) {
            if (SYSCheckTitleExists(NSMBU_IDS[i])) {
                return NSMBU_IDS[i];
            }
        }
    }
    if (strncasecmp(buf, "mariomaker", 10) == 0) {
        for (i = 0; i < 3; i++) {
            if (SYSCheckTitleExists(MARIOMAKER_IDS[i])) {
                return MARIOMAKER_IDS[i];
            }
        }
    }

    return 1;
}

uint64_t readTitleFile() {
    //not sure why i need to remount but it's the only way i got it working
    mount_sd_fat("sd");

    std::string buf;
    char *endptr;

    std::string filepath = SDCAFIINE_AUTOBOOT_PATH;

    CFile file(filepath, CFile::ReadOnly);
    if (!file.isOpen()) {
        return 1;
    }
    buf.resize(file.size());
    file.read((u8 *) &buf[0], buf.size());
    file.close();

    unmount_sd_fat("sd");

    uint64_t titleID = strtoull(buf.c_str(), &endptr, 16);

    if (endptr != buf.c_str()) {
        return titleID;
    }

    return checkTitlePresets(buf.c_str());
}

uint64_t getAutobootTitleID() {
    uint64_t id = readTitleFile();
    return id;
}
