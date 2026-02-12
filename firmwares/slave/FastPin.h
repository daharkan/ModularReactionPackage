#ifndef FASTPIN_H
#define FASTPIN_H

#include <stdint.h>

// Declared in a header so Arduino's auto-generated function prototypes
// can safely reference FastPin before the .ino body is parsed.
struct FastPin {
  volatile uint8_t* out;
  uint8_t mask;
};

#endif
