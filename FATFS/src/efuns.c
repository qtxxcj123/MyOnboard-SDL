#include "efuns.h"
#include "stdlib.h"
#include "sdio_sdcard.h"
#include "stdio.h"
FIL fil;
UINT bww;
FATFS *fs;

FRESULT initFatfs(const char *name)
{
    FRESULT res;
    fs = malloc(sizeof(FATFS));
    res = f_mount(fs, "0:", 1);
    if(res)return res;
    res = f_open (&fil, name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
    if(res != 0)
        printf("file open not succeed, error code=%d\r\n", res);
    else
        printf("file open succed.\r\n");
    //res = f_open (&fil, "0:/NewData.txt", FA_OPEN_ALWAYS | FA_WRITE);
    return res;
}

FRESULT readFile(void *buffer, uint32_t len)
{
    //
}

FRESULT writeFile(void *buffer, uint32_t len)
{
    FRESULT res;
    //res = f_lseek(&fil,fil.fptr + fil.fsize);
    //if(res)return res;
    res = f_write (&fil, buffer, len, &bww);
    return res;
}

FRESULT closeFile()
{
    FRESULT res;
    res = f_close(&fil);
    return res;
}
