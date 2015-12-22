#include "spiffs.h"
#include "espressif/esp_common.h"


#define S_DBG printf

#define LOG_PAGE_SIZE       256

static spiffs fs;
static u8_t spiffs_work_buf[LOG_PAGE_SIZE*2];
static u8_t spiffs_fds[32*4];
static u8_t spiffs_cache_buf[(LOG_PAGE_SIZE+32)*4];

static s32_t
my_spiff_read(u32_t addr, u32_t size, u8_t *dst) {
    sdk_spi_flash_read((uint32_t) addr, (uint32_t *)dst, (uint32_t)size);
    return SPIFFS_OK;
}

static s32_t
my_spiff_write(u32_t addr, u32_t size, u8_t *src) {
    uint32_t buf;
    int i;

    for(i = 0; i < size; i=+4){
       memcpy(&buf, &src[i], 4);
       sdk_spi_flash_write((uint32_t)addr, (uint32_t*)buf, (uint32_t)4);
    }
    return SPIFFS_OK;
}

static s32_t
my_spiff_erase(u32_t addr, u32_t size) {
    printf("%s\n", "erase not implemeneted :(");
    return SPIFFS_OK;
}


void
test_spiffs() {
    char buf[12];

    // Surely, I've mounted spiffs before entering here

    spiffs_file fd = SPIFFS_open(&fs, "my_file", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
    if (SPIFFS_write(&fs, fd, (u8_t *)"Hello world", 12) < 0) printf("errno %i\n", SPIFFS_errno(&fs));
    SPIFFS_close(&fs, fd);
    printf("reading....");
    fd = SPIFFS_open(&fs, "my_file", SPIFFS_RDWR, 0);
    if (SPIFFS_read(&fs, fd, (u8_t *)buf, 12) < 0) printf("errno %i\n", SPIFFS_errno(&fs));
    SPIFFS_close(&fs, fd);

    printf("--> %s <--\n", buf);
}



void
fs_mount(void) {

    spiffs_config cfg;
    cfg.phys_size = 64*1024; // use all spi flash
    cfg.phys_addr = 0; // start spiffs at start of spi flash
    cfg.phys_erase_block = 4096; // according to datasheet
    cfg.log_block_size = 4096; // let us not complicate things
    cfg.log_page_size = LOG_PAGE_SIZE; // as we said

    cfg.hal_read_f = my_spiff_read;
    cfg.hal_write_f = my_spiff_write;
    cfg.hal_erase_f = my_spiff_erase;

    int res = SPIFFS_mount(&fs,
      &cfg,
      spiffs_work_buf,
      spiffs_fds,
      sizeof(spiffs_fds),
      spiffs_cache_buf,
      sizeof(spiffs_cache_buf),
      0);
    printf("mount res: %i\n", res);

   test_spiffs();


}
