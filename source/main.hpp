#pragma once

#include <switch.h>
#include <math.h>

typedef uint8_t NsApplicationIcon[0x20000];

struct Title
{
  uint64_t TitleID;
  std::string TitleName;
  NsApplicationIcon icon;
};
