#ifndef _SPACECOMMS_H
#define _SPACECOMMS_H

#include <stdint.h>

typedef struct SPACK_DIR {
  unsigned packet:2;
  unsigned actaddr:5;
  unsigned actact:6;
  unsigned long :19;
} SPACK_DIR;

typedef struct SLAVE_STATE {
  unsigned red_button:1;
  unsigned slider:2;
  unsigned green_button:1;
  unsigned long :28;
} SLAVE_STATE;

typedef struct SLAVE0_STATE {
  unsigned red_button:1;
  unsigned long :31;
} SLAVE0_STATE;

typedef struct SLAVE1_STATE {
  unsigned slider:2;
  unsigned long :30;
} SLAVE1_STATE;

typedef struct SLAVE2_STATE {
  unsigned green_button:1;
  unsigned long :31;
} SLAVE2_STATE;

#endif