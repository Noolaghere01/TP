#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#define DEBUG(x...)
#define ERROR(x...) fprintf(stderr, x)
#define DIE(c, x...) fprintf(stderr, x); exit(c)
#define PATH_SYSCLASS_GPIO "/sys/class/gpio"
#define PATH_SYSCLASS_EXPORT "/sys/class/gpio/export"
#define PATH_SYSCLASS_LED26 "/sys/class/gpio/gpio26"
#define PATH_SYSCLASS_LED26_DIR "/sys/class/gpio/gpio26/direction"
#define PATH_SYSCLASS_LED26_VAL "/sys/class/gpio/gpio26/value"
int sysfs_write(const char *path, const char *val)
{
FILE *f;
int ret;
if (access(path, W_OK) != F_OK) {
return -EPERM;
}
f = fopen(path, "w");
if (f == NULL) {
return -EBADF;
}
ret = fprintf(f, "%s", val);
if (ret < 0) {
ERROR("export write to %s failed\n", path);
} else {
DEBUG("wrote %s to %s\n", val, path);
ret = 0;
}
fclose(f);
return ret;
}
int main(int argc, char **argv)
{
int ret;
int period_ms = 500;
int val = 0;
if (access(PATH_SYSCLASS_GPIO, R_OK) != F_OK) {DIE(1, "cannot open " PATH_SYSCLASS_GPIO "\n");
}
if (access(PATH_SYSCLASS_LED26, R_OK) != F_OK) {
FILE *f = fopen(PATH_SYSCLASS_EXPORT, "w");
if (f == NULL) {
DIE(1, "cannot open export for writing...\n");
}
ret = fprintf(f, "%d", 26);
if (ret < 0) {
ERROR("export write failure...\n");
}
fclose(f);
}
if (access(PATH_SYSCLASS_LED26, R_OK) != F_OK) {
DIE(1, "cannot open " PATH_SYSCLASS_LED26 "\n");
}
/* set direction */
ret = sysfs_write(PATH_SYSCLASS_LED26_DIR, "out");
if (ret) {
DIE(1, "cannot set direction as output\n");
}
do {
char * buf;
int c;
c = asprintf(&buf, "%d", val ? 1 : 0);
val = val ? 0 : 1;
ret = sysfs_write(PATH_SYSCLASS_LED26_VAL, buf);
if (ret) {
DIE(1, "cannot set direction as output\n");
}
usleep(period_ms * 1000);
} while (true);
return 0;
}