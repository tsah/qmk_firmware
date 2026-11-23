// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Tap-hold and mouse key settings are shared with the Corne: see users/tsah/config.h

// RGB default mode (keyboard.json sets solid blue; override to match the Corne)
#undef RGB_MATRIX_DEFAULT_MODE
#undef RGB_MATRIX_DEFAULT_VAL
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_TYPING_HEATMAP
#define RGB_MATRIX_DEFAULT_VAL 25
