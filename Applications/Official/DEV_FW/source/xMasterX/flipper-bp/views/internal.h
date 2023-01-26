#pragma once
#include <gui/view.h>
#include <furi_hal_vibro.h>

typedef struct FBP FBP;
typedef struct FlipperVibrator FlipperVibrator;

FlipperVibrator* flipper_vibrator_alloc(FBP* fbp);

void flipper_vibrator_free(FlipperVibrator* flipper_vibrator);

View* flipper_vibrator_get_view(FlipperVibrator* flipper_vibrator);
