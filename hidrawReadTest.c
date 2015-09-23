/* Title: hidrawReadTest.c
 * Description: Test to read data sent from CY8CKIT-042-BLE Pioneer
 *    Baseboard over UART.
 *    Adapted from: http://tomoyo.osdn.jp/cgi-bin/lxr/source/samples/hidraw/hid-example.c?a=mips
 * Author: Mitchell Mellone
 * Date Created: 9/23/15
 * Modifications:
 */

 /* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

const char * bus_str(int bus);

int main(int argc, char** argv) {
  int fd;
  int res, i = 0;
  char buf[256];
  // struct hidraw_report_descriptor rpt_desc;
  struct hidraw_devinfo info;
  char* device = "/dev/hidraw0"; //TODO: Find device name with libudev instead of hardcoding it

  /* Opens the HID device with non-blocking reads. */
  fd = open(device, O_RDWR|O_NONBLOCK);
  if (fd < 0) {
    perror("Unable to open device");
    return 1;
  }

  // /* Set rpt_desc, info, and buf to zeros */
  // memset(&rpt_desc, 0x0, sizeof(rpt_desc));
  memset(&info, 0x0, sizeof(info));
  memset(buf, 0x0, sizeof(buf));

  /* Get Raw Name */
  res = ioctl(fd, HIDIOCGRAWNAME(256), buf);
  if (res < 0)
    perror("HIDIOCGRAWNAME");
  else
    printf("Raw Name: %s\n", buf);

  /* Get Physical Location */
  res = ioctl(fd, HIDIOCGRAWPHYS(256), buf);
  if (res < 0)
    perror("HIDIOCGRAWPHYS");
  else
    printf("Raw Phys: %s\n", buf);

  /* Get Raw Info */
  res = ioctl(fd, HIDIOCGRAWINFO, &info);
  if (res < 0) {
    perror("HIDIOCGRAWINFO");
  } else {
    printf("Raw Info:\n");
    printf("\tbustype: %d (%s)\n",
           info.bustype, bus_str(info.bustype));
    printf("\tvendor: 0x%04hx\n", info.vendor);
    printf("\tproduct: 0x%04hx\n", info.product);
  }

  /* Get a report from the device */
  res = read(fd, buf, 16);
  if (res < 0) {
    perror("read");
  } else {
    printf("read() read %d bytes:\n\t", res);
    for (i = 0; i < res; i++)
      printf("%hhx ", buf[i]);
    puts("\n");
  }

  close(fd);
  return 0;
}

const char *
bus_str(int bus) {
  switch (bus) {
  case BUS_USB:
    return "USB";
    break;
  case BUS_HIL:
    return "HIL";
    break;
  case BUS_BLUETOOTH:
    return "Bluetooth";
    break;
  case BUS_VIRTUAL:
    return "Virtual";
    break;
  default:
    return "Other";
    break;
  }
}
