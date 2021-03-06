/* free.c - Display amount of free and used memory in the system.
 *
 * Copyright 2012 Elie De Brauwer <eliedebrauwer@gmail.com>

USE_FREE(NEWTOY(free, "gmkb", TOYFLAG_USR|TOYFLAG_BIN))

config FREE
  bool "free"
  default y
  help
    usage: free [-bkmg]

    Display the total, free and used amount of physical memory and
    swap space.

    -bkmg	Output in bytes (default), KB, MB or GB
*/

#define FOR_free
#include "toys.h"

static unsigned long long convert(unsigned long d, unsigned int iscale,
        unsigned int oscale)
{
  return ((unsigned long long)d*iscale)>>oscale;
}

void free_main(void)
{
  struct sysinfo info;
  unsigned int iscale = 1;
  unsigned int oscale = 0;

  sysinfo(&info);
  if (info.mem_unit) iscale = info.mem_unit;
  if (toys.optflags & FLAG_b) oscale = 0;
  if (toys.optflags & FLAG_k) oscale = 10;
  if (toys.optflags & FLAG_m) oscale = 20;
  if (toys.optflags & FLAG_g) oscale = 30;

  xprintf("\t\ttotal        used        free      shared     buffers\n");
  xprintf("Mem:%17llu%12llu%12llu%12llu%12llu\n",
    convert(info.totalram, iscale, oscale),
    convert(info.totalram-info.freeram, iscale, oscale),
    convert(info.freeram, iscale, oscale),
    convert(info.sharedram, iscale, oscale),
    convert(info.bufferram, iscale, oscale));

  xprintf("-/+ buffers/cache:%15llu%12llu\n",
    convert(info.totalram - info.freeram - info.bufferram, iscale, oscale),
    convert(info.freeram + info.bufferram, iscale, oscale));

  xprintf("Swap:%16llu%12llu%12llu\n",
    convert(info.totalswap, iscale, oscale),
    convert(info.totalswap - info.freeswap, iscale, oscale),
    convert(info.freeswap, iscale, oscale));
}
