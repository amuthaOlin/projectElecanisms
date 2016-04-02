#ifndef _SPACECOMMS_H
#define _SPACECOMMS_H

#include <stdint.h>

typedef struct SPACK_DIR {
  unsigned packet:2;
  unsigned actaddr:5;
  unsigned actact:6;
  unsigned long :19;
} SPACK_DIR;

typedef struct CONS1_STATE {
  unsigned red_button:1;
  unsigned long :31;
} CONS1_STATE;

typedef struct CONS2_STATE {
  unsigned slider:2;
  unsigned long :30;
} CONS2_STATE;

typedef struct CONS3_STATE {
  unsigned green_button:1;
  unsigned long :31;
} CONS3_STATE;

#endif