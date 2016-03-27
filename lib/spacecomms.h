#ifndef _SPACECOMMS_H
#define _SPACECOMMS_H

#include <stdint.h>

typedef struct SPACK_DIR {
  unsigned packet:2;
  unsigned actaddr:5;
  unsigned actact:6;
  unsigned :3;
} SPACK_DIR;

typedef struct SLAVE1_STATE {
  unsigned red_button:1;
  unsigned slider:2;
  unsigned :13;
} SLAVE1_STATE;

#endif