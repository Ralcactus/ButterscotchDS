#pragma once

#include <stdbool.h>
#include <stdint.h>

bool nds_load_bin_5551(const char* path, int frame, uint16_t** out_pixels, int* out_w, int* out_h);