#ifndef SD_MEMORY_MANAGER_H
#define SD_MEMORY_MANAGER_H

#ifndef MAX_FILE_SIZE
#define MAX_FILE_SIZE 4096
#endif

void sd_init(void);
void sd_close(void);
void sd_functionality_test(void);

#endif // !SD_MEMORY_MANAGER_H
