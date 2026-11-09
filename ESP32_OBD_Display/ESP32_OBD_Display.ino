/*
 * ESP32-OBD-Display von ihmSelbst · Firmware V21-RC11b
 * Waveshare ESP32-S3-Touch-LCD-5B (1024 × 600) mit LVGL 8.4 und BLE-OBD-II.
 * Zeigt Livewerte, liest Fehlercodes und unterstützt lokale WLAN-Updates.
 * Copyright (c) 2026 ihmSelbst · SPDX-License-Identifier: MIT
 * Aufbau und Einrichtung stehen in der README.
 */

 

#include <Arduino.h>
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include "lvgl_v8_port.h"
#include "dashboard_assets.h"

extern "C" {
extern const lv_img_dsc_t logo_img;
}

static const uint8_t opel_logo_large_map[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 36, 36, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 26, 36, 36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  12, 12, 148, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 182, 182, 38, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 100, 100, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 100, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 58, 58, 255, 255, 255, 255, 255, 255, 255, 255, 255, 204,
  148, 148, 104, 96, 96, 126, 186, 186, 238, 255, 255, 255, 255, 255, 255, 255, 255, 84, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 255, 255, 255, 255, 255, 255,
  122, 122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 255, 255, 255, 255, 255, 255,
  226, 226, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204,
  255, 255, 255, 255, 255, 255, 122, 122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82,
  255, 255, 255, 255, 255, 255, 226, 226, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 255, 255, 255, 255, 255, 255, 124, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 52, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 202, 255, 255, 255, 255, 255, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 202,
  255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 222,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 156, 156, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180,
  255, 255, 255, 228, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 176, 255, 255, 255, 190, 190, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 180, 255, 255, 255, 228, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 176, 255, 255, 255,
  190, 190, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 234, 234, 255, 255, 255, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255,
  255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 58, 255, 255, 255, 186, 186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 106,
  255, 255, 255, 58, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 248, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 255, 255, 255, 234, 234, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 248, 255, 255, 255, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 234, 234, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 218,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 138, 138, 255, 255, 255, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  122, 122, 255, 255, 255, 74, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 255, 255, 255, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 122, 122, 255, 255, 255, 74, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 154, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 146, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 210, 255, 255, 228,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 146, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 210, 255, 255, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 194, 255,
  250, 250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 206, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 162, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 194, 194, 0, 0, 0, 0, 12, 12, 34,
  64, 64, 98, 142, 142, 176, 204, 204, 184, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62, 62, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 154, 255, 255, 255, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 194, 194, 0,
  0, 0, 0, 12, 12, 34, 64, 64, 98, 142, 142, 176, 204, 204, 184, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 62, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 154, 255, 255, 255,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  255, 255, 255, 168, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 118, 118, 126, 92, 92, 66, 30, 30, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 144, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 255, 255, 255, 178, 178, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  94, 94, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 238, 238, 204, 188, 188, 168, 162, 162, 148, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 178, 178, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 94, 94, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 238, 238, 204, 188, 188, 168,
  162, 162, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 208, 208, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  222, 222, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 222, 222, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 32, 32, 0, 6, 6, 18, 22, 22, 22, 22, 22, 20, 18, 18, 20, 22, 22, 18, 18, 18, 18,
  16, 16, 14, 14, 14, 10, 2, 2, 2, 10, 10, 6, 80, 80, 255, 255, 255, 182, 0, 0, 18, 6, 6, 10,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 98,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 116, 116, 255, 255, 255, 10,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 255, 255, 255, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  116, 116, 255, 255, 255, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 255, 255, 255, 60, 60, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 255, 255, 255, 116, 116, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 255, 255, 255,
  60, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 255, 255, 255, 116, 116, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255,
  255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 134, 134, 255, 255, 255, 178, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 146, 146, 255, 255, 255, 96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 134, 134, 255, 255, 255, 178, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 146, 146, 255, 255, 255, 96, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255,
  255, 255, 255, 112, 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 255, 255, 255, 250, 250, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 10, 255, 255, 255, 255, 255, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 80, 80, 255, 255, 255, 255,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 255, 255, 255, 255, 255, 98, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  80, 80, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 38, 255, 255, 255, 255,
  138, 138, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 138, 255, 255, 255, 255, 255, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 42, 255, 255, 255, 255, 255, 255, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 14, 246, 246, 255, 255, 255, 255, 26, 26, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 42, 255, 255, 255, 255, 255, 255, 10, 10, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 246, 246, 255, 255, 255, 255, 26, 26, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 14, 255, 255, 255, 255, 255, 255, 212,
  50, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 196, 196, 255, 255, 255, 255,
  255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  182, 182, 255, 255, 255, 255, 255, 255, 255, 204, 204, 148, 102, 102, 62, 44, 44, 88, 156, 156, 198, 255, 255, 255,
  255, 255, 255, 255, 255, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 182, 182, 255, 255, 255, 255, 255, 255, 255, 204, 204, 148, 102, 102, 62, 44, 44, 88,
  156, 156, 198, 255, 255, 255, 255, 255, 255, 255, 255, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 208, 208, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 218, 18, 18, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16,
  150, 150, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 140, 8, 8, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 16, 150, 150, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 140,
  8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 54, 80, 80, 106, 104, 104, 78,
  44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 4,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const lv_img_dsc_t opel_logo_large = {
    .header = { .cf = LV_IMG_CF_ALPHA_8BIT, .always_zero = 0, .reserved = 0, .w = 78, .h = 71 },
    .data_size = 5538,
    .data = opel_logo_large_map
};

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "ble_client_access.h"
#include <map>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include "esp_task_wdt.h"
#include "esp_ota_ops.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

using namespace esp_panel::board;
using namespace esp_panel::drivers;

enum PIDType {
    PID_RPM, PID_SPEED, PID_LOAD, PID_COOLANT, PID_OIL, PID_FUEL, PID_MAP,
    PID_BARO, PID_MAF, PID_INTAKE, PID_VOLTAGE, PID_THROTTLE, PID_LAMBDA, PID_FUEL_RATE
};

enum MetricId {
    METRIC_SPEED, METRIC_RPM, METRIC_BOOST, METRIC_LOAD, METRIC_COOLANT,
    METRIC_OIL, METRIC_INTAKE, METRIC_FUEL_LEVEL, METRIC_VOLTAGE, METRIC_THROTTLE,
    METRIC_LAMBDA, METRIC_MAF, METRIC_POWER, METRIC_CONSUMPTION, METRIC_COUNT
};

enum ValueFreshness : uint8_t { VALUE_FRESH = 0, VALUE_STALE = 1, VALUE_EXPIRED = 2 };

enum VehicleProfile : uint8_t {
    VEH_AUTO = 0,
    VEH_OPEL,
    VEH_ASTRA,
    VEH_ASTRA_OPC,
    VEH_CORSA,
    VEH_INSIGNIA,
    VEH_MOKKA,
    VEH_ZAFIRA,
    VEH_MERIVA,
    VEH_ADAM,
    VEH_GRANDLAND,
    VEH_CROSSLAND,
    VEH_COMBO,
    VEH_VIVARO,
    VEH_KARL,
    VEH_CASCADA,
    VEH_ANTARA,
    VEH_SIGNUM,
    VEH_VECTRA,
    VEH_TIGRA,
    VEH_FRONTERA,
    VEH_AMPERA,

    VEH_AGILA,
    VEH_MOVANO,
    VEH_OMEGA,
    VEH_GT,
    VEH_SPEEDSTER,
    VEH_MONTEREY,
    VEH_PROFILE_COUNT
};

#if defined(LV_FONT_MONTSERRAT_28) && LV_FONT_MONTSERRAT_28
  #define UI_FONT_VEHICLE_MODEL (&lv_font_montserrat_28)
#else
  #define UI_FONT_VEHICLE_MODEL (&lv_font_montserrat_20)
#endif

#if defined(LV_FONT_MONTSERRAT_24) && LV_FONT_MONTSERRAT_24
  #define UI_FONT_OTA_TITLE (&lv_font_montserrat_24)
#else
  #define UI_FONT_OTA_TITLE (&lv_font_montserrat_20)
#endif

#if defined(LV_FONT_MONTSERRAT_16) && LV_FONT_MONTSERRAT_16
  #define UI_FONT_BODY_16 (&lv_font_montserrat_16)
#else
  #define UI_FONT_BODY_16 (&lv_font_montserrat_14)
#endif

#define CAR_NAME "ASTRA OPC"
#define RPM_MAX 7500

#define LIGHT_SENSE_EXIO 5

#define BLE_SCAN_SECONDS 4
#define BLE_RECONNECT_DELAY_MS 5000
#define BLE_ADAPTER_WARMUP_MS 7000
#define BLE_MIN_FREE_HEAP 6000

#define OTA_AP_PREFIX "SCHIEREN-OBD"
#define OTA_HTTP_PORT 80
#define OTA_SESSION_TIMEOUT_MS (10UL * 60UL * 1000UL)
#define FW_VERSION "V21-RC11b"
#define CONFIG_SCHEMA_VERSION 22

#define PID_TIMEOUT_MS 120
#define PID_GAP_MS 25
#define OBD_CYCLE_GAP_MS 25

#define SD_CS_EXIO 4
#define SD_MOSI_PIN 11
#define SD_CLK_PIN 12
#define SD_MISO_PIN 13
#define SD_SS_PIN -1
#define SD_SPI_HZ 4000000UL
#define SD_INIT_MIN_FREE_HEAP 18000UL
#define SD_LOG_PERIOD_MS 1000UL
#define SD_LOG_FLUSH_MS 10000UL

#define PROFESSIONAL_WDT_TIMEOUT_MS 12000UL
#define HEALTHY_BOOT_MARK_MS 20000UL
#define UI_UPDATE_INTERVAL_MS 50UL
#define SETTINGS_PERSIST_DEBOUNCE_MS 800UL

#define DEFAULT_BARO_KPA 101.3f
#define STOICH_AFR 14.7f
#define FUEL_DENSITY_KG_L 0.745f

#define COL_BG lv_color_hex(0x080A0F)
#define COL_PANEL lv_color_hex(0x11151D)
#define COL_PANEL2 lv_color_hex(0x191E28)

#define COL_WHITE lv_color_hex(0xF4F6F8)
#define COL_GREY lv_color_hex(0x7D8795)

#define COL_GREEN lv_color_hex(0xA8FF00)
#define COL_BLUE lv_color_hex(0x00BFFF)
#define COL_ORANGE lv_color_hex(0xFF8A00)
#define COL_RED lv_color_hex(0xFF304F)

#define COL_BRAND_PURPLE lv_color_hex(0x8A2BE2)
#define COL_BRAND_BLUE lv_color_hex(0x00BFFF)
#define COL_BRAND_DARK lv_color_hex(0x05060A)
#define COL_BRAND_DARK2 lv_color_hex(0x14121F)

static BLEUUID OBD_SERVICE_UUID("000018f0-0000-1000-8000-00805f9b34fb");
static BLEUUID OBD_READ_UUID("00002af0-0000-1000-8000-00805f9b34fb");
static BLEUUID OBD_WRITE_UUID("00002af1-0000-1000-8000-00805f9b34fb");

BLEClient* obdClient = nullptr;
BLERemoteCharacteristic* obdRead = nullptr;
BLERemoteCharacteristic* obdWrite = nullptr;

volatile bool obdConnected = false;
volatile bool bleDisconnectedEvent = false;

unsigned long lastReconnectAttempt = 0;
unsigned long bleColdStartNotBefore = 0;

#define MAX_SCAN_DEVICES 8

struct ScannedDevice {
    char name[32];
    char address[24];
    bool hasOBDService;
};

ScannedDevice scannedDevices[MAX_SCAN_DEVICES];
BLEAdvertisedDevice* scannedDeviceRefs[MAX_SCAN_DEVICES] = { nullptr };
uint8_t scannedDeviceCount = 0;

bool bleShowingSelector = false;
bool bleManualConnectRequested = false;
uint8_t bleManualConnectIndex = 0;
volatile bool bleRescanRequested = false;  

char bleDiagnosticSummary[64] = "";
char bleDiagnosticDetail[1024] = "";
bool bleDiagnosticHasDetail = false;

SemaphoreHandle_t scanDataMutex = nullptr;

void appLog(const char* fmt, ...) {
    char buf[160];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Serial.println(buf);
}

#define RX_BUFFER_SIZE 256
#define RX_LINE_QUEUE_DEPTH 3
#define RX_LINE_QUEUE_SIZE 128

volatile char rxBuildLine[RX_LINE_QUEUE_SIZE];
volatile uint16_t rxBuildIndex = 0;
volatile char rxLineQueue[RX_LINE_QUEUE_DEPTH][RX_LINE_QUEUE_SIZE];
volatile uint8_t rxQueueHead = 0;
volatile uint8_t rxQueueTail = 0;
volatile uint8_t rxQueueCount = 0;
volatile bool rxLineReady = false;
volatile uint32_t rxDroppedLines = 0;

portMUX_TYPE rxMux = portMUX_INITIALIZER_UNLOCKED;

struct OBDData {
    float rpm = NAN;
    float speed = NAN;
    float load = NAN;
    float coolant = NAN;
    float oil = NAN;
    float intake = NAN;
    float fuel = NAN;
    float map = NAN;
    float baro = NAN;
    float maf = NAN;
    float voltage = NAN;
    float lambda = NAN;
    float throttle = NAN;
    float fuelRate = NAN;

    bool rpmOK = false;
    bool speedOK = false;
    bool loadOK = false;
    bool coolantOK = false;
    bool oilOK = false;
    bool fuelOK = false;
    bool mapOK = false;
    bool baroOK = false;
    bool mafOK = false;
    bool voltageOK = false;
    bool lambdaOK = false;
    bool intakeOK = false;
    bool throttleOK = false;
    bool fuelRateOK = false;
} data;

struct PIDRequest {
    const char* command;
    PIDType type;
    uint32_t intervalMs;  
    uint32_t lastPollAt;   
};

PIDRequest pidTable[] = {
    {"010C",   PID_RPM,       100,  0},  
    {"010D",   PID_SPEED,     150,  0},  
    {"0104",   PID_LOAD,      250,  0},  
    {"0105",   PID_COOLANT,   1000, 0},  

    {"015C",   PID_OIL,       1000, 0},  
    {"012F",   PID_FUEL,      5000, 0},  
    {"010B",   PID_MAP,       100,  0},  
    {"0133",   PID_BARO,      5000, 0},  
    {"0110",   PID_MAF,       250,  0},  
    {"010F",   PID_INTAKE,    1000, 0},  
    {"0142",   PID_VOLTAGE,   1000, 0},  
    {"0111",   PID_THROTTLE,  150,  0},  
    {"0144",   PID_LAMBDA,    250,  0},  
    {"015E",   PID_FUEL_RATE, 500,  0}   
};

const uint8_t PID_COUNT = sizeof(pidTable) / sizeof(pidTable[0]);

uint16_t lastPIDLatencyMs = 0;
unsigned long pidSentAt = 0;

uint32_t pidLastSuccessAt[PID_COUNT] = {0};

#define PID_HEALTH_DISCOVERING 0
#define PID_HEALTH_ACTIVE      1
#define PID_HEALTH_BACKOFF     2
#define PID_HEALTH_UNAVAILABLE 3
#define PID_UNAVAILABLE_RETRY_MS 120000UL
uint8_t pidRuntimeHealth[PID_COUNT] = {0};
uint8_t pidNoDataStreak[PID_COUNT] = {0};
uint8_t pidTimeoutStreak[PID_COUNT] = {0};
uint32_t pidRetryAfter[PID_COUNT] = {0};

uint16_t pidSuccessWindow = 0;
uint16_t pidRequestWindow = 0;
uint16_t pidTimeoutWindow = 0;
uint16_t pidInvalidWindow = 0;
float pidLatencyEwmaMs = 0.0f;
uint8_t schedulerPressure = 0;  
uint32_t schedulerLastTuneAt = 0;
uint32_t minFreeHeapSeen = UINT32_MAX;
uint32_t obdPollingStartedAt = 0;
bool obdPartialRecoveryAttempted = false;

static int8_t pidIndexForType(PIDType type) {
    for (uint8_t i = 0; i < PID_COUNT; i++) if (pidTable[i].type == type) return (int8_t)i;
    return -1;
}

static void resetPidRuntimeHealth() {
    for (uint8_t i = 0; i < PID_COUNT; i++) {
        pidRuntimeHealth[i] = PID_HEALTH_DISCOVERING;
        pidNoDataStreak[i] = 0;
        pidTimeoutStreak[i] = 0;
        pidRetryAfter[i] = 0;
    }
}

static bool pidTypeUnavailable(PIDType type) {
    const int8_t i = pidIndexForType(type);
    return i >= 0 && pidRuntimeHealth[(uint8_t)i] == PID_HEALTH_UNAVAILABLE;
}

static bool pidEligibleForPoll(uint8_t index, uint32_t now) {
    if (index >= PID_COUNT) return false;
    const uint8_t state = pidRuntimeHealth[index];
    if (state != PID_HEALTH_BACKOFF && state != PID_HEALTH_UNAVAILABLE) return true;
    return (int32_t)(now - pidRetryAfter[index]) >= 0;
}

static void recordPidNoData(uint8_t index, uint32_t now) {
    if (index >= PID_COUNT) return;
    if (pidNoDataStreak[index] < 255) pidNoDataStreak[index]++;
    pidTimeoutStreak[index] = 0;

    if (pidNoDataStreak[index] >= 3) {
        if (pidRuntimeHealth[index] != PID_HEALTH_UNAVAILABLE) {
            appLog("[PID] %s unavailable after %u direct NO DATA replies",
                   pidTable[index].command, (unsigned)pidNoDataStreak[index]);
        }
        pidRuntimeHealth[index] = PID_HEALTH_UNAVAILABLE;
        pidRetryAfter[index] = now + PID_UNAVAILABLE_RETRY_MS;
    } else {

        pidRuntimeHealth[index] = PID_HEALTH_BACKOFF;
        pidRetryAfter[index] = now + 1200UL;
    }
}

static void recordPidTimeoutBackoff(uint8_t index, uint32_t now) {
    if (index >= PID_COUNT) return;

    if (pidRuntimeHealth[index] == PID_HEALTH_UNAVAILABLE) {
        pidRetryAfter[index] = now + PID_UNAVAILABLE_RETRY_MS;
        return;
    }

    if (pidTimeoutStreak[index] < 255) pidTimeoutStreak[index]++;
    if (pidTimeoutStreak[index] < 2) {
        pidRuntimeHealth[index] = PID_HEALTH_BACKOFF;
        pidRetryAfter[index] = now + 800UL;
        return;
    }

    pidRuntimeHealth[index] = PID_HEALTH_BACKOFF;
    const uint32_t pauseMs =
        pidTimeoutStreak[index] >= 6 ? 60000UL :
        pidTimeoutStreak[index] >= 4 ? 15000UL : 5000UL;
    pidRetryAfter[index] = now + pauseMs;
}

static void markPidFresh(PIDType type) {
    const int8_t i = pidIndexForType(type);
    if (i < 0) return;
    const uint8_t index = (uint8_t)i;
    pidLastSuccessAt[index] = millis();
    pidRuntimeHealth[index] = PID_HEALTH_ACTIVE;
    pidNoDataStreak[index] = 0;
    pidTimeoutStreak[index] = 0;
    pidRetryAfter[index] = 0;
}

static uint32_t effectivePidIntervalMs(uint8_t index);

static uint32_t freshnessStaleAfterMs(PIDType type) {
    const int8_t i = pidIndexForType(type);
    const uint32_t interval = i >= 0 ? effectivePidIntervalMs((uint8_t)i) : 1000UL;

    return max(3000UL, interval * 3UL + 1000UL);
}

static uint32_t freshnessExpiredAfterMs(PIDType type) {
    const int8_t i = pidIndexForType(type);
    const uint32_t interval = i >= 0 ? effectivePidIntervalMs((uint8_t)i) : 1000UL;

    return max(15000UL, interval * 8UL + 2000UL);
}

static ValueFreshness pidFreshness(PIDType type) {
    const int8_t i = pidIndexForType(type);
    if (i < 0) return VALUE_EXPIRED;
    const uint32_t last = pidLastSuccessAt[(uint8_t)i];
    if (last == 0) return VALUE_EXPIRED;
    const uint32_t age = millis() - last;
    if (age >= freshnessExpiredAfterMs(type)) return VALUE_EXPIRED;
    if (age >= freshnessStaleAfterMs(type)) return VALUE_STALE;
    return VALUE_FRESH;
}

static uint8_t pidPriority(PIDType type) {
    switch (type) {
        case PID_RPM:
        case PID_MAP:
        case PID_LAMBDA: return 0;  
        case PID_SPEED:
        case PID_THROTTLE:
        case PID_MAF:
        case PID_LOAD: return 1;
        default: return 2;          
    }
}

static uint32_t effectivePidIntervalMs(uint8_t index) {
    if (index >= PID_COUNT) return 1000;
    const uint32_t base = pidTable[index].intervalMs;
    const uint8_t p = pidPriority(pidTable[index].type);
    static const uint16_t scalePct[4][3] = {
        {100, 100, 100},
        {100, 115, 140},
        {110, 140, 200},
        {125, 175, 300}
    };
    return max(40UL, (base * (uint32_t)scalePct[min((uint8_t)3, schedulerPressure)][p]) / 100UL);
}

static uint32_t adaptivePidGapMs() {
    return PID_GAP_MS + (uint32_t)schedulerPressure * 12UL;
}

static void schedulerRecordSuccess(uint16_t latencyMs) {
    pidSuccessWindow++;
    lastPIDLatencyMs = latencyMs;
    if (pidLatencyEwmaMs <= 0.1f) pidLatencyEwmaMs = latencyMs;
    else pidLatencyEwmaMs = pidLatencyEwmaMs * 0.82f + latencyMs * 0.18f;
}

static void schedulerRecordTimeout() { pidTimeoutWindow++; }
static void schedulerRecordInvalid() { pidInvalidWindow++; }

static void invalidateAllPidFreshness() {
    for (uint8_t i = 0; i < PID_COUNT; i++) pidLastSuccessAt[i] = 0;
}

static void resetSchedulerHealth() {
    pidSuccessWindow = pidRequestWindow = pidTimeoutWindow = pidInvalidWindow = 0;
    pidLatencyEwmaMs = 0.0f;
    schedulerPressure = 0;
    schedulerLastTuneAt = millis();
}

static void tuneAdaptiveScheduler(uint32_t now) {
    if (now - schedulerLastTuneAt < 2000UL) return;
    schedulerLastTuneAt = now;
    if (pidRequestWindow < 4) return;

    const uint16_t failures = pidTimeoutWindow + pidInvalidWindow;
    const uint16_t failurePct = (uint16_t)((failures * 100UL) / max((uint16_t)1, pidRequestWindow));

    if ((failurePct >= 20 || pidLatencyEwmaMs > 115.0f) && schedulerPressure < 3) schedulerPressure++;
    else if (failurePct <= 5 && pidLatencyEwmaMs > 0.0f && pidLatencyEwmaMs < 75.0f && schedulerPressure > 0) schedulerPressure--;

    pidRequestWindow = pidSuccessWindow = pidTimeoutWindow = pidInvalidWindow = 0;
}

enum OBDState { OBD_IDLE, OBD_SEND, OBD_WAIT, OBD_NEXT };

OBDState obdState = OBD_IDLE;
uint8_t currentPID = 0;
unsigned long obdStateTime = 0;
bool obdCycleActive = false;
char currentResponse[RX_BUFFER_SIZE];

enum ELMState { ELM_IDLE, ELM_ATZ, ELM_ATE0, ELM_ATL0, ELM_ATH0, ELM_ATSP0, ELM_READY, ELM_FAILED };

ELMState elmState = ELM_IDLE;
unsigned long elmTimer = 0;
unsigned long elmNotBefore = 0;
uint8_t elmInitAttempt = 0;
uint8_t elmStepRetry = 0;
bool elmInitialized = false;

struct MetricDef {
    const char* name;
    const char* unit;
    lv_color_t color;
    float warnMin;  
    float warnMax;  
};

#define NO_WARN NAN

MetricDef metricDefs[METRIC_COUNT] = {

    { "SPEED",         "km/h",     COL_WHITE,   NO_WARN,  NO_WARN },
    { "RPM",            "rpm",     COL_GREEN,   NO_WARN,  6500    },
    { "BOOST",          "bar",     COL_BLUE,    NO_WARN,  1.5f    },
    { "ENGINE LOAD",    "%",       COL_GREEN,   NO_WARN,  NO_WARN },
    { "COOLANT",        "C",       COL_WHITE,   NO_WARN,  110     },
    { "OIL TEMP",       "C",       COL_ORANGE,  NO_WARN,  130     },
    { "INTAKE AIR",     "C",       COL_BLUE,    NO_WARN,  NO_WARN },
    { "FUEL LEVEL",     "%",       COL_WHITE,   10,       NO_WARN },
    { "VOLTAGE",        "V",       COL_WHITE,   11.5f,    15      },
    { "THROTTLE",       "%",       COL_GREEN,   NO_WARN,  NO_WARN },
    { "LAMBDA",         "",        COL_GREEN,   NO_WARN,  NO_WARN },
    { "MAF",            "g/s",     COL_BLUE,    NO_WARN,  NO_WARN },
    { "POWER (EST)",    "PS",      COL_ORANGE,  NO_WARN,  NO_WARN },
    { "CONSUMPTION",    "L/100km", COL_GREEN,   NO_WARN,  NO_WARN },
};

bool isMetricOutOfRange(MetricId id, float v) {
    if (!isnan(metricDefs[id].warnMin) && v < metricDefs[id].warnMin) return true;
    if (!isnan(metricDefs[id].warnMax) && v > metricDefs[id].warnMax) return true;
    return false;
}

#define DASHBOARD_SLOT_COUNT 8

MetricId dashboardSlotMetric[DASHBOARD_SLOT_COUNT] = {
    METRIC_BOOST, METRIC_COOLANT, METRIC_OIL, METRIC_INTAKE,
    METRIC_LOAD, METRIC_CONSUMPTION, METRIC_FUEL_LEVEL, METRIC_VOLTAGE
};

uint8_t dashboardSlotRefreshHz[DASHBOARD_SLOT_COUNT] = {
    10, 1, 1, 1, 5, 2, 1, 1
};
unsigned long dashboardSlotLastUiUpdate[DASHBOARD_SLOT_COUNT] = {0};

bool dashboardSlotHidden[DASHBOARD_SLOT_COUNT] = { false, false, false, false, false, false, false, false };

#define PICKER_SLOT_RPM_GAUGE 200
uint8_t rpmGaugeRefreshHz = 10;

bool centerMetricHidden[3] = { false, false, false };

uint32_t refreshHzToIntervalMs(uint8_t hz) {
    if (hz == 0) hz = 1;
    return 1000UL / hz;
}

void applySlotRefreshRatesToPidTable() {

    const uint32_t baseInterval[PID_COUNT] = {
        100, 150, 1000, 1000, 1000, 5000, 1000, 5000, 250, 1000, 1000, 150, 250, 1000
    };

    for (uint8_t p = 0; p < PID_COUNT; p++) pidTable[p].intervalMs = baseInterval[p];

    for (uint8_t p = 0; p < PID_COUNT; p++) {
        if (pidTable[p].type == PID_RPM) pidTable[p].intervalMs = refreshHzToIntervalMs(rpmGaugeRefreshHz);
    }

    auto applyPid = [&](PIDType type, uint32_t interval) {
        for (uint8_t p = 0; p < PID_COUNT; p++) {
            if (pidTable[p].type == type && interval < pidTable[p].intervalMs) {
                pidTable[p].intervalMs = interval;
            }
        }
    };

    for (uint8_t i = 0; i < DASHBOARD_SLOT_COUNT; i++) {
        if (dashboardSlotHidden[i]) continue;
        const uint32_t interval = refreshHzToIntervalMs(dashboardSlotRefreshHz[i]);

        switch (dashboardSlotMetric[i]) {
            case METRIC_SPEED:       applyPid(PID_SPEED, interval); break;
            case METRIC_RPM:         applyPid(PID_RPM, interval); break;
            case METRIC_BOOST:       applyPid(PID_MAP, interval); break;
            case METRIC_LOAD:        applyPid(PID_LOAD, interval); break;
            case METRIC_COOLANT:     applyPid(PID_COOLANT, interval); break;
            case METRIC_OIL:         applyPid(PID_OIL, interval); break;
            case METRIC_INTAKE:      applyPid(PID_INTAKE, interval); break;
            case METRIC_FUEL_LEVEL:  applyPid(PID_FUEL, interval); break;
            case METRIC_VOLTAGE:     applyPid(PID_VOLTAGE, interval); break;
            case METRIC_THROTTLE:    applyPid(PID_THROTTLE, interval); break;
            case METRIC_LAMBDA:      applyPid(PID_LAMBDA, interval); break;
            case METRIC_MAF:         applyPid(PID_MAF, interval); break;
            case METRIC_POWER:
                applyPid(PID_RPM, interval);
                applyPid(PID_LOAD, interval);
                break;
            case METRIC_CONSUMPTION:
                applyPid(PID_SPEED, interval);
                applyPid(PID_FUEL_RATE, interval);
                applyPid(PID_MAF, interval);
                break;
            default: break;
        }
    }
}

lv_obj_t* dashboardPanel[DASHBOARD_SLOT_COUNT];
lv_obj_t* dashboardPanelTitle[DASHBOARD_SLOT_COUNT];
lv_obj_t* dashboardPanelValue[DASHBOARD_SLOT_COUNT];
lv_obj_t* dashboardPanelUnit[DASHBOARD_SLOT_COUNT];
lv_obj_t* dashboardPanelIcon[DASHBOARD_SLOT_COUNT] = { nullptr };
lv_obj_t* dashboardPanelAccent[DASHBOARD_SLOT_COUNT] = { nullptr };

lv_obj_t* dashboardSlotHotspot[DASHBOARD_SLOT_COUNT] = { nullptr };
lv_obj_t* centerMetricCard[3] = { nullptr, nullptr, nullptr };
lv_obj_t* centerMetricHotspot[3] = { nullptr, nullptr, nullptr };

void applyCenterMetricVisibility(uint8_t i) {
    if (i >= 3 || !centerMetricCard[i]) return;
    const bool hidden = centerMetricHidden[i];
    if (hidden) lv_obj_add_flag(centerMetricCard[i], LV_OBJ_FLAG_HIDDEN);
    else lv_obj_clear_flag(centerMetricCard[i], LV_OBJ_FLAG_HIDDEN);
    if (centerMetricHotspot[i]) {
        if (hidden) lv_obj_clear_flag(centerMetricHotspot[i], LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(centerMetricHotspot[i], LV_OBJ_FLAG_HIDDEN);
    }
}

void applyDashboardSlotVisibility(uint8_t slot) {
    if (slot >= DASHBOARD_SLOT_COUNT || !dashboardPanel[slot]) return;
    const bool hidden = dashboardSlotHidden[slot];
    if (hidden) lv_obj_add_flag(dashboardPanel[slot], LV_OBJ_FLAG_HIDDEN);
    else lv_obj_clear_flag(dashboardPanel[slot], LV_OBJ_FLAG_HIDDEN);
    if (dashboardSlotHotspot[slot]) {
        if (hidden) lv_obj_clear_flag(dashboardSlotHotspot[slot], LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(dashboardSlotHotspot[slot], LV_OBJ_FLAG_HIDDEN);
    }
}

lv_obj_t* dashboardScreen = nullptr;
lv_obj_t* splashOverlay = nullptr;
lv_obj_t* splashStatusLabel = nullptr;

lv_obj_t* connectionLabel = nullptr;
lv_obj_t* connectionStatusLabel = nullptr;

lv_obj_t* rpmMeter = nullptr;  
lv_obj_t* rpmZoneArc[4] = { nullptr, nullptr, nullptr, nullptr };
lv_obj_t* rpmTickLine[31] = { nullptr };
lv_obj_t* rpmTickLabel[8] = { nullptr };

lv_obj_t* rpmLabel = nullptr;
lv_obj_t* rpmUnitLabel = nullptr;
lv_obj_t* speedLabel = nullptr;
lv_obj_t* speedUnitLabel = nullptr;
lv_obj_t* centerMetricValue[3] = { nullptr, nullptr, nullptr };
lv_obj_t* centerMetricUnit[3] = { nullptr, nullptr, nullptr };
lv_obj_t* centerMetricTitle[3] = { nullptr, nullptr, nullptr };

lv_obj_t* statusOBDLabel = nullptr;
lv_obj_t* statusBLELabel = nullptr;
lv_obj_t* statusVoltageLabel = nullptr;
lv_obj_t* statusLatencyLabel = nullptr;

lv_obj_t* scanScreen = nullptr;
lv_obj_t* scanListWidget = nullptr;
lv_obj_t* scanListItemBtn[MAX_SCAN_DEVICES];
lv_obj_t* scanListItemLabel[MAX_SCAN_DEVICES];
lv_obj_t* scanStatusLabel = nullptr;

lv_obj_t* metricPickerOverlay = nullptr;
lv_obj_t* metricPickerPanel = nullptr;
lv_obj_t* metricPickerMatrix = nullptr;
lv_obj_t* metricPickerTitle = nullptr;
lv_obj_t* metricPickerHint = nullptr;
uint8_t metricPickerTargetSlot = 0;
bool metricPickerSettingsMode = false;
bool metricPickerRefreshMode = false;
bool metricPickerVehicleMode = false;
bool metricPickerDiagnosticMode = false;
bool metricPickerEventBusy = false;
lv_obj_t* metricPickerDiagnosticText = nullptr;

lv_obj_t* settingsOverlay = nullptr;
lv_obj_t* dayBrightnessSlider = nullptr;
lv_obj_t* nightBrightnessSlider = nullptr;
lv_obj_t* languageDropdown = nullptr;
lv_obj_t* automaticLightSwitch = nullptr;
lv_obj_t* automaticLightLabel = nullptr;
lv_obj_t* manualLightButton = nullptr;
lv_obj_t* manualLightButtonLabel = nullptr;
lv_obj_t* brightnessDimmer = nullptr;
lv_obj_t* settingsTitleLabel = nullptr;
lv_obj_t* dayBrightnessLabel = nullptr;
lv_obj_t* nightBrightnessLabel = nullptr;
lv_obj_t* dayBrightnessValueLabel = nullptr;
lv_obj_t* nightBrightnessValueLabel = nullptr;
lv_obj_t* languageLabel = nullptr;

uint8_t dayBrightness = 100;
uint8_t nightBrightness = 35;
uint8_t uiLanguage = 0;  
bool automaticLightMode = true;
bool manualNightMode = false;
bool displayRotated = false;
bool vehicleLightsOn = false;
bool pendingLightState = false;
unsigned long lightStateChangedAt = 0;
unsigned long lastLightInputPollAt = 0;

static const char* vehicleProfileNames[VEH_PROFILE_COUNT] = {
    "AUTO", "OPEL", "ASTRA", "ASTRA OPC", "CORSA", "INSIGNIA", "MOKKA", "ZAFIRA",
    "MERIVA", "ADAM", "GRANDLAND", "CROSSLAND", "COMBO", "VIVARO", "KARL", "CASCADA",
    "ANTARA", "SIGNUM", "VECTRA", "TIGRA", "FRONTERA", "AMPERA",
    "AGILA", "MOVANO", "OMEGA", "GT", "SPEEDSTER", "MONTEREY"
};

VehicleProfile vehicleProfileSetting = VEH_ASTRA_OPC;    
VehicleProfile vehicleProfileDetected = VEH_OPEL;
char vehicleVIN[18] = "";
bool vehicleVinValid = false;

lv_obj_t* vehicleModelLabel = nullptr;
lv_obj_t* vehicleTrimLabel = nullptr;
lv_obj_t* vehicleSubHeaderLabel = nullptr;

volatile bool vinCaptureActive = false;
volatile bool vinCaptureComplete = false;
volatile uint16_t vinCaptureIndex = 0;
char vinCaptureBuffer[512] = "";

enum VinQueryState : uint8_t { VIN_IDLE, VIN_PENDING, VIN_WAIT, VIN_DONE };
VinQueryState vinQueryState = VIN_IDLE;
unsigned long vinQueryStartedAt = 0;

enum PidSupportQueryState : uint8_t { PID_SUPPORT_IDLE, PID_SUPPORT_PENDING, PID_SUPPORT_WAIT, PID_SUPPORT_DONE };
PidSupportQueryState pidSupportQueryState = PID_SUPPORT_IDLE;
const uint8_t pidSupportBases[] = { 0x00, 0x20, 0x40 };
const uint8_t PID_SUPPORT_RANGE_COUNT = sizeof(pidSupportBases) / sizeof(pidSupportBases[0]);
uint32_t pidSupportMask[PID_SUPPORT_RANGE_COUNT] = {0, 0, 0};
bool pidSupportRangeKnown[PID_SUPPORT_RANGE_COUNT] = {false, false, false};
uint8_t pidSupportRangeIndex = 0;
unsigned long pidSupportQueryStartedAt = 0;

char vehicleDetectedTrim[24] = "";
bool vehicleDetectedIsAstraJ = false;

Preferences prefs;

WebServer otaServer(OTA_HTTP_PORT);
bool otaRoutesConfigured = false;
bool otaModeActive = false;
volatile bool otaStartRequested = false;
volatile bool otaStopRequested = false;
bool otaBleReleased = false;
uint32_t otaStopButtonArmedAt = 0;
bool otaUploadRunning = false;
bool otaUploadSucceeded = false;
bool otaRebootPending = false;
unsigned long otaRebootAt = 0;
size_t otaBytesWritten = 0;
size_t otaExpectedBytes = 0;
char otaApSsid[32] = "";
char otaApPassword[12] = "";
uint32_t otaSessionStartedAt = 0;
uint32_t otaLastActivityAt = 0;

lv_obj_t* otaOverlay = nullptr;
lv_obj_t* otaTitleLabel = nullptr;
lv_obj_t* otaInfoLabel = nullptr;
lv_obj_t* otaStatusLabel = nullptr;
lv_obj_t* otaStopButton = nullptr;

char otaUiStatus[128] = "Bereit";
volatile bool otaUiDirty = false;

struct DtcEntry {
    char code[6];
    uint8_t group;  
};
#define MAX_DTC_ENTRIES 16
DtcEntry dtcEntries[MAX_DTC_ENTRIES];
uint8_t dtcCount = 0;

enum DiagnosticState : uint8_t {
    DIAG_IDLE, DIAG_SEND_CONFIRMED, DIAG_WAIT_CONFIRMED,
    DIAG_SEND_PENDING, DIAG_WAIT_PENDING,
    DIAG_SEND_PERMANENT, DIAG_WAIT_PERMANENT,
    DIAG_READY, DIAG_SEND_CLEAR, DIAG_WAIT_CLEAR
};
DiagnosticState diagnosticState = DIAG_IDLE;
bool diagnosticModeActive = false;
volatile bool diagnosticRefreshRequested = false;
volatile bool diagnosticClearRequested = false;
uint32_t diagnosticStateStartedAt = 0;
uint32_t diagnosticEnterAt = 0;
uint32_t diagnosticClearArmedUntil = 0;
volatile bool diagCaptureActive = false;
volatile bool diagCaptureComplete = false;
volatile uint16_t diagCaptureIndex = 0;
char diagCaptureBuffer[512] = "";
char diagnosticUiStatus[112] = "Bereit";
char diagnosticUiText[512] = "Noch keine Abfrage";
volatile bool diagnosticUiDirty = false;

bool sdLoggingEnabled = false;
volatile bool sdInitRequested = false;
volatile bool sdStopRequested = false;
bool sdCardReady = false;
bool sdLogFileReady = false;
File sdLogFile;
char sdLogPath[48] = "";
uint32_t sdLastEnqueueAt = 0;
uint32_t sdLastFlushAt = 0;
uint32_t sdDroppedRecords = 0;
uint8_t sdPendingCount = 0;
uint8_t sdSlowIoStrikes = 0;
uint32_t sdLastIoMs = 0;

struct LogRecord {
    uint32_t ms;
    float speed, rpm, boost, oil, coolant, lambda, maf, load, throttle, consumption, voltage;
};
#define SD_LOG_RING_SIZE 8
LogRecord sdLogRing[SD_LOG_RING_SIZE];
uint8_t sdLogHead = 0;
uint8_t sdLogTail = 0;

uint16_t crashStreak = 0;
bool professionalWatchdogActive = false;
bool healthyBootMarked = false;
uint32_t bootStartedAt = 0;

volatile bool displaySettingsPersistDirty = false;
volatile bool dashboardPersistDirty = false;
volatile uint32_t displaySettingsPersistDueAt = 0;
volatile uint32_t dashboardPersistDueAt = 0;

Board* displayBoard = nullptr;
esp_expander::Base* boardIOExpander = nullptr;

static const char* pickerMapDe[] = { "GESCHWINDIGKEIT", "DREHZAHL", "\n", "LADEDRUCK", "MOTORLAST", "\n", "KUEHLWASSER", "OELTEMP", "\n", "ANSAUGLUFT", "TANK", "\n", "SPANNUNG", "DROSSELKLAPPE", "\n", "LAMBDA", "LUFTMASSE", "\n", "LEISTUNG", "VERBRAUCH", "\n", "WERT AUSBLENDEN", "" };
static const char* pickerMapEn[] = { "SPEED", "RPM", "\n", "BOOST", "ENGINE LOAD", "\n", "COOLANT", "OIL TEMP", "\n", "INTAKE AIR", "FUEL LEVEL", "\n", "VOLTAGE", "THROTTLE", "\n", "LAMBDA", "MAF", "\n", "POWER", "CONSUMPTION", "\n", "HIDE VALUE", "" };
char settingsLoggingLabel[20] = "LOGGING: AN";
static const char* settingsMapAuto[]   = { "TAG -", "TAG +", "\n", "NACHT -", "NACHT +", "\n", "AUTOMATIK: AN", "\n", "DEUTSCH", "ENGLISH", "\n", "ROTATE 180", "\n", "FAHRZEUG", "DIAGNOSE", "\n", settingsLoggingLabel, "OTA UPDATE", "\n", "FERTIG", "" };
static const char* settingsMapManual[] = { "TAG -", "TAG +", "\n", "NACHT -", "NACHT +", "\n", "AUTOMATIK: AUS", "\n", "DEUTSCH", "ENGLISH", "\n", "ROTATE 180", "\n", "FAHRZEUG", "DIAGNOSE", "\n", settingsLoggingLabel, "OTA UPDATE", "\n", "FERTIG", "" };
static const char* diagnosticActionMap[] = { "AKTUALISIEREN", "FEHLER LOESCHEN", "\n", "ZURUECK", "" };
static const char* refreshRateMap[]    = { "1 x/s", "2 x/s", "\n", "5 x/s", "10 x/s", "\n", "20 x/s", "" };
static const uint8_t refreshRateValues[] = { 1, 2, 5, 10, 20 };

static const char* vehicleProfileMap[] = {
    "AUTO", "OPEL", "ASTRA", "ASTRA OPC", "\n",
    "CORSA", "INSIGNIA", "MOKKA", "ZAFIRA", "\n",
    "MERIVA", "ADAM", "GRANDLAND", "CROSSLAND", "\n",
    "COMBO", "VIVARO", "KARL", "CASCADA", "\n",
    "ANTARA", "SIGNUM", "VECTRA", "TIGRA", "\n",
    "FRONTERA", "AMPERA", "AGILA", "MOVANO", "\n",
    "OMEGA", "GT", "SPEEDSTER", "MONTEREY", ""
};

void updateVehicleHeader();
void processVinQuery();
void processPidSupportQuery();
void resetPIDTimings();

void updateDashboardPanelIcon(uint8_t slot, MetricId metric);
void onConnectionLabelClicked(lv_event_t* event);
void createSettingsOverlay();
void showSettingsPicker();
void updateManualLightControl();
void applyLanguageToDashboard();
void createOtaOverlay();
void requestStartOtaMode();
void stopOtaMode();
void serviceOtaMode();
void serviceDiagnosticMode();
void openDiagnosticPicker();
void initSdLogging();
void serviceSdLogging();
void enqueueSdLogRecord();
void initProfessionalWatchdog();
void recordBootHealth();
void markHealthyBootIfReady();
void bootSetStatus(const char* text);

lv_obj_t* connectOverlay = nullptr;
lv_obj_t* connectStepsLabel = nullptr;
char connectStepsBuf[600] = "";

unsigned long connectOverlayCloseAt = 0;

bool connectOverlayUserClosed = false;

unsigned long lastUIUpdate = 0;

int8_t lastConnectionUiState = -1;
int32_t lastDisplayedSpeed = INT32_MIN;
int32_t lastDisplayedRpm = INT32_MIN;
int8_t lastRpmColorBand = -1;
lv_coord_t lastNeedleX = INT16_MIN;
lv_coord_t lastNeedleY = INT16_MIN;
int8_t dashboardAlertState[DASHBOARD_SLOT_COUNT] = { -1, -1, -1, -1, -1, -1, -1, -1 };
int8_t dashboardFreshState[DASHBOARD_SLOT_COUNT] = { -1, -1, -1, -1, -1, -1, -1, -1 };
int8_t lastSpeedFreshState = -1;
int8_t lastRpmFreshState = -1;
int8_t lastVoltageFreshState = -1;
int8_t centerMetricFreshState[3] = { -1, -1, -1 };
bool lastAlertBlinkPhase = false;

static bool setLabelTextIfChanged(lv_obj_t* label, const char* text) {
    if (!label || !text) return false;
    const char* current = lv_label_get_text(label);
    if (current && strcmp(current, text) == 0) return false;
    lv_label_set_text(label, text);
    return true;
}

static bool readVehicleLightInput() {
    if (!boardIOExpander) return false;
    return boardIOExpander->digitalRead(LIGHT_SENSE_EXIO) == HIGH;
}

static constexpr int RPM_CX = 512;
static constexpr int RPM_CY = 326;
static constexpr float RPM_RX = 220.0f;
static constexpr float RPM_RY = 155.0f;
lv_point_t rpmBasePoints[61];
lv_point_t rpmZonePoints[4][25];
lv_point_t rpmNeedlePoints[2];
lv_obj_t* rpmBaseLine = nullptr;
lv_obj_t* rpmZoneLine[4] = { nullptr, nullptr, nullptr, nullptr };

class OBDClientCallbacks : public BLEClientCallbacks {
    void onConnect(BLEClient* client) override {
        appLog("[BLE] Connected");
    }

    void onDisconnect(BLEClient* client) override {
        appLog("[BLE] DISCONNECTED");

        obdConnected = false;
        elmInitialized = false;
        bleDisconnectedEvent = true;

        obdRead = nullptr;
        obdWrite = nullptr;
        portENTER_CRITICAL(&rxMux);
        diagCaptureActive = false;
        vinCaptureActive = false;
        portEXIT_CRITICAL(&rxMux);

    }
};

static OBDClientCallbacks obdClientCallbacks;

void notifyCallback(
    BLERemoteCharacteristic* characteristic,
    uint8_t* payload,
    size_t length,
    bool isNotify
) {
    portENTER_CRITICAL(&rxMux);

    if (diagCaptureActive) {
        for (size_t i = 0; i < length; i++) {
            const char c = (char)payload[i];
            if (c == '>') {
                diagCaptureComplete = true;
                diagCaptureActive = false;
                continue;
            }
            if (diagCaptureIndex < sizeof(diagCaptureBuffer) - 2) {
                diagCaptureBuffer[diagCaptureIndex++] = (c == '\r' || c == '\n') ? '\n' : c;
                diagCaptureBuffer[diagCaptureIndex] = '\0';
            }
        }
        portEXIT_CRITICAL(&rxMux);
        return;
    }

    if (vinCaptureActive) {
        for (size_t i = 0; i < length; i++) {
            const char c = (char)payload[i];
            if (c == '>') {
                vinCaptureComplete = true;
                vinCaptureActive = false;
                continue;
            }
            if (vinCaptureIndex < sizeof(vinCaptureBuffer) - 2) {
                vinCaptureBuffer[vinCaptureIndex++] = (c == '\r' || c == '\n') ? '\n' : c;
                vinCaptureBuffer[vinCaptureIndex] = '\0';
            }
        }
        portEXIT_CRITICAL(&rxMux);
        return;
    }

    for (size_t i = 0; i < length; i++) {
        const char c = (char)payload[i];
        const bool lineEnd = (c == '\r' || c == '\n' || c == '>');

        if (lineEnd) {
            if (rxBuildIndex > 0) {
                rxBuildLine[rxBuildIndex] = '\0';

                if (rxQueueCount >= RX_LINE_QUEUE_DEPTH) {
                    rxQueueTail = (uint8_t)((rxQueueTail + 1) % RX_LINE_QUEUE_DEPTH);
                    rxQueueCount--;
                    rxDroppedLines++;
                }

                strncpy((char*)rxLineQueue[rxQueueHead], (const char*)rxBuildLine, RX_LINE_QUEUE_SIZE - 1);
                rxLineQueue[rxQueueHead][RX_LINE_QUEUE_SIZE - 1] = '\0';
                rxQueueHead = (uint8_t)((rxQueueHead + 1) % RX_LINE_QUEUE_DEPTH);
                rxQueueCount++;
                rxLineReady = true;
                rxBuildIndex = 0;
            }
            continue;
        }

        if (rxBuildIndex < RX_LINE_QUEUE_SIZE - 1) {
            rxBuildLine[rxBuildIndex++] = c;
        } else {

            rxBuildIndex = 0;
            rxDroppedLines++;
        }
    }

    portEXIT_CRITICAL(&rxMux);
}

void clearRxBuffer() {
    portENTER_CRITICAL(&rxMux);
    rxBuildIndex = 0;
    rxQueueHead = 0;
    rxQueueTail = 0;
    rxQueueCount = 0;
    rxLineReady = false;
    rxBuildLine[0] = '\0';
    portEXIT_CRITICAL(&rxMux);
}

bool getRxLine(char* destination, size_t destinationSize) {
    if (!destination || destinationSize == 0) return false;

    bool ready = false;
    portENTER_CRITICAL(&rxMux);
    if (rxQueueCount > 0) {
        strncpy(destination, (const char*)rxLineQueue[rxQueueTail], destinationSize - 1);
        destination[destinationSize - 1] = '\0';
        rxQueueTail = (uint8_t)((rxQueueTail + 1) % RX_LINE_QUEUE_DEPTH);
        rxQueueCount--;
        rxLineReady = (rxQueueCount > 0);
        ready = true;
    } else {
        rxLineReady = false;
    }
    portEXIT_CRITICAL(&rxMux);
    return ready;
}

bool sendOBDCommand(const char* command) {
    if (!obdConnected || !obdWrite) return false;

    clearRxBuffer();

    char commandBuffer[20];
    snprintf(commandBuffer, sizeof(commandBuffer), "%s\r", command);

    return obdWrite->writeValue((uint8_t*)commandBuffer, strlen(commandBuffer), false);
}

static VehicleProfile effectiveVehicleProfile() {
    if (vehicleProfileSetting != VEH_AUTO) return vehicleProfileSetting;
    return vehicleProfileDetected;
}

void updateVehicleHeader() {
    if (!vehicleModelLabel) return;

    VehicleProfile p = effectiveVehicleProfile();
    const char* model = "OPEL";
    const char* trim = "";

    switch (p) {
        case VEH_ASTRA:      model = "ASTRA"; break;
        case VEH_ASTRA_OPC:  model = "ASTRA"; trim = "OPC"; break;
        case VEH_CORSA:      model = "CORSA"; break;
        case VEH_INSIGNIA:   model = "INSIGNIA"; break;
        case VEH_MOKKA:      model = "MOKKA"; break;
        case VEH_ZAFIRA:     model = "ZAFIRA"; break;
        case VEH_MERIVA:     model = "MERIVA"; break;
        case VEH_ADAM:       model = "ADAM"; break;
        case VEH_GRANDLAND:  model = "GRANDLAND"; break;
        case VEH_CROSSLAND:  model = "CROSSLAND"; break;
        case VEH_COMBO:      model = "COMBO"; break;
        case VEH_VIVARO:     model = "VIVARO"; break;
        case VEH_KARL:       model = "KARL"; break;
        case VEH_CASCADA:    model = "CASCADA"; break;
        case VEH_ANTARA:     model = "ANTARA"; break;
        case VEH_SIGNUM:     model = "SIGNUM"; break;
        case VEH_VECTRA:     model = "VECTRA"; break;
        case VEH_TIGRA:      model = "TIGRA"; break;
        case VEH_FRONTERA:   model = "FRONTERA"; break;
        case VEH_AMPERA:     model = "AMPERA"; break;
        case VEH_AGILA:      model = "AGILA"; break;
        case VEH_MOVANO:     model = "MOVANO"; break;
        case VEH_OMEGA:      model = "OMEGA"; break;
        case VEH_GT:         model = "GT"; break;
        case VEH_SPEEDSTER:  model = "SPEEDSTER"; break;
        case VEH_MONTEREY:   model = "MONTEREY"; break;
        case VEH_OPEL:
        case VEH_AUTO:
        default:             model = "OPEL"; break;
    }

    if (vehicleProfileSetting == VEH_AUTO && vehicleDetectedTrim[0]) {
        trim = vehicleDetectedTrim;
    }

    setLabelTextIfChanged(vehicleModelLabel, model);
    if (vehicleTrimLabel) {
        setLabelTextIfChanged(vehicleTrimLabel, trim);
        if (trim[0]) lv_obj_clear_flag(vehicleTrimLabel, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(vehicleTrimLabel, LV_OBJ_FLAG_HIDDEN);
    }

    if (vehicleSubHeaderLabel) {
        const bool astraOpcTarget =
            vehicleProfileSetting == VEH_ASTRA_OPC ||
            (vehicleProfileSetting == VEH_AUTO && vehicleDetectedIsAstraJ && strstr(vehicleDetectedTrim, "OPC"));
        setLabelTextIfChanged(
            vehicleSubHeaderLabel,
            astraOpcTarget ? "2.0 TURBO  |  PERFORMANCE" : "OPEL  |  PERFORMANCE"
        );
    }
}

static bool parseVinResponse(const char* raw, char outVin[18]) {
    if (!raw || !outVin) return false;
    char collected[18] = "";
    uint8_t count = 0;

    const char* p = raw;
    while (*p && count < 17) {
        char line[160];
        size_t n = 0;
        while (*p && *p != '\n' && n < sizeof(line) - 1) line[n++] = *p++;
        if (*p == '\n') p++;
        line[n] = '\0';

        char hex[160];
        size_t h = 0;
        for (size_t i = 0; line[i] && h < sizeof(hex) - 1; i++) {
            if (isxdigit((unsigned char)line[i])) hex[h++] = toupper((unsigned char)line[i]);
        }
        hex[h] = '\0';

        char* marker = strstr(hex, "4902");
        if (!marker) continue;
        marker += 4;
        if (strlen(marker) >= 2) marker += 2;  

        while (strlen(marker) >= 2 && count < 17) {
            char b[3] = { marker[0], marker[1], '\0' };
            uint8_t v = (uint8_t)strtoul(b, nullptr, 16);
            marker += 2;
            if ((v >= '0' && v <= '9') || (v >= 'A' && v <= 'Z')) collected[count++] = (char)v;
        }
    }

    if (count != 17) return false;
    collected[17] = '\0';
    strncpy(outVin, collected, 18);
    return true;
}

static VehicleProfile detectOpelProfileFromVin(const char* vin) {
    vehicleDetectedTrim[0] = '\0';
    vehicleDetectedIsAstraJ = false;
    if (!vin || strlen(vin) != 17) return VEH_OPEL;

    const bool gmOpelWmi =
        strncmp(vin, "W0L", 3) == 0 ||
        strncmp(vin, "W0V", 3) == 0 ||
        strncmp(vin, "XUF", 3) == 0 ||
        strncmp(vin, "XWF", 3) == 0 ||
        strncmp(vin, "XUU", 3) == 0;

    if (strncmp(vin, "1G0R8", 5) == 0) {
        snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "");
        return VEH_AMPERA;
    }
    if (strncmp(vin, "1G0MG", 5) == 0) return VEH_GT;

    if (!gmOpelWmi) return VEH_OPEL;

    if (vin[4] == '9' && strchr("EFGHJNPU", vin[3])) return VEH_MOVANO;
    if (vin[4] == '7' && strchr("EFJ", vin[3])) return VEH_VIVARO;

    switch (vin[3]) {
        case 'P': {  
            if (vin[5] == '9') {
                snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "TOURER");
                return VEH_ZAFIRA;
            }
            vehicleDetectedIsAstraJ = true;
            if (vin[5] == '2') {

                if (vin[4] == 'F' && vin[7] == 'S') snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "J GTC OPC");
                else snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "J GTC");
            } else {
                snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "J");
            }
            return VEH_ASTRA;
        }
        case 'B':
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "K");
            return VEH_ASTRA;        
        case 'G':
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", vin[4] == 'X' ? "A OPC" : "A");
            return VEH_INSIGNIA;     
        case 'Z':
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", vin[4] == 'X' ? "B GSI" : "B");
            return VEH_INSIGNIA;     
        case 'J':
            if (vin[5] == '7' && (vin[4] == 'B' || vin[4] == 'C' || vin[4] == 'D')) {
                snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "A");
                return VEH_MOKKA;    
            }
            break;
        case 'S':
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "B");
            return VEH_MERIVA;       
        case 'W':
            return VEH_CASCADA;
        case 'D':
            return VEH_KARL;
        case '6':
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "D");
            return VEH_COMBO;        
        default:
            break;
    }

    if (vin[3] == '0' && vin[4] == 'M' && vin[5] == 'A') return VEH_ADAM;

    if (vin[3] == '0') {
        const char platform = vin[4];
        const char modelCode = vin[5];
        const char body1 = vin[7];
        const char body2 = vin[8];
        const bool monocab75 = (body1 == '7' && body2 == '5');

        if (platform == 'S' && modelCode == 'D') {
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "D");
            return VEH_CORSA;
        }
        if (platform == 'X' && modelCode == 'E') {
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "E");
            return VEH_CORSA;
        }
        if (platform == 'A' && modelCode == 'H') {
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", monocab75 ? "B" : "H");
            return monocab75 ? VEH_ZAFIRA : VEH_ASTRA;
        }
        if (platform == 'T' && modelCode == 'G') {
            snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", monocab75 ? "A" : "G");
            return monocab75 ? VEH_ZAFIRA : VEH_ASTRA;
        }
    }

    if (strncmp(vin + 3, "06B", 3) == 0) {
        snprintf(vehicleDetectedTrim, sizeof(vehicleDetectedTrim), "%s", "B");
        return VEH_FRONTERA;
    }

    return VEH_OPEL;
}

static int pidSupportRangeFor(uint8_t pid) {
    if (pid >= 0x01 && pid <= 0x20) return 0;
    if (pid >= 0x21 && pid <= 0x40) return 1;
    if (pid >= 0x41 && pid <= 0x60) return 2;
    return -1;
}

static bool isStandardPidSupportKnown(uint8_t pid) {
    int range = pidSupportRangeFor(pid);
    return range >= 0 && pidSupportRangeKnown[range];
}

static bool isStandardPidSupported(uint8_t pid) {
    int range = pidSupportRangeFor(pid);
    if (range < 0 || !pidSupportRangeKnown[range]) return true;  

    const uint8_t base = pidSupportBases[range];
    const uint8_t offset = pid - (base + 1);  
    if (offset > 31) return true;
    return (pidSupportMask[range] & (1UL << (31 - offset))) != 0;
}

static bool parseSupportedPidMask(const char* response, uint8_t base, uint32_t& maskOut) {
    if (!response) return false;
    char clean[RX_BUFFER_SIZE];
    size_t j = 0;
    for (size_t i = 0; response[i] && j < sizeof(clean) - 1; i++) {
        if (isxdigit((unsigned char)response[i])) clean[j++] = toupper((unsigned char)response[i]);
    }
    clean[j] = '\0';

    char target[5];
    snprintf(target, sizeof(target), "41%02X", base);
    char* p = strstr(clean, target);
    if (!p) return false;
    p += 4;
    if (strlen(p) < 8) return false;

    char bits[9];
    memcpy(bits, p, 8);
    bits[8] = '\0';
    maskOut = strtoul(bits, nullptr, 16);
    return true;
}

void processPidSupportQuery() {
    if (!obdConnected || !elmInitialized) return;
    const unsigned long now = millis();

    if (pidSupportQueryState == PID_SUPPORT_PENDING) {
        if (pidSupportRangeIndex >= PID_SUPPORT_RANGE_COUNT) {
            pidSupportQueryState = PID_SUPPORT_DONE;
            vinQueryState = VIN_PENDING;
            resetPIDTimings();
            appLog("[PID] Capability probe complete");
            return;
        }

        char cmd[5];
        snprintf(cmd, sizeof(cmd), "01%02X", pidSupportBases[pidSupportRangeIndex]);
        clearRxBuffer();
        if (!sendOBDCommand(cmd)) {
            pidSupportRangeIndex++;
            return;
        }
        pidSupportQueryStartedAt = now;
        pidSupportQueryState = PID_SUPPORT_WAIT;
        appLog("[PID] Support query %s", cmd);
        return;
    }

    if (pidSupportQueryState == PID_SUPPORT_WAIT) {
        if (rxLineReady) {
            char response[RX_BUFFER_SIZE];
            if (getRxLine(response, sizeof(response))) {
                uint32_t mask = 0;
                const uint8_t base = pidSupportBases[pidSupportRangeIndex];
                if (parseSupportedPidMask(response, base, mask)) {
                    pidSupportMask[pidSupportRangeIndex] = mask;
                    pidSupportRangeKnown[pidSupportRangeIndex] = true;
                    appLog("[PID] 0x%02X support mask = %08lX", base, (unsigned long)mask);

                    if ((mask & 0x00000001UL) == 0) {
                        for (uint8_t r = pidSupportRangeIndex + 1; r < PID_SUPPORT_RANGE_COUNT; r++) {
                            pidSupportMask[r] = 0;
                            pidSupportRangeKnown[r] = true;
                        }
                        pidSupportRangeIndex = PID_SUPPORT_RANGE_COUNT;
                    } else {
                        pidSupportRangeIndex++;
                    }
                    pidSupportQueryState = PID_SUPPORT_PENDING;
                }

            }
            return;
        }

        const unsigned long supportTimeoutMs = (pidSupportRangeIndex == 0) ? 5000UL : 1500UL;
        if (now - pidSupportQueryStartedAt >= supportTimeoutMs) {
            appLog("[PID] Support range 0x%02X unknown (timeout)", pidSupportBases[pidSupportRangeIndex]);
            pidSupportRangeIndex++;
            pidSupportQueryState = PID_SUPPORT_PENDING;
            clearRxBuffer();
        }
    }
}

void processVinQuery() {
    if (!obdConnected || !elmInitialized) return;
    const unsigned long now = millis();

    if (vinQueryState == VIN_PENDING) {
        portENTER_CRITICAL(&rxMux);
        vinCaptureIndex = 0;
        vinCaptureBuffer[0] = '\0';
        vinCaptureComplete = false;
        vinCaptureActive = true;
        portEXIT_CRITICAL(&rxMux);
        clearRxBuffer();

        if (!sendOBDCommand("0902")) {
            portENTER_CRITICAL(&rxMux);
            vinCaptureActive = false;
            portEXIT_CRITICAL(&rxMux);
            vinQueryState = VIN_DONE;
            return;
        }
        vinQueryStartedAt = now;
        vinQueryState = VIN_WAIT;
        appLog("[VIN] Request 09 02 sent");
        return;
    }

    if (vinQueryState == VIN_WAIT) {
        bool vinDone = false;
        portENTER_CRITICAL(&rxMux);
        vinDone = vinCaptureComplete;
        portEXIT_CRITICAL(&rxMux);
        if (!vinDone && now - vinQueryStartedAt < 2500) return;

        portENTER_CRITICAL(&rxMux);
        vinCaptureActive = false;
        portEXIT_CRITICAL(&rxMux);

        char parsed[18] = "";
        if (parseVinResponse(vinCaptureBuffer, parsed)) {
            strncpy(vehicleVIN, parsed, sizeof(vehicleVIN));
            vehicleVIN[17] = '\0';
            vehicleVinValid = true;
            vehicleProfileDetected = detectOpelProfileFromVin(vehicleVIN);
            appLog("[VIN] %s", vehicleVIN);

            ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
            updateVehicleHeader();
            lvgl_port_unlock();
            resetPIDTimings();
        } else {
            appLog("[VIN] 09 02 unavailable or could not be parsed");
        }
        clearRxBuffer();
        resetPIDTimings();
        vinQueryState = VIN_DONE;
    }
}

void openConnectOverlay() {

    if (connectOverlayUserClosed) return;

    connectStepsBuf[0] = '\0';
    connectOverlayCloseAt = 0;

    ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
    if (connectStepsLabel) { lv_label_set_text(connectStepsLabel, ""); }
    if (connectOverlay) { lv_obj_clear_flag(connectOverlay, LV_OBJ_FLAG_HIDDEN); }
    lvgl_port_unlock();
}

void connectStep(const char* fmt, ...) {
    char line[100];
    va_list args;
    va_start(args, fmt);
    vsnprintf(line, sizeof(line), fmt, args);
    va_end(args);

    appLog("%s", line);

    size_t len = strlen(connectStepsBuf);
    size_t lineLen = strlen(line);

    if (len + lineLen + 2 >= sizeof(connectStepsBuf)) {

        size_t half = sizeof(connectStepsBuf) / 2;
        memmove(connectStepsBuf, connectStepsBuf + half, len - half + 1);
        len = strlen(connectStepsBuf);
    }

    if (len > 0) {
        connectStepsBuf[len++] = '\n';
        connectStepsBuf[len] = '\0';
    }

    strncat(connectStepsBuf, line, sizeof(connectStepsBuf) - strlen(connectStepsBuf) - 1);

    ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
    if (connectStepsLabel) { lv_label_set_text(connectStepsLabel, connectStepsBuf); }
    lvgl_port_unlock();
}

void bleDeleteClient() {
    if (!obdClient) return;

    if (obdClient->isConnected()) {
        obdClient->disconnect();
        delay(50);
    }

    delete obdClient;
    obdClient = nullptr;

    delay(200);
}

void disconnectOBD() {
    obdConnected = false;
    elmInitialized = false;
    obdPollingStartedAt = 0;
    obdPartialRecoveryAttempted = false;

    bleDeleteClient();

    obdRead = nullptr;
    obdWrite = nullptr;

    obdCycleActive = false;
    obdState = OBD_IDLE;
}

void clearScannedDevices() {
    xSemaphoreTake(scanDataMutex, portMAX_DELAY);

    for (uint8_t i = 0; i < scannedDeviceCount; i++) {
        if (scannedDeviceRefs[i]) {
            delete scannedDeviceRefs[i];
            scannedDeviceRefs[i] = nullptr;
        }
    }

    scannedDeviceCount = 0;

    bleDiagnosticSummary[0] = '\0';
    bleDiagnosticDetail[0] = '\0';
    bleDiagnosticHasDetail = false;

    xSemaphoreGive(scanDataMutex);
}

bool autoDiscoverOBDCharacteristics() {
    if (!obdClient) return false;

    std::map<std::string, BLERemoteService*>* services = obdClient->getServices();
    if (!services) return false;

    for (auto& serviceEntry : *services) {
        BLERemoteService* service = serviceEntry.second;

        std::map<std::string, BLERemoteCharacteristic*>* chars = service->getCharacteristics();
        if (!chars) continue;

        BLERemoteCharacteristic* notifyChar = nullptr;
        BLERemoteCharacteristic* writeChar = nullptr;

        for (auto& charEntry : *chars) {
            BLERemoteCharacteristic* c = charEntry.second;

            if (!notifyChar && (c->canNotify() || c->canIndicate())) {
                notifyChar = c;
            }

            if (!writeChar && (c->canWrite() || c->canWriteNoResponse())) {
                writeChar = c;
            }
        }

        if (notifyChar && writeChar) {
            obdRead = notifyChar;
            obdWrite = writeChar;
            appLog("[BLE] Auto-discovered OBD characteristics");
            return true;
        }
    }

    return false;
}

bool bleResolveCharacteristics() {
    obdRead = nullptr;
    obdWrite = nullptr;

    BLERemoteService* service = obdClient->getService(OBD_SERVICE_UUID);

    if (service) {
        obdRead = service->getCharacteristic(OBD_READ_UUID);
        obdWrite = service->getCharacteristic(OBD_WRITE_UUID);
    }

    if (obdRead && obdWrite) return true;

    return autoDiscoverOBDCharacteristics();
}

void appendServiceDiagnostics() {
    xSemaphoreTake(scanDataMutex, portMAX_DELAY);

    bleDiagnosticDetail[0] = '\0';
    bleDiagnosticHasDetail = false;

    if (!obdClient) { xSemaphoreGive(scanDataMutex); return; }

    std::map<std::string, BLERemoteService*>* services = obdClient->getServices();
    if (!services) { xSemaphoreGive(scanDataMutex); return; }

    size_t used = 0;

    for (auto& serviceEntry : *services) {
        BLERemoteService* service = serviceEntry.second;

        std::map<std::string, BLERemoteCharacteristic*>* chars = service->getCharacteristics();
        if (!chars) continue;

        for (auto& charEntry : *chars) {
            BLERemoteCharacteristic* c = charEntry.second;

            char line[96];
            snprintf(
                line,
                sizeof(line),
                "%s  N=%d W=%d",
                c->getUUID().toString().c_str(),
                c->canNotify() || c->canIndicate(),
                c->canWrite() || c->canWriteNoResponse()
            );

            size_t lineLen = strlen(line);

            if (used + lineLen + 1 < sizeof(bleDiagnosticDetail)) {
                strcpy(bleDiagnosticDetail + used, line);
                used += lineLen;
                bleDiagnosticDetail[used++] = '\n';
                bleDiagnosticDetail[used] = '\0';
            }
        }
    }

    bleDiagnosticHasDetail = (used > 0);

    xSemaphoreGive(scanDataMutex);
}

bool bleAttemptConnect(BLEAdvertisedDevice* device) {
    openConnectOverlay();

    connectStep("RC11: Connectpfad");
    connectStep("Vgate | %s | Typ=%u | RSSI=%d",
                device->getAddress().toString().c_str(),
                (unsigned)device->getAddressType(),
                device->getRSSI());

    if (ESP.getFreeHeap() < BLE_MIN_FREE_HEAP) {
        connectStep("FEHLER: zu wenig Speicher (%u bytes frei)", (unsigned)ESP.getFreeHeap());
        return false;
    }

    BLEDevice::getScan()->stop();
    delay(50);

    if (obdClient) {
        bleDeleteClient();
    }

    static constexpr uint8_t BLE_CONNECT_ATTEMPTS = 3;
    bool connectResult = false;
    bool clientConnected = false;

    for (uint8_t attempt = 1; attempt <= BLE_CONNECT_ATTEMPTS; attempt++) {
        if (attempt > 1) {
            if (obdClient) {
                bleClientDropOrphanLink(obdClient);
                bleDeleteClient();
            }
            if (professionalWatchdogActive) esp_task_wdt_reset();
            delay(attempt == 2 ? 600 : 1500);
            if (professionalWatchdogActive) esp_task_wdt_reset();
        }

        obdClient = BLEDevice::createClient();
        if (!obdClient) {
            connectStep("FEHLER: BLEClient create");
            return false;
        }
        obdClient->setClientCallbacks(&obdClientCallbacks);

        bleClientSetConnectTimeout(obdClient, 6000);

        connectStep("Connect Versuch %u/%u START", (unsigned)attempt, (unsigned)BLE_CONNECT_ATTEMPTS);

        bool watchdogPausedForConnect = false;
        if (professionalWatchdogActive && esp_task_wdt_delete(nullptr) == ESP_OK) {
            watchdogPausedForConnect = true;
        }
        unsigned long t0 = millis();
        connectResult = obdClient->connectTimeout(device, 6000);
        unsigned long dt = millis() - t0;
        if (watchdogPausedForConnect) {
            esp_task_wdt_add(nullptr);
            esp_task_wdt_reset();
        }
        clientConnected = obdClient->isConnected();

        const int bleErr = bleClientLastError(obdClient);
        connectStep("CONNECT ENDE | ret=%d conn=%d | %lu ms | rc=%d",
                    connectResult ? 1 : 0,
                    clientConnected ? 1 : 0,
                    dt,
                    bleErr);
        if (!connectResult && !clientConnected && bleErr == BLE_HS_EALREADY &&
            bleClientAdoptEstablishedLink(obdClient)) {

            connectStep("Link aktiv (MTU vom Adapter) - uebernommen");
            appLog("[BLE] Connected");
            connectResult = true;
            clientConnected = true;
        } else if (!connectResult && !clientConnected && bleErr != 0) {
            const char* errText = BLEUtils::returnCodeToString(bleErr);
            if (errText && errText[0]) connectStep("NimBLE: %s", errText);
        }

        if (connectResult || clientConnected) break;
    }

    if (!connectResult && !clientConnected) {
        connectStep("FEHLER: Verbindung nach 3 Versuchen fehlgeschlagen");
        bleClientDropOrphanLink(obdClient);
        bleDeleteClient();

        xSemaphoreTake(scanDataMutex, portMAX_DELAY);
        strncpy(bleDiagnosticSummary, "BLE V6-Connect fehlgeschlagen", sizeof(bleDiagnosticSummary) - 1);
        bleDiagnosticSummary[sizeof(bleDiagnosticSummary) - 1] = '\0';
        bleDiagnosticHasDetail = false;
        xSemaphoreGive(scanDataMutex);
        return false;
    }

    if (!connectResult && clientConnected) {
        connectStep("Connect ret=0, Link aber aktiv - weiter");
    } else {
        connectStep("BLE-Verbindung hergestellt");
    }

    delay(300);
    connectStep("Suche OBD-Service/Characteristics...");

    if (!bleResolveCharacteristics()) {
        connectStep("FEHLER: keine passende Notify+Write-Char");

        xSemaphoreTake(scanDataMutex, portMAX_DELAY);
        strncpy(bleDiagnosticSummary, "Keine passende Notify+Write-Char gefunden", sizeof(bleDiagnosticSummary) - 1);
        bleDiagnosticSummary[sizeof(bleDiagnosticSummary) - 1] = '\0';
        xSemaphoreGive(scanDataMutex);

        appendServiceDiagnostics();
        disconnectOBD();
        return false;
    }

    connectStep("Charakteristik gefunden");

    if (obdRead->canNotify()) {
        obdRead->registerForNotify(notifyCallback);
        connectStep("Benachrichtigungen aktiviert");
    }

    obdConnected = true;
    bleDisconnectedEvent = false;
    bleShowingSelector = false;
    obdPollingStartedAt = 0;
    obdPartialRecoveryAttempted = false;
    connectStep("OBD-Kanal bereit - starte ELM327...");
    return true;
}

volatile bool bleScanInProgress = false;
volatile bool bleScanComplete = false;
BLEScanResults bleLastScanResults;
unsigned long bleScanStartedAt = 0;

bool isOBDCandidate(BLEAdvertisedDevice& device) {
    String name = device.haveName() ? device.getName().c_str() : "";
    name.toUpperCase();
    bool hasOBDService = device.haveServiceUUID() && device.isAdvertisingService(OBD_SERVICE_UUID);

    return name.indexOf("VGATE") >= 0 ||
           name.indexOf("V-LINK") >= 0 ||
           name.indexOf("VLINK") >= 0 ||
           name.indexOf("IOS-VLINK") >= 0 ||
           name.indexOf("ANDROID-VLINK") >= 0 ||
           hasOBDService;
}

volatile bool bleEarlyCandidateFound = false;
BLEAdvertisedDevice* bleEarlyCandidateDevice = nullptr;

class OBDAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {

        if (bleEarlyCandidateFound) return;
        if (!isOBDCandidate(advertisedDevice)) return;

        bleEarlyCandidateDevice = new BLEAdvertisedDevice(advertisedDevice);
        bleEarlyCandidateFound = true;

        BLEDevice::getScan()->stop();
    }
};

static OBDAdvertisedDeviceCallbacks obdAdvertisedDeviceCallbacks;

void onBleScanComplete(BLEScanResults results) {
    bleLastScanResults = results;
    appLog("[BLE] Scan complete, %d devices", results.getCount());

    bleScanInProgress = false;

    if (bleEarlyCandidateFound) {

        return;
    }

    bleScanComplete = true;
}

void startBleScanAsync() {
    if (bleScanInProgress) return;

    appLog("[BLE] Scanning for Vgate... (free heap: %u bytes)", (unsigned)ESP.getFreeHeap());

    clearScannedDevices();

    if (bleEarlyCandidateDevice) {
        delete bleEarlyCandidateDevice;
        bleEarlyCandidateDevice = nullptr;
    }
    bleEarlyCandidateFound = false;

    BLEScan* scan = BLEDevice::getScan();
    scan->setActiveScan(true);
    scan->setInterval(100);
    scan->setWindow(80);

    bleScanInProgress = true;
    bleScanComplete = false;
    bleScanStartedAt = millis();

    scan->start(BLE_SCAN_SECONDS, onBleScanComplete, false);
}

void showScanSelector();

bool processScanResults() {
    BLEAdvertisedDevice* found = nullptr;
    int count = bleLastScanResults.getCount();

    for (int i = 0; i < count; i++) {
        BLEAdvertisedDevice device = bleLastScanResults.getDevice(i);

        String name = device.haveName() ? device.getName().c_str() : "";
        bool hasOBDService = device.haveServiceUUID() && device.isAdvertisingService(OBD_SERVICE_UUID);
        bool candidate = isOBDCandidate(device);

        if (candidate && !found) {
            found = new BLEAdvertisedDevice(device);
        }

        xSemaphoreTake(scanDataMutex, portMAX_DELAY);

        if (scannedDeviceCount < MAX_SCAN_DEVICES) {
            uint8_t idx = scannedDeviceCount;

            strncpy(scannedDevices[idx].name, name.length() ? name.c_str() : "", sizeof(scannedDevices[idx].name) - 1);
            scannedDevices[idx].name[sizeof(scannedDevices[idx].name) - 1] = '\0';

            strncpy(scannedDevices[idx].address, device.getAddress().toString().c_str(), sizeof(scannedDevices[idx].address) - 1);
            scannedDevices[idx].address[sizeof(scannedDevices[idx].address) - 1] = '\0';

            scannedDevices[idx].hasOBDService = hasOBDService;
            scannedDeviceRefs[idx] = new BLEAdvertisedDevice(device);

            scannedDeviceCount++;
        }

        xSemaphoreGive(scanDataMutex);
    }

    BLEDevice::getScan()->clearResults();

    if (bleShowingSelector) {
        if (found) {
            delete found;
            found = nullptr;
        }

        showScanSelector();
        return false;
    }

    if (!found) {
        appLog("[BLE] Vgate not found (%d devices seen)", count);
        return false;
    }

    bool ok = bleAttemptConnect(found);
    delete found;

    return ok;
}

bool connectToScannedDevice(uint8_t idx) {
    if (idx >= scannedDeviceCount || !scannedDeviceRefs[idx]) return false;
    return bleAttemptConnect(scannedDeviceRefs[idx]);
}

void resetPIDTimings();

void startELMInitialization() {
    connectStep("Initialisiere ELM327 (robuster Kaltstart)...");

    elmState = ELM_ATZ;
    elmTimer = 0;
    elmNotBefore = millis() + 1200UL;
    elmInitAttempt = 1;
    elmStepRetry = 0;
    elmInitialized = false;
    vinQueryState = VIN_IDLE;
    pidSupportQueryState = PID_SUPPORT_IDLE;
    pidSupportRangeIndex = 0;
    for (uint8_t i = 0; i < PID_SUPPORT_RANGE_COUNT; i++) {
        pidSupportRangeKnown[i] = false;
        pidSupportMask[i] = 0;
    }
    portENTER_CRITICAL(&rxMux);
    vinCaptureActive = false;
    vinCaptureComplete = false;
    portEXIT_CRITICAL(&rxMux);

    clearRxBuffer();
}

static void advanceElmStep(uint8_t next) {

    elmState = (ELMState)next;
    elmTimer = 0;
    elmStepRetry = 0;
    elmNotBefore = millis() + 120UL;
    clearRxBuffer();
}

static void retryElmStepOrSequence(const char* stepName) {
    const unsigned long now = millis();
    clearRxBuffer();

    if (elmStepRetry < 2) {
        elmStepRetry++;
        elmTimer = 0;
        elmNotBefore = now + 350UL;
        connectStep("%s ohne Antwort - Wiederholung %u/2", stepName, (unsigned)elmStepRetry);
        return;
    }

    if (elmInitAttempt < 3) {
        elmInitAttempt++;
        elmStepRetry = 0;
        elmState = ELM_ATZ;
        elmTimer = 0;
        elmNotBefore = now + 1200UL;
        connectStep("ELM Kaltstart - kompletter Versuch %u/3", (unsigned)elmInitAttempt);
        return;
    }

    connectStep("FEHLER: %s nach 3 Initialisierungen", stepName);
    elmState = ELM_FAILED;
}

void processELMInitialization() {
    if (!obdConnected) return;

    unsigned long now = millis();
    if ((int32_t)(now - elmNotBefore) < 0) return;

    switch (elmState) {
        case ELM_ATZ:
            if (elmTimer == 0) {
                clearRxBuffer();

                if (!sendOBDCommand("ATZ")) {
                    retryElmStepOrSequence("ATZ Write");
                    return;
                }

                elmTimer = now;
                return;
            }

            if (rxLineReady) {
                char response[RX_BUFFER_SIZE];

                if (getRxLine(response, sizeof(response))) {
                    connectStep("ATZ -> %s", response);
                    advanceElmStep(ELM_ATE0);
                }

                return;
            }

            if (now - elmTimer > 5000) {
                retryElmStepOrSequence("ATZ Timeout");
            }

            break;

        case ELM_ATE0:
            if (elmTimer == 0) {
                if (!sendOBDCommand("ATE0")) {
                    retryElmStepOrSequence("ATE0 Write");
                    return;
                }

                elmTimer = now;
                return;
            }

            if (rxLineReady) {
                clearRxBuffer();
                connectStep("ATE0 OK (Echo aus)");
                advanceElmStep(ELM_ATL0);
                return;
            }

            if (now - elmTimer > 2200) {
                retryElmStepOrSequence("ATE0 Timeout");
            }

            break;

        case ELM_ATL0:
            if (elmTimer == 0) {
                if (!sendOBDCommand("ATL0")) {
                    retryElmStepOrSequence("ATL0 Write");
                    return;
                }

                elmTimer = now;
                return;
            }

            if (rxLineReady) {
                clearRxBuffer();
                connectStep("ATL0 OK (Linefeed aus)");
                advanceElmStep(ELM_ATH0);
                return;
            }

            if (now - elmTimer > 2200) {
                retryElmStepOrSequence("ATL0 Timeout");
            }

            break;

        case ELM_ATH0:
            if (elmTimer == 0) {
                if (!sendOBDCommand("ATH0")) {
                    retryElmStepOrSequence("ATH0 Write");
                    return;
                }

                elmTimer = now;
                return;
            }

            if (rxLineReady) {
                clearRxBuffer();
                connectStep("ATH0 OK (Header aus)");
                advanceElmStep(ELM_ATSP0);
                return;
            }

            if (now - elmTimer > 2200) {
                retryElmStepOrSequence("ATH0 Timeout");
            }

            break;

        case ELM_ATSP0:
            if (elmTimer == 0) {
                if (!sendOBDCommand("ATSP0")) {
                    retryElmStepOrSequence("ATSP0 Write");
                    return;
                }

                elmTimer = now;
                return;
            }

            if (rxLineReady) {
                char response[RX_BUFFER_SIZE];

                if (getRxLine(response, sizeof(response))) {
                    connectStep("Protokoll: %s", response);

                    elmState = ELM_READY;
                    elmInitialized = true;
                    elmInitAttempt = 0;
                    elmStepRetry = 0;
                    resetSchedulerHealth();
                    invalidateAllPidFreshness();
                    resetPidRuntimeHealth();
                    resetPIDTimings();
                    pidSupportRangeIndex = 0;
                    pidSupportQueryState = PID_SUPPORT_PENDING;
                    vinQueryState = VIN_IDLE;

                    connectStep("ELM327 bereit - pruefe PID-Support");

                    connectOverlayCloseAt = millis();
                }

                return;
            }

            if (now - elmTimer > 3000) {
                retryElmStepOrSequence("ATSP0 Timeout");
            }

            break;

        case ELM_READY:
            break;

        case ELM_FAILED:
            elmInitialized = false;

            connectStep("ELM327-Initialisierung fehlgeschlagen - trenne Verbindung");

            disconnectOBD();

            elmState = ELM_IDLE;

            break;

        default:
            break;
    }
}

bool parsePIDResponse(
    const char* response,
    uint8_t pid,
    uint8_t& A,
    uint8_t& B
) {
    char clean[RX_BUFFER_SIZE];
    size_t j = 0;

    for (size_t i = 0; response[i] != '\0' && j < sizeof(clean) - 1; i++) {
        char c = response[i];

        if (c == ' ' || c == '\r' || c == '\n' || c == ':') {
            continue;
        }

        clean[j++] = toupper((unsigned char)c);
    }
    clean[j] = '\0';

    char target[8];
    snprintf(target, sizeof(target), "41%02X", pid);

    char* pos = strstr(clean, target);
    if (!pos) return false;

    pos += 4;
    if (strlen(pos) < 2) return false;

    char hexA[3];
    hexA[0] = pos[0];
    hexA[1] = pos[1];
    hexA[2] = '\0';
    A = strtoul(hexA, nullptr, 16);

    B = 0;

    if (strlen(pos) >= 4) {
        char hexB[3];
        hexB[0] = pos[2];
        hexB[1] = pos[3];
        hexB[2] = '\0';
        B = strtoul(hexB, nullptr, 16);
    }

    return true;
}

bool parseMode22Response(
    const char* response,
    uint16_t pid,
    uint8_t& A,
    uint8_t& B
) {
    char clean[RX_BUFFER_SIZE];
    size_t j = 0;

    for (size_t i = 0; response[i] != '\0' && j < sizeof(clean) - 1; i++) {
        const char c = response[i];
        if (c == ' ' || c == '\r' || c == '\n' || c == ':') continue;
        clean[j++] = toupper((unsigned char)c);
    }
    clean[j] = '\0';

    char target[8];
    snprintf(target, sizeof(target), "62%04X", pid);

    char* pos = strstr(clean, target);
    if (!pos) return false;

    pos += 6;
    if (strlen(pos) < 2) return false;

    char hexByte[3] = { pos[0], pos[1], '\0' };
    A = strtoul(hexByte, nullptr, 16);
    B = 0;

    if (strlen(pos) >= 4) {
        hexByte[0] = pos[2];
        hexByte[1] = pos[3];
        B = strtoul(hexByte, nullptr, 16);
    }

    return true;
}

bool applyPID(
    PIDType type,
    const char* response
) {
    uint8_t A = 0;
    uint8_t B = 0;

    if (type == PID_OIL) {
        if (parsePIDResponse(response, 0x5C, A, B) || parseMode22Response(response, 0x1154, A, B)) {
            data.oil = A - 40.0f;
            data.oilOK = true;
            markPidFresh(type);
            return true;
        }
        return false;
    }

    uint8_t pid = 0;
    switch (type) {
        case PID_RPM:       pid = 0x0C; break;
        case PID_SPEED:     pid = 0x0D; break;
        case PID_LOAD:      pid = 0x04; break;
        case PID_COOLANT:   pid = 0x05; break;
        case PID_OIL:       pid = 0x5C; break;
        case PID_FUEL:      pid = 0x2F; break;
        case PID_MAP:       pid = 0x0B; break;
        case PID_BARO:      pid = 0x33; break;
        case PID_MAF:       pid = 0x10; break;
        case PID_INTAKE:    pid = 0x0F; break;
        case PID_VOLTAGE:   pid = 0x42; break;
        case PID_THROTTLE:  pid = 0x11; break;
        case PID_LAMBDA:    pid = 0x44; break;
        case PID_FUEL_RATE: pid = 0x5E; break;
    }

    if (!parsePIDResponse(response, pid, A, B)) return false;

    switch (type) {
        case PID_RPM:       data.rpm = ((A * 256.0f) + B) / 4.0f; data.rpmOK = true; break;
        case PID_SPEED:     data.speed = A; data.speedOK = true; break;
        case PID_LOAD:      data.load = A * 100.0f / 255.0f; data.loadOK = true; break;
        case PID_COOLANT:   data.coolant = A - 40.0f; data.coolantOK = true; break;
        case PID_OIL:       data.oil = A - 40.0f; data.oilOK = true; break;
        case PID_FUEL:      data.fuel = A * 100.0f / 255.0f; data.fuelOK = true; break;
        case PID_MAP:       data.map = A; data.mapOK = true; break;
        case PID_BARO:      data.baro = A; data.baroOK = true; break;
        case PID_MAF:       data.maf = ((A * 256.0f) + B) / 100.0f; data.mafOK = true; break;
        case PID_INTAKE:    data.intake = A - 40.0f; data.intakeOK = true; break;
        case PID_VOLTAGE:   data.voltage = ((A * 256.0f) + B) / 1000.0f; data.voltageOK = true; break;
        case PID_THROTTLE:  data.throttle = A * 100.0f / 255.0f; data.throttleOK = true; break;
        case PID_LAMBDA:    data.lambda = ((A * 256.0f) + B) / 32768.0f; data.lambdaOK = true; break;
        case PID_FUEL_RATE: data.fuelRate = ((A * 256.0f) + B) / 20.0f; data.fuelRateOK = true; break;
    }

    markPidFresh(type);
    return true;
}

static uint8_t standardPidForType(PIDType type) {
    switch (type) {
        case PID_RPM:       return 0x0C;
        case PID_SPEED:     return 0x0D;
        case PID_LOAD:      return 0x04;
        case PID_COOLANT:   return 0x05;
        case PID_OIL:       return 0x5C;
        case PID_FUEL:      return 0x2F;
        case PID_MAP:       return 0x0B;
        case PID_BARO:      return 0x33;
        case PID_MAF:       return 0x10;
        case PID_INTAKE:    return 0x0F;
        case PID_VOLTAGE:   return 0x42;
        case PID_THROTTLE:  return 0x11;
        case PID_LAMBDA:    return 0x44;
        case PID_FUEL_RATE: return 0x5E;
        default:            return 0;
    }
}

static bool vehicleAllowsEnhancedOil1154() {

    if (vehicleProfileSetting == VEH_ASTRA_OPC) return true;
    if (vehicleProfileSetting == VEH_AUTO && vehicleDetectedIsAstraJ) return true;
    return false;
}

static const char* commandForPidIndex(uint8_t index) {
    if (index >= PID_COUNT) return nullptr;
    PIDType type = pidTable[index].type;

    if (type == PID_OIL) {

        if (isStandardPidSupportKnown(0x5C)) {
            if (isStandardPidSupported(0x5C)) return "015C";
            return vehicleAllowsEnhancedOil1154() ? "221154" : nullptr;
        }

        return vehicleAllowsEnhancedOil1154() ? "221154" : "015C";
    }

    const uint8_t pid = standardPidForType(type);

    (void)pid;
    return pidTable[index].command;
}

int findNextDuePID(unsigned long now) {
    int bestIndex = -1;
    int32_t bestOverdue = INT32_MIN;

    for (uint8_t i = 0; i < PID_COUNT; i++) {
        if (!pidEligibleForPoll(i, now)) continue;

        if (!commandForPidIndex(i)) {
            pidRuntimeHealth[i] = PID_HEALTH_UNAVAILABLE;
            pidRetryAfter[i] = now + PID_UNAVAILABLE_RETRY_MS;
            continue;
        }

        const uint32_t interval = effectivePidIntervalMs(i);
        int32_t overdue =
            (pidTable[i].lastPollAt == 0)
                ? (INT32_MAX - i)
                : (int32_t)((uint32_t)(now - pidTable[i].lastPollAt) - interval);

        if (overdue >= 0 && overdue < INT32_MAX / 2) {
            const uint8_t priority = pidPriority(pidTable[i].type);
            overdue += priority == 0 ? 80 : priority == 1 ? 30 : 0;
        }

        if (overdue >= 0 && overdue > bestOverdue) {
            bestOverdue = overdue;
            bestIndex = i;
        }
    }

    return bestIndex;
}

void resetPIDTimings() {
    for (uint8_t i = 0; i < PID_COUNT; i++) {
        pidTable[i].lastPollAt = 0;
    }
}

static bool isPidUnsupportedLine(const char* line) {
    if (!line) return false;
    return strstr(line, "NO DATA") || strcmp(line, "?") == 0 ||
           strstr(line, "7F 01 11") || strstr(line, "7F0111") ||
           strstr(line, "7F 01 12") || strstr(line, "7F0112") ||
           strstr(line, "7F 22 12") || strstr(line, "7F2212") ||
           strstr(line, "7F 22 31") || strstr(line, "7F2231");
}

static bool isTerminalObdErrorLine(const char* line) {
    if (!line) return false;
    return isPidUnsupportedLine(line) || strstr(line, "STOPPED") || strstr(line, "UNABLE") ||
           strstr(line, "ERROR") || strstr(line, "BUS ERROR") || strstr(line, "CAN ERROR");
}

static uint32_t currentPidTimeoutMs() {
    if (currentPID >= PID_COUNT) return PID_TIMEOUT_MS;
    const uint8_t priority = pidPriority(pidTable[currentPID].type);

    const uint32_t base = priority == 0 ? 260UL : priority == 1 ? 340UL : 500UL;
    return base + schedulerPressure * 60UL;
}

void processOBD() {
    if (!obdConnected || !elmInitialized) { return; }

    unsigned long now = millis();

    switch (obdState) {
        case OBD_IDLE: {

            if ((int32_t)(now - obdStateTime) < 0) {
                return;
            }

            int nextPID = findNextDuePID(now);

            if (nextPID >= 0) {
                currentPID = (uint8_t)nextPID;
                obdState = OBD_SEND;
            }

            break;
        }

        case OBD_SEND: {
            clearRxBuffer();
            const char* command = commandForPidIndex(currentPID);
            if (!command) {
                obdState = OBD_NEXT;
                break;
            }

            if (!sendOBDCommand(command)) {
                disconnectOBD();

                obdState = OBD_IDLE;
                obdCycleActive = false;

                return;
            }

            pidTable[currentPID].lastPollAt = now;
            pidRequestWindow++;

            pidSentAt = now;
            obdState = OBD_WAIT;
            obdStateTime = now;

            break;
        }

        case OBD_WAIT:
            if (rxLineReady) {
                if (getRxLine(currentResponse, sizeof(currentResponse))) {
                    const bool parsed = applyPID(pidTable[currentPID].type, currentResponse);
                    if (parsed) {
                        const uint16_t latency = (uint16_t)min(65535UL, now - pidSentAt);
                        schedulerRecordSuccess(latency);
                        obdState = OBD_NEXT;
                    } else if (isTerminalObdErrorLine(currentResponse)) {
                        schedulerRecordInvalid();
                        if (isPidUnsupportedLine(currentResponse)) {
                            recordPidNoData(currentPID, now);
                        } else {
                            recordPidTimeoutBackoff(currentPID, now);
                        }
                        obdState = OBD_NEXT;
                    }

                }

                return;
            }

            if (now - obdStateTime >= currentPidTimeoutMs()) {
                schedulerRecordTimeout();
                recordPidTimeoutBackoff(currentPID, now);
                obdState = OBD_NEXT;
            }

            break;

        case OBD_NEXT:

            obdCycleActive = false;
            obdState = OBD_IDLE;
            obdStateTime = now + adaptivePidGapMs();

            break;
    }
}

static void serviceObdDataHealth(uint32_t now) {
    if (!obdConnected || !elmInitialized || vinQueryState != VIN_DONE) {
        obdPollingStartedAt = 0;
        return;
    }

    if (obdPollingStartedAt == 0) {
        obdPollingStartedAt = now;
        return;
    }

    if (obdPartialRecoveryAttempted || now - obdPollingStartedAt < 25000UL) return;

    uint8_t distinctValues = 0;
    for (uint8_t i = 0; i < PID_COUNT; i++) {
        if (pidLastSuccessAt[i] != 0) distinctValues++;
    }

    if (distinctValues >= 4) return;

    obdPartialRecoveryAttempted = true;
    appLog("[OBD] Only %u PID values after 25s - restarting ELM initialization", (unsigned)distinctValues);
    connectStep("Nur %u Werte - ELM wird automatisch neu initialisiert", (unsigned)distinctValues);
    startELMInitialization();
}

float getBoostBar() {
    if (!data.mapOK) return NAN;

    float baro = DEFAULT_BARO_KPA;
    if (data.baroOK && data.baro > 70 && data.baro < 110) { baro = data.baro; }

    return (data.map - baro) / 100.0f;
}

float getInstantConsumption() {
    if (!data.speedOK || data.speed < 2) { return NAN; }

    if (data.fuelRateOK && data.fuelRate >= 0 && data.fuelRate < 200) {
        return data.fuelRate * 100.0f / data.speed;
    }

    if (data.mafOK && data.maf >= 0) {
        float fuelKgPerSecond = (data.maf / STOICH_AFR) / 1000.0f;
        float fuelLitersPerHour = fuelKgPerSecond * 3600.0f / FUEL_DENSITY_KG_L;
        return fuelLitersPerHour * 100.0f / data.speed;
    }

    return NAN;
}

float getEstimatedPowerPS() {
    if (!data.rpmOK || !data.loadOK) return NAN;

    float loadFactor = data.load / 100.0f;
    return (data.rpm * loadFactor) / 40.0f;
}

bool getMetricValue(MetricId id, float& value) {
    switch (id) {
        case METRIC_SPEED:
            if (!data.speedOK) return false;
            value = data.speed;
            return true;

        case METRIC_RPM:
            if (!data.rpmOK) return false;
            value = data.rpm;
            return true;

        case METRIC_BOOST: {
            float b = getBoostBar();
            if (isnan(b)) return false;
            value = b;
            return true;
        }

        case METRIC_LOAD:
            if (!data.loadOK) return false;
            value = data.load;
            return true;

        case METRIC_COOLANT:
            if (!data.coolantOK) return false;
            value = data.coolant;
            return true;

        case METRIC_OIL:
            if (!data.oilOK) return false;
            value = data.oil;
            return true;

        case METRIC_INTAKE:
            if (!data.intakeOK) return false;
            value = data.intake;
            return true;

        case METRIC_FUEL_LEVEL:
            if (!data.fuelOK) return false;
            value = data.fuel;
            return true;

        case METRIC_VOLTAGE:
            if (!data.voltageOK) return false;
            value = data.voltage;
            return true;

        case METRIC_THROTTLE:
            if (!data.throttleOK) return false;
            value = data.throttle;
            return true;

        case METRIC_LAMBDA:
            if (!data.lambdaOK) return false;
            value = data.lambda;
            return true;

        case METRIC_MAF:
            if (!data.mafOK) return false;
            value = data.maf;
            return true;

        case METRIC_POWER: {
            float p = getEstimatedPowerPS();
            if (isnan(p)) return false;
            value = p;
            return true;
        }

        case METRIC_CONSUMPTION: {
            float c = getInstantConsumption();
            if (isnan(c)) return false;
            value = c;
            return true;
        }

        default:
            return false;
    }
}

static ValueFreshness combineFreshness(ValueFreshness a, ValueFreshness b) {
    return (ValueFreshness)max((uint8_t)a, (uint8_t)b);
}

ValueFreshness getMetricFreshness(MetricId id) {
    switch (id) {
        case METRIC_SPEED:       return pidFreshness(PID_SPEED);
        case METRIC_RPM:         return pidFreshness(PID_RPM);
        case METRIC_BOOST:       return combineFreshness(pidFreshness(PID_MAP), data.baroOK ? pidFreshness(PID_BARO) : VALUE_FRESH);
        case METRIC_LOAD:        return pidFreshness(PID_LOAD);
        case METRIC_COOLANT:     return pidFreshness(PID_COOLANT);
        case METRIC_OIL:         return pidFreshness(PID_OIL);
        case METRIC_INTAKE:      return pidFreshness(PID_INTAKE);
        case METRIC_FUEL_LEVEL:  return pidFreshness(PID_FUEL);
        case METRIC_VOLTAGE:     return pidFreshness(PID_VOLTAGE);
        case METRIC_THROTTLE:    return pidFreshness(PID_THROTTLE);
        case METRIC_LAMBDA:      return pidFreshness(PID_LAMBDA);
        case METRIC_MAF:         return pidFreshness(PID_MAF);
        case METRIC_POWER:       return combineFreshness(pidFreshness(PID_RPM), pidFreshness(PID_LOAD));
        case METRIC_CONSUMPTION: {
            ValueFreshness f = pidFreshness(PID_SPEED);
            if (data.fuelRateOK) f = combineFreshness(f, pidFreshness(PID_FUEL_RATE));
            else f = combineFreshness(f, pidFreshness(PID_MAF));
            return f;
        }
        default: return VALUE_EXPIRED;
    }
}

static bool metricUnavailable(MetricId id) {
    switch (id) {
        case METRIC_SPEED:       return pidTypeUnavailable(PID_SPEED);
        case METRIC_RPM:         return pidTypeUnavailable(PID_RPM);
        case METRIC_BOOST:       return pidTypeUnavailable(PID_MAP);
        case METRIC_LOAD:        return pidTypeUnavailable(PID_LOAD);
        case METRIC_COOLANT:     return pidTypeUnavailable(PID_COOLANT);
        case METRIC_OIL:         return pidTypeUnavailable(PID_OIL);
        case METRIC_INTAKE:      return pidTypeUnavailable(PID_INTAKE);
        case METRIC_FUEL_LEVEL:  return pidTypeUnavailable(PID_FUEL);
        case METRIC_VOLTAGE:     return pidTypeUnavailable(PID_VOLTAGE);
        case METRIC_THROTTLE:    return pidTypeUnavailable(PID_THROTTLE);
        case METRIC_LAMBDA:      return pidTypeUnavailable(PID_LAMBDA);
        case METRIC_MAF:         return pidTypeUnavailable(PID_MAF);
        case METRIC_POWER:
            return pidTypeUnavailable(PID_RPM) || pidTypeUnavailable(PID_LOAD);
        case METRIC_CONSUMPTION:
            return pidTypeUnavailable(PID_SPEED) ||
                   (pidTypeUnavailable(PID_FUEL_RATE) && pidTypeUnavailable(PID_MAF));
        default:
            return false;
    }
}

void formatMetricNumber(MetricId id, float v, char* buf, size_t bufSize) {
    switch (id) {
        case METRIC_SPEED:       snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_RPM:         snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_BOOST:       snprintf(buf, bufSize, "%+.2f", v); break;
        case METRIC_LOAD:        snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_COOLANT:     snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_OIL:         snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_INTAKE:      snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_FUEL_LEVEL:  snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_VOLTAGE:     snprintf(buf, bufSize, "%.1f", v); break;
        case METRIC_THROTTLE:    snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_LAMBDA:      snprintf(buf, bufSize, "%.2f", v); break;
        case METRIC_MAF:         snprintf(buf, bufSize, "%.1f", v); break;
        case METRIC_POWER:       snprintf(buf, bufSize, "%.0f", v); break;
        case METRIC_CONSUMPTION: snprintf(buf, bufSize, "%.1f", v); break;
        default:                 snprintf(buf, bufSize, "--"); break;
    }
}

const char* metricUnit(MetricId id) {
    switch (id) {
        case METRIC_SPEED:       return "KM/H";
        case METRIC_RPM:         return "RPM";
        case METRIC_BOOST:       return "BAR";
        case METRIC_LOAD:        return "%";
        case METRIC_COOLANT:     return "C";
        case METRIC_OIL:         return "C";
        case METRIC_INTAKE:      return "C";
        case METRIC_FUEL_LEVEL:  return "%";
        case METRIC_VOLTAGE:     return "V";
        case METRIC_THROTTLE:    return "%";
        case METRIC_LAMBDA:      return "";
        case METRIC_MAF:         return "G/S";
        case METRIC_POWER:       return "PS";
        case METRIC_CONSUMPTION: return "L/100KM";
        default:                 return "";
    }
}

const char* metricDisplayName(MetricId id) {
    static const char* de[METRIC_COUNT] = {
        "GESCHWINDIGKEIT", "DREHZAHL", "LADEDRUCK", "MOTORLAST", "KUEHLWASSER",
        "OELTEMP", "ANSAUGLUFT", "TANKINHALT", "SPANNUNG", "DROSSELKLAPPE",
        "LAMBDA", "LUFTMASSE", "LEISTUNG (EST)", "VERBRAUCH"
    };
    return uiLanguage == 0 ? de[id] : metricDefs[id].name;
}

void loadSettings() {
    prefs.begin("astra", true);
    const uint16_t storedSchema = prefs.getUShort("schema", 0);

    for (uint8_t i = 0; i < DASHBOARD_SLOT_COUNT; i++) {
        char key[8];
        snprintf(key, sizeof(key), "dsl%d", i);
        uint8_t savedMetric = prefs.getUChar(key, (uint8_t)dashboardSlotMetric[i]);
        if (savedMetric >= METRIC_COUNT) savedMetric = (uint8_t)dashboardSlotMetric[i];
        dashboardSlotMetric[i] = (MetricId)savedMetric;
        char hideKey[8];
        snprintf(hideKey, sizeof(hideKey), "dsh%d", i);
        dashboardSlotHidden[i] = prefs.getBool(hideKey, false);

        char hzKey[8];
        snprintf(hzKey, sizeof(hzKey), "dshz%d", i);
        uint8_t hz = prefs.getUChar(hzKey, dashboardSlotRefreshHz[i]);
        if (hz != 1 && hz != 2 && hz != 5 && hz != 10 && hz != 20) hz = dashboardSlotRefreshHz[i];
        dashboardSlotRefreshHz[i] = hz;
    }

    {
        const uint8_t gaugeHz = prefs.getUChar("rpmhz", rpmGaugeRefreshHz);
        if (gaugeHz == 1 || gaugeHz == 2 || gaugeHz == 5 || gaugeHz == 10 || gaugeHz == 20) rpmGaugeRefreshHz = gaugeHz;
        for (uint8_t i = 0; i < 3; i++) {
            char centerKey[8];
            snprintf(centerKey, sizeof(centerKey), "cmh%d", i);
            centerMetricHidden[i] = prefs.getBool(centerKey, false);
        }
    }

    dayBrightness = constrain((int)prefs.getUChar("brightDay", 100), 10, 100);
    nightBrightness = constrain((int)prefs.getUChar("brightNight", 35), 5, 80);
    const uint8_t savedLanguage = prefs.getUChar("language", 0);
    uiLanguage = savedLanguage <= 1 ? savedLanguage : 0;
    automaticLightMode = prefs.getBool("autoLight", true);
    manualNightMode = prefs.getBool("manualNight", false);
    displayRotated = prefs.getBool("rotate180", false);
    sdLoggingEnabled = prefs.getBool("sdLogging", false);

    uint8_t vp = prefs.getUChar("vehicle", (uint8_t)VEH_ASTRA_OPC);
    if (vp >= VEH_PROFILE_COUNT) vp = VEH_ASTRA_OPC;
    vehicleProfileSetting = (VehicleProfile)vp;

    prefs.end();
    applySlotRefreshRatesToPidTable();

    if (storedSchema < CONFIG_SCHEMA_VERSION) {
        prefs.begin("astra", false);
        prefs.remove("vin");
        prefs.putUShort("schema", CONFIG_SCHEMA_VERSION);
        prefs.end();
    }
}

void saveDisplaySettings() {
    prefs.begin("astra", false);
    prefs.putUChar("brightDay", dayBrightness);
    prefs.putUChar("brightNight", nightBrightness);
    prefs.putUChar("language", uiLanguage);
    prefs.putBool("autoLight", automaticLightMode);
    prefs.putBool("manualNight", manualNightMode);
    prefs.putBool("rotate180", displayRotated);
    prefs.putUChar("vehicle", (uint8_t)vehicleProfileSetting);
    prefs.putBool("sdLogging", sdLoggingEnabled);
    prefs.putUShort("schema", CONFIG_SCHEMA_VERSION);
    prefs.end();
}

void applyDisplayRotation() {
    lv_disp_t* display = lv_disp_get_default();
    if (!display) return;
    lv_disp_set_rotation(display, displayRotated ? LV_DISP_ROT_180 : LV_DISP_ROT_NONE);
}

void applyDisplayBrightness() {
    if (!displayBoard || !displayBoard->getBacklight()) return;

    const bool useNight = automaticLightMode ? vehicleLightsOn : manualNightMode;
    const uint8_t level = useNight ? nightBrightness : dayBrightness;

    displayBoard->getBacklight()->setBrightness(100);

    if (brightnessDimmer) {
        const uint8_t dimOpacity = (uint8_t)(((100 - min((int)level, 100)) * 255) / 100);
        lv_obj_set_style_bg_opa(brightnessDimmer, dimOpacity, 0);

        if (dimOpacity == 0) lv_obj_add_flag(brightnessDimmer, LV_OBJ_FLAG_HIDDEN);
        else lv_obj_clear_flag(brightnessDimmer, LV_OBJ_FLAG_HIDDEN);

        lv_obj_move_foreground(brightnessDimmer);
    }
}

void createBrightnessDimmer() {
    brightnessDimmer = lv_obj_create(lv_layer_sys());
    lv_obj_set_size(brightnessDimmer, 1024, 600);
    lv_obj_set_pos(brightnessDimmer, 0, 0);
    lv_obj_set_style_bg_color(brightnessDimmer, lv_color_black(), 0);
    lv_obj_set_style_border_width(brightnessDimmer, 0, 0);
    lv_obj_set_style_radius(brightnessDimmer, 0, 0);
    lv_obj_set_style_pad_all(brightnessDimmer, 0, 0);
    lv_obj_clear_flag(brightnessDimmer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(brightnessDimmer, LV_OBJ_FLAG_SCROLLABLE);
}

void saveDashboardSlots() {
    prefs.begin("astra", false);

    for (uint8_t i = 0; i < DASHBOARD_SLOT_COUNT; i++) {
        char key[8];
        snprintf(key, sizeof(key), "dsl%d", i);
        prefs.putUChar(key, (uint8_t)dashboardSlotMetric[i]);
        char hideKey[8];
        snprintf(hideKey, sizeof(hideKey), "dsh%d", i);
        prefs.putBool(hideKey, dashboardSlotHidden[i]);

        char hzKey[8];
        snprintf(hzKey, sizeof(hzKey), "dshz%d", i);
        prefs.putUChar(hzKey, dashboardSlotRefreshHz[i]);
    }

    prefs.putUChar("rpmhz", rpmGaugeRefreshHz);
    for (uint8_t i = 0; i < 3; i++) {
        char centerKey[8];
        snprintf(centerKey, sizeof(centerKey), "cmh%d", i);
        prefs.putBool(centerKey, centerMetricHidden[i]);
    }

    prefs.end();
}

static void requestDisplaySettingsPersist() {
    displaySettingsPersistDirty = true;
    displaySettingsPersistDueAt = millis() + SETTINGS_PERSIST_DEBOUNCE_MS;
}

static void requestDashboardPersist() {
    dashboardPersistDirty = true;
    dashboardPersistDueAt = millis() + SETTINGS_PERSIST_DEBOUNCE_MS;
}

static void serviceDeferredPersistence() {
    const uint32_t now = millis();
    const bool displayDue = displaySettingsPersistDirty && (int32_t)(now - displaySettingsPersistDueAt) >= 0;
    const bool dashboardDue = dashboardPersistDirty && (int32_t)(now - dashboardPersistDueAt) >= 0;
    if (!displayDue && !dashboardDue) return;

    if (otaModeActive || otaStartRequested || diagnosticModeActive || bleScanInProgress) return;
    if (obdConnected && (!elmInitialized || pidSupportQueryState != PID_SUPPORT_DONE || vinQueryState != VIN_DONE || obdState != OBD_IDLE)) return;

    if (displayDue) {

        displaySettingsPersistDirty = false;
        saveDisplaySettings();
    }
    if (dashboardDue) {
        dashboardPersistDirty = false;
        saveDashboardSlots();
    }
}

lv_obj_t* createPanel(lv_obj_t* parent, int x, int y, int w, int h) {
    lv_obj_t* p = lv_obj_create(parent);
    lv_obj_set_size(p, w, h);
    lv_obj_set_pos(p, x, y);

    lv_obj_set_style_bg_color(p, COL_PANEL, 0);
    lv_obj_set_style_bg_grad_color(p, COL_PANEL2, 0);
    lv_obj_set_style_bg_grad_dir(p, LV_GRAD_DIR_VER, 0);

    lv_obj_set_style_border_width(p, 1, 0);
    lv_obj_set_style_border_color(p, lv_color_hex(0x303743), 0);
    lv_obj_set_style_radius(p, 12, 0);

    lv_obj_set_style_shadow_width(p, 8, 0);
    lv_obj_set_style_shadow_opa(p, LV_OPA_20, 0);
    lv_obj_set_style_shadow_color(p, lv_color_black(), 0);

    lv_obj_set_style_pad_left(p, 14, 0);
    lv_obj_set_style_pad_right(p, 14, 0);
    lv_obj_set_style_pad_top(p, 9, 0);
    lv_obj_set_style_pad_bottom(p, 8, 0);

    lv_obj_clear_flag(p, LV_OBJ_FLAG_SCROLLABLE);

    return p;
}

lv_obj_t* createValueLabel(lv_obj_t* parent, const char* value, lv_color_t color) {
    lv_obj_t* l = lv_label_create(parent);
    lv_label_set_text(l, value);
    lv_obj_set_style_text_font(l, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(l, COL_WHITE, 0);
    lv_obj_set_style_text_letter_space(l, 0, 0);
    lv_obj_align(l, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    return l;
}

lv_obj_t* createPanelTitle(lv_obj_t* parent, const char* title) {
    lv_obj_t* l = lv_label_create(parent);
    lv_label_set_text(l, title);
    lv_obj_set_style_text_font(l, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(l, COL_GREY, 0);
    lv_obj_align(l, LV_ALIGN_TOP_LEFT, 0, 0);
    return l;
}

lv_obj_t* createPanelUnit(lv_obj_t* parent, const char* unit) {
    lv_obj_t* l = lv_label_create(parent);
    lv_label_set_text(l, unit);
    lv_obj_set_style_text_font(l, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(l, COL_GREY, 0);
    lv_obj_align(l, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    return l;
}

void addAccentBar(lv_obj_t* screen) {
    lv_obj_t* bar = lv_obj_create(screen);
    lv_obj_set_size(bar, 1024, 3);
    lv_obj_set_pos(bar, 0, 0);

    lv_obj_set_style_bg_color(bar, COL_BRAND_PURPLE, 0);
    lv_obj_set_style_bg_grad_color(bar, COL_BRAND_BLUE, 0);
    lv_obj_set_style_bg_grad_dir(bar, LV_GRAD_DIR_HOR, 0);

    lv_obj_set_style_border_width(bar, 0, 0);
    lv_obj_set_style_radius(bar, 0, 0);

    lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(bar, LV_OBJ_FLAG_CLICKABLE);
}

void setScreenBackground(lv_obj_t* screen) {
    lv_obj_set_style_bg_color(screen, COL_BRAND_DARK, 0);
    lv_obj_set_style_bg_grad_color(screen, COL_BRAND_DARK2, 0);
    lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, 0);

    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    addAccentBar(screen);
}

lv_point_t rpmTickPoints[31][2];

void createRpmTick(lv_obj_t* screen, int index, float rpm) {
    const float angleDeg = 150.0f + (240.0f * rpm / RPM_MAX);
    const float angle = angleDeg * PI / 180.0f;

    const bool major = fmodf(rpm, 1000.0f) < 1.0f;
    const float innerScale = major ? 0.88f : 0.935f;

    rpmTickPoints[index][0].x = (lv_coord_t)(RPM_CX + cosf(angle) * RPM_RX * innerScale);
    rpmTickPoints[index][0].y = (lv_coord_t)(RPM_CY + sinf(angle) * RPM_RY * innerScale);
    rpmTickPoints[index][1].x = (lv_coord_t)(RPM_CX + cosf(angle) * RPM_RX);
    rpmTickPoints[index][1].y = (lv_coord_t)(RPM_CY + sinf(angle) * RPM_RY);

    rpmTickLine[index] = lv_line_create(screen);
    lv_line_set_points(rpmTickLine[index], rpmTickPoints[index], 2);

    lv_obj_set_style_line_width(rpmTickLine[index], major ? 4 : 2, 0);

    lv_obj_set_style_line_color(
        rpmTickLine[index],
        rpm >= 6500 ? COL_RED : rpm >= 5500 ? COL_ORANGE : rpm >= 2500 ? COL_GREEN : COL_BLUE,
        0
    );

    lv_obj_clear_flag(rpmTickLine[index], LV_OBJ_FLAG_CLICKABLE);
}

void onRpmGaugeLongPress(lv_event_t* event);

void createRpmGauge() {

    for (uint8_t i = 0; i <= 60; i++) {
        const float a = (150.0f + 240.0f * i / 60.0f) * PI / 180.0f;
        rpmBasePoints[i].x = (lv_coord_t)(RPM_CX + cosf(a) * RPM_RX);
        rpmBasePoints[i].y = (lv_coord_t)(RPM_CY + sinf(a) * RPM_RY);
    }

    rpmBaseLine = lv_line_create(dashboardScreen);
    lv_line_set_points(rpmBaseLine, rpmBasePoints, 61);
    lv_obj_set_style_line_width(rpmBaseLine, 13, 0);
    lv_obj_set_style_line_color(rpmBaseLine, lv_color_hex(0x252B35), 0);
    lv_obj_set_style_line_rounded(rpmBaseLine, true, 0);
    lv_obj_clear_flag(rpmBaseLine, LV_OBJ_FLAG_CLICKABLE);

    const uint16_t zoneStart[4] = { 0, 20, 44, 52 };
    const uint16_t zoneEnd[4] = { 20, 44, 52, 60 };
    const lv_color_t zoneColor[4] = { COL_BLUE, COL_GREEN, COL_ORANGE, COL_RED };

    for (uint8_t i = 0; i < 4; i++) {
        const uint8_t count = zoneEnd[i] - zoneStart[i] + 1;
        for (uint8_t p = 0; p < count; p++) rpmZonePoints[i][p] = rpmBasePoints[zoneStart[i] + p];

        rpmZoneLine[i] = lv_line_create(dashboardScreen);
        lv_line_set_points(rpmZoneLine[i], rpmZonePoints[i], count);
        lv_obj_set_style_line_width(rpmZoneLine[i], 10, 0);
        lv_obj_set_style_line_color(rpmZoneLine[i], zoneColor[i], 0);
        lv_obj_set_style_line_rounded(rpmZoneLine[i], true, 0);
        lv_obj_clear_flag(rpmZoneLine[i], LV_OBJ_FLAG_CLICKABLE);
    }

    for (uint8_t i = 0; i <= 30; i++) {
        createRpmTick(dashboardScreen, i, (float)i * 250.0f);
    }

    const float labelScale = 0.76f;

    for (uint8_t i = 0; i < 8; i++) {
        float rpm = i * 1000.0f;
        float angleDeg = 150.0f + (240.0f * rpm / RPM_MAX);
        float angle = angleDeg * PI / 180.0f;

        int x = (int)(RPM_CX + cosf(angle) * RPM_RX * labelScale);
        int y = (int)(RPM_CY + sinf(angle) * RPM_RY * labelScale);

        rpmTickLabel[i] = lv_label_create(dashboardScreen);

        char buf[8];
        snprintf(buf, sizeof(buf), "%d", i);
        lv_label_set_text(rpmTickLabel[i], buf);

        lv_obj_set_style_text_font(rpmTickLabel[i], &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(rpmTickLabel[i], COL_WHITE, 0);

        lv_obj_set_pos(rpmTickLabel[i], x - 7, y - 10);
    }

    lv_obj_t* rpmScaleText = lv_label_create(dashboardScreen);
    lv_label_set_text(rpmScaleText, "RPM x 1000");
    lv_obj_set_style_text_font(rpmScaleText, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(rpmScaleText, COL_GREY, 0);
    lv_obj_align(rpmScaleText, LV_ALIGN_CENTER, 0, -22);

    rpmLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(rpmLabel, "0");
    lv_obj_set_style_text_font(rpmLabel, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(rpmLabel, COL_WHITE, 0);
    lv_obj_align(rpmLabel, LV_ALIGN_CENTER, 0, 89);

    rpmUnitLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(rpmUnitLabel, "RPM");
    lv_obj_set_style_text_font(rpmUnitLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(rpmUnitLabel, COL_BRAND_PURPLE, 0);
    lv_obj_align(rpmUnitLabel, LV_ALIGN_CENTER, 0, 120);

    rpmNeedlePoints[0] = { RPM_CX, RPM_CY };
    rpmNeedlePoints[1] = { (lv_coord_t)(RPM_CX - RPM_RX * 0.68f), (lv_coord_t)(RPM_CY + RPM_RY * 0.34f) };

    rpmMeter = lv_line_create(dashboardScreen);
    lv_line_set_points(rpmMeter, rpmNeedlePoints, 2);
    lv_obj_set_style_line_width(rpmMeter, 5, 0);
    lv_obj_set_style_line_color(rpmMeter, COL_WHITE, 0);
    lv_obj_set_style_line_rounded(rpmMeter, true, 0);
    lv_obj_clear_flag(rpmMeter, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* hub = lv_obj_create(dashboardScreen);
    lv_obj_set_size(hub, 18, 18);
    lv_obj_set_pos(hub, RPM_CX - 9, RPM_CY - 9);
    lv_obj_set_style_radius(hub, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(hub, COL_BRAND_PURPLE, 0);
    lv_obj_set_style_border_width(hub, 3, 0);
    lv_obj_set_style_border_color(hub, COL_WHITE, 0);
    lv_obj_clear_flag(hub, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(hub, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* gaugeHotspot = lv_obj_create(dashboardScreen);
    lv_obj_remove_style_all(gaugeHotspot);
    lv_obj_set_pos(gaugeHotspot, RPM_CX - (int)RPM_RX, RPM_CY - (int)RPM_RY);
    lv_obj_set_size(gaugeHotspot, 2 * (int)RPM_RX, 436 - (RPM_CY - (int)RPM_RY));
    lv_obj_clear_flag(gaugeHotspot, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(gaugeHotspot, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(gaugeHotspot, onRpmGaugeLongPress, LV_EVENT_LONG_PRESSED, nullptr);
}

void createStatusBar() {
    lv_obj_t* bar = lv_obj_create(dashboardScreen);
    lv_obj_set_size(bar, 984, 38);
    lv_obj_set_pos(bar, 20, 555);

    lv_obj_set_style_bg_color(bar, lv_color_hex(0x0B0E14), 0);
    lv_obj_set_style_border_width(bar, 1, 0);
    lv_obj_set_style_border_color(bar, lv_color_hex(0x353B47), 0);
    lv_obj_set_style_radius(bar, 9, 0);
    lv_obj_set_style_pad_all(bar, 0, 0);

    const int itemX[4] = { 18, 264, 510, 756 };
    const char* initial[4] = { "OBD-II OFFLINE", "BLE OFFLINE", "VOLTAGE --.- V", "LATENCY --- ms" };

    lv_obj_t** labels[4] = { &statusOBDLabel, &statusBLELabel, &statusVoltageLabel, &statusLatencyLabel };

    for (uint8_t i = 0; i < 4; i++) {
        if (i > 0) {
            lv_obj_t* sep = lv_obj_create(bar);
            lv_obj_set_size(sep, 1, 20);
            lv_obj_set_pos(sep, itemX[i] - 18, 9);
            lv_obj_set_style_bg_color(sep, lv_color_hex(0x303640), 0);
            lv_obj_set_style_border_width(sep, 0, 0);
        }

        *labels[i] = lv_label_create(bar);
        lv_label_set_text(*labels[i], initial[i]);

        lv_obj_set_style_text_font(*labels[i], &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(*labels[i], COL_GREY, 0);

        lv_obj_set_pos(*labels[i], itemX[i], 9);

        if (i == 0) {

            lv_obj_add_flag(*labels[i], LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_event_cb(*labels[i], onConnectionLabelClicked, LV_EVENT_CLICKED, nullptr);
        }
    }
}

static void otaSetUiStatus(const char* text) {
    if (!text) return;
    strncpy(otaUiStatus, text, sizeof(otaUiStatus) - 1);
    otaUiStatus[sizeof(otaUiStatus) - 1] = '\0';
    otaUiDirty = true;
}

static void onOtaStopClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;

    if ((int32_t)(millis() - otaStopButtonArmedAt) < 0) return;
    if (otaUploadRunning) {
        otaSetUiStatus("Update laeuft - Abbruch gesperrt");
        return;
    }
    otaStopRequested = true;
}

void createOtaOverlay() {
    otaOverlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(otaOverlay, 1024, 600);
    lv_obj_set_pos(otaOverlay, 0, 0);
    lv_obj_set_style_bg_color(otaOverlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(otaOverlay, LV_OPA_90, 0);
    lv_obj_set_style_border_width(otaOverlay, 0, 0);
    lv_obj_clear_flag(otaOverlay, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* panel = lv_obj_create(otaOverlay);
    lv_obj_set_size(panel, 720, 430);
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, COL_PANEL, 0);
    lv_obj_set_style_border_color(panel, COL_BRAND_PURPLE, 0);
    lv_obj_set_style_border_width(panel, 2, 0);
    lv_obj_set_style_radius(panel, 16, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    otaTitleLabel = lv_label_create(panel);
    lv_label_set_text(otaTitleLabel, "SCHIEREN OTA UPDATE");
    lv_obj_set_style_text_font(otaTitleLabel, UI_FONT_OTA_TITLE, 0);
    lv_obj_set_style_text_color(otaTitleLabel, COL_WHITE, 0);
    lv_obj_align(otaTitleLabel, LV_ALIGN_TOP_MID, 0, 22);

    otaInfoLabel = lv_label_create(panel);
    lv_label_set_text(otaInfoLabel, "WLAN wird gestartet ...");
    lv_obj_set_style_text_font(otaInfoLabel, UI_FONT_BODY_16, 0);
    lv_obj_set_style_text_color(otaInfoLabel, COL_GREY, 0);
    lv_obj_set_style_text_align(otaInfoLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(otaInfoLabel, 650);
    lv_obj_align(otaInfoLabel, LV_ALIGN_TOP_MID, 0, 92);

    otaStatusLabel = lv_label_create(panel);
    lv_label_set_text(otaStatusLabel, "Bereit");
    lv_obj_set_style_text_font(otaStatusLabel, UI_FONT_BODY_16, 0);
    lv_obj_set_style_text_color(otaStatusLabel, COL_GREEN, 0);
    lv_obj_set_style_text_align(otaStatusLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(otaStatusLabel, 650);
    lv_obj_align(otaStatusLabel, LV_ALIGN_TOP_MID, 0, 240);

    otaStopButton = lv_btn_create(panel);
    lv_obj_set_size(otaStopButton, 220, 50);
    lv_obj_align(otaStopButton, LV_ALIGN_BOTTOM_MID, 0, -26);
    lv_obj_set_style_bg_color(otaStopButton, COL_PANEL2, 0);
    lv_obj_add_event_cb(otaStopButton, onOtaStopClicked, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* stopLabel = lv_label_create(otaStopButton);
    lv_label_set_text(stopLabel, "OTA BEENDEN");
    lv_obj_center(stopLabel);

    lv_obj_add_flag(otaOverlay, LV_OBJ_FLAG_HIDDEN);
}

static const char OTA_INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html><html lang="de"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>SCHIEREN OBD OTA</title>
<style>body{font-family:system-ui;background:#080a0f;color:#f4f6f8;max-width:680px;margin:36px auto;padding:18px}div{background:#11151d;border:1px solid #8a2be2;border-radius:16px;padding:24px}button,input{font-size:16px;margin-top:16px}button{background:#8a2be2;color:white;border:0;border-radius:8px;padding:12px 18px}progress{width:100%;height:22px;margin-top:18px}</style></head>
<body><div><h2>SCHIEREN OBD Firmware Update</h2><p>Firmware: )HTML" FW_VERSION R"HTML(</p>
<form id="f"><input id="bin" type="file" name="firmware" accept=".bin,application/octet-stream" required><br><button>Firmware installieren</button></form>
<progress id="p" max="100" value="0"></progress><p id="s">Bereit.</p>
<script>document.getElementById('f').onsubmit=function(e){e.preventDefault();const f=document.getElementById('bin').files[0];if(!f)return;const x=new XMLHttpRequest();const d=new FormData();const pb=document.getElementById('p'),st=document.getElementById('s');d.append('firmware',f);x.upload.onprogress=e=>{if(e.lengthComputable){pb.value=Math.round(e.loaded*100/e.total);st.textContent='Upload '+pb.value+' %';}};x.onload=()=>{st.textContent=x.responseText;};x.onerror=()=>{st.textContent='Upload fehlgeschlagen';};x.open('POST','/update');x.send(d);};</script>
<p>Das Display waehrend des Updates nicht ausschalten. Der Wartungsmodus beendet sich nach 10 Minuten automatisch.</p></div></body></html>
)HTML";

static void generateOtaCredentials() {
    const uint32_t r1 = esp_random();
    const uint32_t r2 = esp_random();
    snprintf(otaApSsid, sizeof(otaApSsid), "%s-%04X", OTA_AP_PREFIX, (unsigned)(r1 & 0xFFFF));
    snprintf(otaApPassword, sizeof(otaApPassword), "%08lu", (unsigned long)(10000000UL + (r2 % 90000000UL)));
}

static void configureOtaRoutes() {
    if (otaRoutesConfigured) return;

    otaServer.on("/", HTTP_GET, []() {
        otaLastActivityAt = millis();
        otaServer.sendHeader("Cache-Control", "no-store");
        otaServer.send_P(200, "text/html; charset=utf-8", OTA_INDEX_HTML);
    });

    otaServer.on(
        "/update", HTTP_POST,
        []() {
            const bool ok = !Update.hasError() && otaUploadSucceeded;
            otaServer.sendHeader("Connection", "close");
            otaServer.send(
                ok ? 200 : 500,
                "text/plain; charset=utf-8",
                ok ? "Update erfolgreich. Display startet neu." : "Update fehlgeschlagen. Alte Firmware bleibt aktiv."
            );
            if (ok) {
                otaSetUiStatus("Update erfolgreich - Neustart ...");
                otaRebootPending = true;
                otaRebootAt = millis() + 1500;
            } else {
                otaSetUiStatus("Update fehlgeschlagen - Firmware unveraendert");
            }
            otaUploadRunning = false;
        },
        []() {
            HTTPUpload& upload = otaServer.upload();

            otaLastActivityAt = millis();
            if (upload.status == UPLOAD_FILE_START) {
                otaUploadRunning = true;
                otaUploadSucceeded = false;
                otaBytesWritten = 0;
                otaExpectedBytes = otaServer.hasHeader("Content-Length") ? (size_t)otaServer.header("Content-Length").toInt() : 0;
                otaSetUiStatus("Firmware wird empfangen ...");

                if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) {
                    Update.printError(Serial);
                    otaSetUiStatus("OTA-Partition fehlt / Update.begin fehlgeschlagen");
                }
            } else if (upload.status == UPLOAD_FILE_WRITE) {
                if (professionalWatchdogActive) esp_task_wdt_reset();
                if (!Update.hasError()) {
                    const size_t written = Update.write(upload.buf, upload.currentSize);
                    otaBytesWritten += written;
                    if (written != upload.currentSize) {
                        Update.printError(Serial);
                        otaSetUiStatus("Schreibfehler beim Firmware-Update");
                    } else {
                        char buf[96];
                        snprintf(buf, sizeof(buf), "Empfangen: %u kB", (unsigned)(otaBytesWritten / 1024));
                        otaSetUiStatus(buf);
                    }
                }
            } else if (upload.status == UPLOAD_FILE_END) {
                if (!Update.hasError() && Update.end(true)) {
                    otaUploadSucceeded = true;
                    char buf[96];
                    snprintf(buf, sizeof(buf), "%u kB geschrieben - pruefe Update ...", (unsigned)(otaBytesWritten / 1024));
                    otaSetUiStatus(buf);
                    appLog("[OTA] Firmware written: %u bytes", (unsigned)otaBytesWritten);
                } else {
                    Update.printError(Serial);
                    otaSetUiStatus("Firmware konnte nicht abgeschlossen werden");
                }
            } else if (upload.status == UPLOAD_FILE_ABORTED) {
                Update.abort();
                otaUploadRunning = false;
                otaSetUiStatus("Upload abgebrochen");
            }
        }
    );

    otaServer.onNotFound([]() {
        otaServer.send(404, "text/plain", "Not found");
    });

    static const char* otaHeaderKeys[] = { "Content-Length" };
    otaServer.collectHeaders(otaHeaderKeys, 1);
    otaRoutesConfigured = true;
}

void requestStartOtaMode() {
    if (otaModeActive || otaStartRequested) return;
    otaStopRequested = false;
    otaStopButtonArmedAt = millis() + 2000UL;
    otaStartRequested = true;
    if (metricPickerOverlay) lv_obj_add_flag(metricPickerOverlay, LV_OBJ_FLAG_HIDDEN);
    if (otaOverlay) {
        lv_obj_clear_flag(otaOverlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(otaOverlay);
    }
    if (otaInfoLabel) lv_label_set_text(otaInfoLabel, "WLAN wird gestartet ...");
    if (otaStatusLabel) lv_label_set_text(otaStatusLabel, "OBD wird pausiert");
}

static void startOtaModeFromLoop() {
    otaStartRequested = false;
    if (otaModeActive) return;

    otaModeActive = true;
    otaStopRequested = false;
    otaBleReleased = false;

    appLog("[OTA] Starting exclusive local maintenance AP");
    if (sdLogFileReady) { sdLogFile.flush(); }

    if (bleScanInProgress) {
        BLEDevice::getScan()->stop();
        delay(150);
        bleScanInProgress = false;
    }
    bleRescanRequested = false;
    bleManualConnectRequested = false;
    bleScanComplete = false;
    bleEarlyCandidateFound = false;

    if (bleEarlyCandidateDevice) {
        delete bleEarlyCandidateDevice;
        bleEarlyCandidateDevice = nullptr;
    }
    if (obdConnected || obdClient) disconnectOBD();
    elmState = ELM_IDLE;
    elmInitialized = false;
    clearScannedDevices();
    BLEDevice::getScan()->clearResults();

    BLEDevice::deinit(true);
    otaBleReleased = true;
    delay(250);

    generateOtaCredentials();
    WiFi.mode(WIFI_OFF);
    delay(100);

    bool apOk = false;
    for (uint8_t attempt = 1; attempt <= 3 && !apOk; attempt++) {
        WiFi.mode(WIFI_AP);
        delay(150);
        apOk = WiFi.softAP(otaApSsid, otaApPassword);
        appLog("[OTA] softAP attempt %u/3: %s (heap=%u)",
               (unsigned)attempt, apOk ? "OK" : "FAILED", (unsigned)ESP.getFreeHeap());
        if (!apOk) {
            WiFi.softAPdisconnect(true);
            WiFi.mode(WIFI_OFF);
            delay(250);
        }
    }

    if (!apOk) {
        otaSessionStartedAt = millis();
        otaLastActivityAt = otaSessionStartedAt;
        otaSetUiStatus("WLAN-Start fehlgeschlagen - OTA BEENDEN startet neu");
        appLog("[OTA] softAP start failed after 3 attempts");
        return;
    }

    configureOtaRoutes();
    otaServer.begin();

    const IPAddress ip = WiFi.softAPIP();
    char info[256];
    snprintf(
        info,
        sizeof(info),
        "1. WLAN: %s\n2. Passwort: %s\n3. Browser: http://%u.%u.%u.%u/\n4. Arduino IDE: Sketch > Export compiled Binary",
        otaApSsid,
        otaApPassword,
        ip[0], ip[1], ip[2], ip[3]
    );
    otaSessionStartedAt = millis();
    otaLastActivityAt = otaSessionStartedAt;
    otaSetUiStatus("OTA bereit - .bin im Browser auswaehlen");

    ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
    if (otaInfoLabel) lv_label_set_text(otaInfoLabel, info);
    if (otaStatusLabel) lv_label_set_text(otaStatusLabel, otaUiStatus);
    lvgl_port_unlock();

    appLog("[OTA] AP %s, open http://%s/", otaApSsid, ip.toString().c_str());
}

void stopOtaMode() {
    if (!otaModeActive && !otaStartRequested && !otaStopRequested) return;
    if (otaUploadRunning) return;

    otaStartRequested = false;
    otaStopRequested = false;
    otaServer.stop();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);

    if (otaBleReleased) {

        otaUploadSucceeded = false;
        otaSetUiStatus("OTA beendet - System startet neu ...");
        otaRebootPending = true;
        otaRebootAt = millis() + 800UL;
        appLog("[OTA] Maintenance mode stopped - rebooting to restore BLE");
        return;
    }

    otaModeActive = false;
    otaUploadSucceeded = false;
    otaRebootPending = false;
    otaSetUiStatus("Bereit");

    ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
    if (otaOverlay) lv_obj_add_flag(otaOverlay, LV_OBJ_FLAG_HIDDEN);
    lvgl_port_unlock();

    lastReconnectAttempt = millis() - BLE_RECONNECT_DELAY_MS;
    appLog("[OTA] Maintenance mode stopped");
}

void serviceOtaMode() {
    if (otaStartRequested) startOtaModeFromLoop();
    if (otaStopRequested) stopOtaMode();

    if (!otaModeActive) return;
    otaServer.handleClient();

    if (!otaUploadRunning && millis() - otaLastActivityAt >= OTA_SESSION_TIMEOUT_MS) {
        otaSetUiStatus("OTA-Zeitfenster abgelaufen");
        otaStopRequested = true;
        return;
    }

    if (otaUiDirty) {
        otaUiDirty = false;
        ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
        if (otaStatusLabel) lv_label_set_text(otaStatusLabel, otaUiStatus);
        lvgl_port_unlock();
    }

    if (otaRebootPending && (int32_t)(millis() - otaRebootAt) >= 0) {
        delay(100);
        ESP.restart();
    }
}

static void diagnosticSetUi(const char* status) {
    if (!status) return;
    if (strncmp(diagnosticUiStatus, status, sizeof(diagnosticUiStatus)) == 0) return;
    strncpy(diagnosticUiStatus, status, sizeof(diagnosticUiStatus) - 1);
    diagnosticUiStatus[sizeof(diagnosticUiStatus) - 1] = '\0';
    diagnosticUiDirty = true;
}

static void decodeDtc(uint8_t a, uint8_t b, char out[6]) {
    static const char family[4] = {'P','C','B','U'};
    out[0] = family[(a >> 6) & 0x03];
    out[1] = '0' + ((a >> 4) & 0x03);
    const uint8_t n2 = a & 0x0F, n3 = (b >> 4) & 0x0F, n4 = b & 0x0F;
    out[2] = n2 < 10 ? '0' + n2 : 'A' + (n2 - 10);
    out[3] = n3 < 10 ? '0' + n3 : 'A' + (n3 - 10);
    out[4] = n4 < 10 ? '0' + n4 : 'A' + (n4 - 10);
    out[5] = '\0';
}

static void addDtcEntry(const char* code, uint8_t group) {
    for (uint8_t i = 0; i < dtcCount; i++) {
        if (strcmp(dtcEntries[i].code, code) == 0) {
            if (group < dtcEntries[i].group) dtcEntries[i].group = group;
            return;
        }
    }
    if (dtcCount >= MAX_DTC_ENTRIES) return;
    strncpy(dtcEntries[dtcCount].code, code, 5);
    dtcEntries[dtcCount].code[5] = '\0';
    dtcEntries[dtcCount].group = group;
    dtcCount++;
}

static void parseDtcResponse(const char* response, uint8_t responseService, uint8_t group) {
    if (!response) return;

    char markerHex[3];
    snprintf(markerHex, sizeof(markerHex), "%02X", responseService);

    const char* line = response;
    while (*line) {
        const char* end = strchr(line, '\n');
        const size_t rawLen = end ? (size_t)(end - line) : strlen(line);

        char clean[192];
        size_t j = 0;
        for (size_t i = 0; i < rawLen && j < sizeof(clean) - 1; i++) {
            if (isxdigit((unsigned char)line[i])) clean[j++] = toupper((unsigned char)line[i]);
        }
        clean[j] = '\0';

        char* marker = strstr(clean, markerHex);
        if (marker) {
            char* p = marker + 2;
            while (strlen(p) >= 4) {
                char h[3] = {p[0], p[1], '\0'};
                char l[3] = {p[2], p[3], '\0'};
                const uint8_t a = (uint8_t)strtoul(h, nullptr, 16);
                const uint8_t b = (uint8_t)strtoul(l, nullptr, 16);
                if (a == 0 && b == 0) break;
                char code[6];
                decodeDtc(a, b, code);
                addDtcEntry(code, group);
                p += 4;
            }
        }

        if (!end) break;
        line = end + 1;
    }
}

static void rebuildDiagnosticText() {
    size_t used = 0;
    diagnosticUiText[0] = '\0';
    if (dtcCount == 0) {
        snprintf(diagnosticUiText, sizeof(diagnosticUiText), "Keine Fehlercodes gemeldet.\n\nScheduler: P%u  Latenz ~%.0f ms\nHeap: %u B (min %u B)\nSD: %s  Drops: %lu  IO: %lums\nRX-Drops: %lu  Crash-Streak: %u",
            schedulerPressure, pidLatencyEwmaMs, (unsigned)ESP.getFreeHeap(), (unsigned)minFreeHeapSeen,
            sdCardReady ? (sdLoggingEnabled ? "LOG" : "bereit") : "--", (unsigned long)sdDroppedRecords, (unsigned long)sdLastIoMs,
            (unsigned long)rxDroppedLines, crashStreak);
        diagnosticUiDirty = true;
        return;
    }
    for (uint8_t i = 0; i < dtcCount && used < sizeof(diagnosticUiText)-32; i++) {
        const char* state = dtcEntries[i].group == 0 ? "BESTAETIGT" : dtcEntries[i].group == 1 ? "AUSSTEHEND" : "PERMANENT";
        used += snprintf(diagnosticUiText + used, sizeof(diagnosticUiText)-used, "%s   %s\n", dtcEntries[i].code, state);
    }
    used += snprintf(diagnosticUiText + used, sizeof(diagnosticUiText)-used, "\n%u Code(s) | Scheduler P%u | Heap %u B", dtcCount, schedulerPressure, (unsigned)ESP.getFreeHeap());
    diagnosticUiDirty = true;
}

static bool startDiagnosticCommand(const char* command) {
    if (!obdConnected || !elmInitialized || !obdWrite) return false;
    portENTER_CRITICAL(&rxMux);
    diagCaptureIndex = 0;
    diagCaptureBuffer[0] = '\0';
    diagCaptureComplete = false;
    diagCaptureActive = true;
    portEXIT_CRITICAL(&rxMux);
    clearRxBuffer();
    if (!sendOBDCommand(command)) {
        portENTER_CRITICAL(&rxMux);
        diagCaptureActive = false;
        portEXIT_CRITICAL(&rxMux);
        return false;
    }
    diagnosticStateStartedAt = millis();
    return true;
}

void openDiagnosticPicker() {

    clearRxBuffer();
    obdState = OBD_IDLE;
    obdCycleActive = false;

    if (pidSupportQueryState == PID_SUPPORT_WAIT) pidSupportQueryState = PID_SUPPORT_PENDING;
    if (vinQueryState == VIN_WAIT) vinQueryState = VIN_PENDING;
    portENTER_CRITICAL(&rxMux);
    vinCaptureActive = false;
    vinCaptureComplete = false;
    portEXIT_CRITICAL(&rxMux);
    diagnosticEnterAt = millis();

    diagnosticModeActive = true;
    metricPickerDiagnosticMode = true;
    metricPickerSettingsMode = false;
    metricPickerRefreshMode = false;
    metricPickerVehicleMode = false;
    diagnosticRefreshRequested = true;
    diagnosticClearRequested = false;
    diagnosticClearArmedUntil = 0;
    portENTER_CRITICAL(&rxMux);
    diagCaptureActive = false;
    diagCaptureComplete = false;
    portEXIT_CRITICAL(&rxMux);
    diagnosticState = DIAG_IDLE;

    lv_btnmatrix_set_map(metricPickerMatrix, diagnosticActionMap);
    lv_obj_set_size(metricPickerMatrix, 650, 104);
    lv_obj_align(metricPickerMatrix, LV_ALIGN_BOTTOM_MID, 0, -20);
    if (metricPickerTitle) lv_label_set_text(metricPickerTitle, "OBD DIAGNOSE");
    if (metricPickerHint) lv_label_set_text(metricPickerHint, "Mode 03 / 07 / 0A | Loeschen nur nach Doppelbestaetigung");
    if (metricPickerDiagnosticText) {
        lv_label_set_text(metricPickerDiagnosticText, "Fehlercodes werden abgefragt ...");
        lv_obj_clear_flag(metricPickerDiagnosticText, LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(metricPickerOverlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(metricPickerOverlay);
}

static void stopDiagnosticCapture() {
    portENTER_CRITICAL(&rxMux);
    diagCaptureActive = false;
    portEXIT_CRITICAL(&rxMux);
}

void serviceDiagnosticMode() {
    if (!diagnosticModeActive) return;

    if (diagnosticUiDirty) {
        diagnosticUiDirty = false;
        ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
        if (metricPickerDiagnosticText) lv_label_set_text(metricPickerDiagnosticText, diagnosticUiText);
        if (metricPickerHint) lv_label_set_text(metricPickerHint, diagnosticUiStatus);
        lvgl_port_unlock();
    }

    if (!obdConnected || !elmInitialized) {
        diagnosticState = DIAG_IDLE;
        diagnosticSetUi("OBD nicht bereit - Verbindung wird im Hintergrund wiederhergestellt");
        return;
    }

    if (millis() - diagnosticEnterAt < 220UL) {
        diagnosticSetUi("Diagnose wird vorbereitet ...");
        return;
    }

    if (diagnosticRefreshRequested && diagnosticState == DIAG_IDLE) {
        diagnosticRefreshRequested = false;
        dtcCount = 0;
        diagnosticState = DIAG_SEND_CONFIRMED;
        diagnosticSetUi("Bestaetigte Fehler werden gelesen ...");
    }

    if (diagnosticClearRequested && (diagnosticState == DIAG_IDLE || diagnosticState == DIAG_READY)) {
        diagnosticClearRequested = false;
        diagnosticState = DIAG_SEND_CLEAR;
    }

    const uint32_t now = millis();
    auto captureFinishedOrTimeout = [&]() {
        bool complete = false;
        portENTER_CRITICAL(&rxMux);
        complete = diagCaptureComplete;
        if (!complete && now - diagnosticStateStartedAt >= 2600UL) diagCaptureActive = false;
        portEXIT_CRITICAL(&rxMux);
        return complete || (now - diagnosticStateStartedAt >= 2600UL);
    };

    switch (diagnosticState) {
        case DIAG_SEND_CONFIRMED:
            if (startDiagnosticCommand("03")) diagnosticState = DIAG_WAIT_CONFIRMED;
            else { diagnosticState = DIAG_IDLE; diagnosticSetUi("Mode 03 konnte nicht gesendet werden"); }
            break;
        case DIAG_WAIT_CONFIRMED:
            if (captureFinishedOrTimeout()) {
                stopDiagnosticCapture();
                parseDtcResponse(diagCaptureBuffer, 0x43, 0);
                diagnosticState = DIAG_SEND_PENDING;
            }
            break;
        case DIAG_SEND_PENDING:
            diagnosticSetUi("Ausstehende Fehler werden gelesen ...");
            if (startDiagnosticCommand("07")) diagnosticState = DIAG_WAIT_PENDING;
            else diagnosticState = DIAG_SEND_PERMANENT;
            break;
        case DIAG_WAIT_PENDING:
            if (captureFinishedOrTimeout()) {
                stopDiagnosticCapture();
                parseDtcResponse(diagCaptureBuffer, 0x47, 1);
                diagnosticState = DIAG_SEND_PERMANENT;
            }
            break;
        case DIAG_SEND_PERMANENT:
            diagnosticSetUi("Permanente Fehler werden gelesen ...");
            if (startDiagnosticCommand("0A")) diagnosticState = DIAG_WAIT_PERMANENT;
            else diagnosticState = DIAG_READY;
            break;
        case DIAG_WAIT_PERMANENT:
            if (captureFinishedOrTimeout()) {
                stopDiagnosticCapture();
                parseDtcResponse(diagCaptureBuffer, 0x4A, 2);
                diagnosticState = DIAG_READY;
                rebuildDiagnosticText();
                diagnosticSetUi(dtcCount ? "Diagnose abgeschlossen" : "Keine DTCs gemeldet");
            }
            break;
        case DIAG_SEND_CLEAR:
            diagnosticSetUi("Fehlercodes werden geloescht ...");
            if (startDiagnosticCommand("04")) diagnosticState = DIAG_WAIT_CLEAR;
            else { diagnosticState = DIAG_READY; diagnosticSetUi("Loeschbefehl konnte nicht gesendet werden"); }
            break;
        case DIAG_WAIT_CLEAR:
            if (captureFinishedOrTimeout()) {
                stopDiagnosticCapture();
                diagnosticState = DIAG_IDLE;
                diagnosticRefreshRequested = true;
                diagnosticSetUi("Loeschbefehl gesendet - lese erneut ...");
            }
            break;
        case DIAG_READY:
        case DIAG_IDLE:
        default: break;
    }
}

void initSdLogging() {
    if (!sdLoggingEnabled) return;
    if (!boardIOExpander) { appLog("[SD] No CH422G handle - logger disabled"); return; }
    if (ESP.getFreeHeap() < SD_INIT_MIN_FREE_HEAP) {
        appLog("[SD] Skip mount: free heap %u < %u", (unsigned)ESP.getFreeHeap(), (unsigned)SD_INIT_MIN_FREE_HEAP);
        return;
    }

    boardIOExpander->pinMode(SD_CS_EXIO, OUTPUT);
    boardIOExpander->digitalWrite(SD_CS_EXIO, LOW);
    SPI.setHwCs(false);
    SPI.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_SS_PIN);

    if (professionalWatchdogActive) esp_task_wdt_reset();
    if (!SD.begin(SD_SS_PIN)) {
        appLog("[SD] Card mount failed - dashboard continues without logging");
        boardIOExpander->digitalWrite(SD_CS_EXIO, HIGH);
        return;
    }
    if (professionalWatchdogActive) esp_task_wdt_reset();
    if (SD.cardType() == CARD_NONE) {
        appLog("[SD] No card attached");
        SD.end();
        boardIOExpander->digitalWrite(SD_CS_EXIO, HIGH);
        return;
    }

    sdCardReady = true;
    sdSlowIoStrikes = 0;
    sdLastIoMs = 0;
    SD.mkdir("/logs");
    prefs.begin("astra", false);
    uint32_t session = prefs.getUInt("logSession", 0) + 1;
    prefs.putUInt("logSession", session);
    prefs.end();
    snprintf(sdLogPath, sizeof(sdLogPath), "/logs/session_%05lu.csv", (unsigned long)session);
    if (professionalWatchdogActive) esp_task_wdt_reset();
    sdLogFile = SD.open(sdLogPath, FILE_WRITE);
    if (!sdLogFile) {
        appLog("[SD] Cannot open %s", sdLogPath);
        SD.end();
        sdCardReady = false;
        boardIOExpander->digitalWrite(SD_CS_EXIO, HIGH);
        return;
    }
    sdLogFileReady = true;
    sdLogFile.println("ms,speed_kmh,rpm,boost_bar,oil_c,coolant_c,lambda,maf_gs,load_pct,throttle_pct,l100km,voltage");
    sdLogFile.flush();
    if (professionalWatchdogActive) esp_task_wdt_reset();
    sdLastFlushAt = millis();
    appLog("[SD] Logging to %s", sdLogPath);
}

void stopSdLogging() {
    if (sdLogFileReady) {
        sdLogFile.flush();
        sdLogFile.close();
    }
    sdLogFileReady = false;
    sdCardReady = false;
    sdSlowIoStrikes = 0;
    sdLastIoMs = 0;
    sdLogHead = sdLogTail = sdPendingCount = 0;
    SD.end();
    if (boardIOExpander) boardIOExpander->digitalWrite(SD_CS_EXIO, HIGH);
    appLog("[SD] Logging stopped");
}

void serviceSdLifecycle() {

    if (sdStopRequested) {
        sdStopRequested = false;
        stopSdLogging();
    }
    if (!sdInitRequested) return;
    if (millis() - bootStartedAt < 5000UL) return;
    if (otaModeActive || otaStartRequested || diagnosticModeActive || bleScanInProgress ||
        bleManualConnectRequested || bleEarlyCandidateFound) return;
    if (obdConnected && (!elmInitialized || obdState != OBD_IDLE)) return;
    if (data.speedOK && pidFreshness(PID_SPEED) != VALUE_EXPIRED && data.speed > 3.0f) return;
    sdInitRequested = false;
    if (sdLoggingEnabled && !sdLogFileReady) initSdLogging();
}

void enqueueSdLogRecord() {
    if (!sdLoggingEnabled || !sdCardReady || !sdLogFileReady) return;
    if (ESP.getFreeHeap() < SD_INIT_MIN_FREE_HEAP) return;
    const uint32_t now = millis();
    if (now - sdLastEnqueueAt < SD_LOG_PERIOD_MS) return;
    sdLastEnqueueAt = now;

    const uint8_t next = (uint8_t)((sdLogHead + 1) % SD_LOG_RING_SIZE);
    if (next == sdLogTail) {
        sdDroppedRecords++;
        return;
    }
    LogRecord &r = sdLogRing[sdLogHead];
    r.ms = now;
    r.speed = pidFreshness(PID_SPEED) == VALUE_EXPIRED ? NAN : data.speed;
    r.rpm = pidFreshness(PID_RPM) == VALUE_EXPIRED ? NAN : data.rpm;
    r.boost = pidFreshness(PID_MAP) == VALUE_EXPIRED ? NAN : getBoostBar();
    r.oil = pidFreshness(PID_OIL) == VALUE_EXPIRED ? NAN : data.oil;
    r.coolant = pidFreshness(PID_COOLANT) == VALUE_EXPIRED ? NAN : data.coolant;
    r.lambda = pidFreshness(PID_LAMBDA) == VALUE_EXPIRED ? NAN : data.lambda;
    r.maf = pidFreshness(PID_MAF) == VALUE_EXPIRED ? NAN : data.maf;
    r.load = pidFreshness(PID_LOAD) == VALUE_EXPIRED ? NAN : data.load;
    r.throttle = pidFreshness(PID_THROTTLE) == VALUE_EXPIRED ? NAN : data.throttle;
    r.consumption = getMetricFreshness(METRIC_CONSUMPTION) == VALUE_EXPIRED ? NAN : getInstantConsumption();
    r.voltage = pidFreshness(PID_VOLTAGE) == VALUE_EXPIRED ? NAN : data.voltage;
    sdLogHead = next;
    sdPendingCount++;
}

void serviceSdLogging() {
    if (!sdLoggingEnabled || !sdLogFileReady || sdLogTail == sdLogHead) return;
    if (ESP.getFreeHeap() < SD_INIT_MIN_FREE_HEAP) return;

    if (diagnosticModeActive || otaModeActive || bleScanInProgress) return;
    if (obdConnected && (!elmInitialized || pidSupportQueryState != PID_SUPPORT_DONE || vinQueryState != VIN_DONE || obdState != OBD_IDLE)) return;

    const LogRecord r = sdLogRing[sdLogTail];
    sdLogTail = (uint8_t)((sdLogTail + 1) % SD_LOG_RING_SIZE);
    if (sdPendingCount) sdPendingCount--;

    char line[256];
    snprintf(line, sizeof(line), "%lu,%.1f,%.0f,%.2f,%.1f,%.1f,%.3f,%.2f,%.1f,%.1f,%.2f,%.2f\n",
        (unsigned long)r.ms, r.speed, r.rpm, r.boost, r.oil, r.coolant, r.lambda,
        r.maf, r.load, r.throttle, r.consumption, r.voltage);
    const size_t n = strlen(line);
    const uint32_t ioStartedAt = millis();
    if (professionalWatchdogActive) esp_task_wdt_reset();
    if (sdLogFile.write((const uint8_t*)line, n) != n) {
        appLog("[SD] Write error - logging disabled for this boot");
        sdLoggingEnabled = false;
        sdStopRequested = true;
        return;
    }
    if (professionalWatchdogActive) esp_task_wdt_reset();

    const uint32_t now = millis();
    if (now - sdLastFlushAt >= SD_LOG_FLUSH_MS) {
        if (professionalWatchdogActive) esp_task_wdt_reset();
        sdLogFile.flush();
        if (professionalWatchdogActive) esp_task_wdt_reset();
        sdLastFlushAt = now;
    }

    sdLastIoMs = millis() - ioStartedAt;

    if (sdLastIoMs > 80UL) {
        if (sdSlowIoStrikes < 255) sdSlowIoStrikes++;
        appLog("[SD] Slow I/O: %lu ms (strike %u)", (unsigned long)sdLastIoMs, sdSlowIoStrikes);
        if (sdSlowIoStrikes >= 3) {
            appLog("[SD] Card too slow - logging disabled for this boot");
            sdLoggingEnabled = false;
            sdStopRequested = true;
        }
    } else if (sdSlowIoStrikes > 0) {
        sdSlowIoStrikes--;
    }
}

void recordBootHealth() {
    const esp_reset_reason_t reason = esp_reset_reason();
    prefs.begin("astra", false);
    crashStreak = prefs.getUShort("crashStreak", 0);
    if (reason == ESP_RST_PANIC || reason == ESP_RST_TASK_WDT || reason == ESP_RST_INT_WDT || reason == ESP_RST_WDT) {
        crashStreak = min((uint16_t)999, (uint16_t)(crashStreak + 1));
        prefs.putUShort("crashStreak", crashStreak);
    }
    prefs.end();
    appLog("[HEALTH] reset_reason=%d crash_streak=%u", (int)reason, crashStreak);
}

void initProfessionalWatchdog() {
    esp_task_wdt_config_t cfg = {};
    cfg.timeout_ms = PROFESSIONAL_WDT_TIMEOUT_MS;
    cfg.idle_core_mask = 0;
    cfg.trigger_panic = true;
    esp_err_t err = esp_task_wdt_reconfigure(&cfg);
    if (err == ESP_ERR_INVALID_STATE) err = esp_task_wdt_init(&cfg);
    if (err == ESP_OK || err == ESP_ERR_INVALID_STATE) {
        const esp_err_t addErr = esp_task_wdt_add(nullptr);

        professionalWatchdogActive = (addErr == ESP_OK || addErr == ESP_ERR_INVALID_ARG);
        if (professionalWatchdogActive) {
            appLog("[HEALTH] Task watchdog active (%u ms)", (unsigned)PROFESSIONAL_WDT_TIMEOUT_MS);
        } else {
            appLog("[HEALTH] Task watchdog subscribe error %d", (int)addErr);
        }
    } else {
        professionalWatchdogActive = false;
        appLog("[HEALTH] Task watchdog init error %d", (int)err);
    }
}

void markHealthyBootIfReady() {
    if (healthyBootMarked || millis() - bootStartedAt < HEALTHY_BOOT_MARK_MS) return;
    if (ESP.getFreeHeap() < BLE_MIN_FREE_HEAP) return;

    prefs.begin("astra", false);
    prefs.putUShort("crashStreak", 0);
    prefs.end();
    crashStreak = 0;

    const esp_partition_t* running = esp_ota_get_running_partition();
    esp_ota_img_states_t state;
    if (running && esp_ota_get_state_partition(running, &state) == ESP_OK && state == ESP_OTA_IMG_PENDING_VERIFY) {
        const esp_err_t e = esp_ota_mark_app_valid_cancel_rollback();
        appLog("[OTA] Mark running image valid: %d", (int)e);
    }
    healthyBootMarked = true;
}

void closeMetricPicker() {

    const bool wasDiagnostic = metricPickerDiagnosticMode || diagnosticModeActive;
    if (metricPickerOverlay) { lv_obj_add_flag(metricPickerOverlay, LV_OBJ_FLAG_HIDDEN); }

    metricPickerSettingsMode = false;
    metricPickerRefreshMode = false;
    metricPickerVehicleMode = false;
    metricPickerDiagnosticMode = false;
    diagnosticModeActive = false;
    stopDiagnosticCapture();
    diagnosticState = DIAG_IDLE;
    diagnosticRefreshRequested = false;
    diagnosticClearRequested = false;
    metricPickerEventBusy = false;

    if (wasDiagnostic) {
        clearRxBuffer();
        obdState = OBD_IDLE;
        obdCycleActive = false;
        obdStateTime = millis();
        resetPIDTimings();
    }

    if (metricPickerDiagnosticText) lv_obj_add_flag(metricPickerDiagnosticText, LV_OBJ_FLAG_HIDDEN);
    if (metricPickerMatrix) {
        lv_obj_set_size(metricPickerMatrix, 650, 360);
        lv_obj_align(metricPickerMatrix, LV_ALIGN_TOP_MID, 0, 76);
        lv_btnmatrix_set_map(metricPickerMatrix, uiLanguage == 0 ? pickerMapDe : pickerMapEn);
    }
    if (metricPickerTitle) lv_label_set_text(metricPickerTitle, "MESSWERT WAEHLEN");
    if (metricPickerHint) lv_label_set_text(metricPickerHint, "Messwert fuer das ausgewaehlte Feld");
}

void refreshSettingsPicker() {
    if (!metricPickerMatrix) return;

    snprintf(settingsLoggingLabel, sizeof(settingsLoggingLabel), "LOGGING: %s", sdLoggingEnabled ? "AN" : "AUS");
    lv_btnmatrix_set_map(metricPickerMatrix, automaticLightMode ? settingsMapAuto : settingsMapManual);
    lv_btnmatrix_clear_btn_ctrl_all(metricPickerMatrix, LV_BTNMATRIX_CTRL_CHECKED);

    if (metricPickerTitle) lv_label_set_text(metricPickerTitle, "EINSTELLUNGEN");

    if (metricPickerHint) {
        char buf[112];
        snprintf(
            buf,
            sizeof(buf),
            "Tag %u%% | Nacht %u%% | %s | %s",
            dayBrightness,
            nightBrightness,
            automaticLightMode ? "Automatik" : "Manuell",
            uiLanguage == 0 ? "Deutsch" : "English"
        );
        lv_label_set_text(metricPickerHint, buf);
    }
}

void showSettingsPicker() {
    metricPickerSettingsMode = true;
    refreshSettingsPicker();
    lv_obj_clear_flag(metricPickerOverlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(metricPickerOverlay);
}

void onMetricPickerBgClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;
    if (lv_event_get_target(event) == metricPickerOverlay) closeMetricPicker();
}

void onMetricPickerMatrixClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_VALUE_CHANGED) return;
    if (!metricPickerMatrix || metricPickerEventBusy) return;

    uint16_t selected = lv_btnmatrix_get_selected_btn(metricPickerMatrix);
    if (selected == LV_BTNMATRIX_BTN_NONE) return;
    metricPickerEventBusy = true;

    if (metricPickerDiagnosticMode) {
        if (selected == 0) {
            diagnosticState = DIAG_IDLE;
            diagnosticRefreshRequested = true;
            diagnosticSetUi("Diagnose wird aktualisiert ...");
        } else if (selected == 1) {
            const uint32_t now = millis();
            if (diagnosticClearArmedUntil == 0 || (int32_t)(now - diagnosticClearArmedUntil) >= 0) {
                diagnosticClearArmedUntil = now + 5000UL;
                diagnosticSetUi("ACHTUNG: Noch einmal FEHLER LOESCHEN druecken (5 s)");
            } else {
                diagnosticClearArmedUntil = 0;
                diagnosticClearRequested = true;
            }
        } else if (selected == 2) {
            metricPickerEventBusy = false;
            closeMetricPicker();
            return;
        }
        metricPickerEventBusy = false;
        return;
    }

    if (metricPickerVehicleMode) {
        if (selected < VEH_PROFILE_COUNT) {
            vehicleProfileSetting = (VehicleProfile)selected;
            requestDisplaySettingsPersist();
            updateVehicleHeader();
            resetPIDTimings();
        }
        metricPickerVehicleMode = false;
        metricPickerSettingsMode = true;
        refreshSettingsPicker();
        metricPickerEventBusy = false;
        return;
    }

    if (metricPickerRefreshMode) {
        if (metricPickerTargetSlot >= DASHBOARD_SLOT_COUNT && metricPickerTargetSlot != PICKER_SLOT_RPM_GAUGE) { metricPickerEventBusy = false; return; }
        const char* rateText = lv_btnmatrix_get_btn_text(metricPickerMatrix, selected);
        uint8_t selectedHz = 0;
        if (rateText) {
            if (strcmp(rateText, "1 x/s") == 0) selectedHz = 1;
            else if (strcmp(rateText, "2 x/s") == 0) selectedHz = 2;
            else if (strcmp(rateText, "5 x/s") == 0) selectedHz = 5;
            else if (strcmp(rateText, "10 x/s") == 0) selectedHz = 10;
            else if (strcmp(rateText, "20 x/s") == 0) selectedHz = 20;
        }
        if (selectedHz == 0) { metricPickerEventBusy = false; return; }

        if (metricPickerTargetSlot == PICKER_SLOT_RPM_GAUGE) {
            rpmGaugeRefreshHz = selectedHz;
        } else {
            dashboardSlotRefreshHz[metricPickerTargetSlot] = selectedHz;
            dashboardSlotLastUiUpdate[metricPickerTargetSlot] = 0;
        }
        applySlotRefreshRatesToPidTable();
        requestDashboardPersist();
        metricPickerEventBusy = false;
        closeMetricPicker();
        return;
    }

    if (metricPickerSettingsMode) {
        switch (selected) {
            case 0: dayBrightness = max(10, (int)dayBrightness - 5); break;
            case 1: dayBrightness = min(100, (int)dayBrightness + 5); break;
            case 2: nightBrightness = max(5, (int)nightBrightness - 5); break;
            case 3: nightBrightness = min(80, (int)nightBrightness + 5); break;
            case 4: automaticLightMode = !automaticLightMode; break;
            case 5: uiLanguage = 0; break;
            case 6: uiLanguage = 1; break;
            case 7: displayRotated = !displayRotated; applyDisplayRotation(); break;
            case 8:
                metricPickerVehicleMode = true;
                metricPickerSettingsMode = false;
                lv_btnmatrix_set_map(metricPickerMatrix, vehicleProfileMap);
                lv_btnmatrix_clear_btn_ctrl_all(metricPickerMatrix, LV_BTNMATRIX_CTRL_CHECKED);
                if ((uint16_t)vehicleProfileSetting < VEH_PROFILE_COUNT) {
                    lv_btnmatrix_set_btn_ctrl(metricPickerMatrix, (uint16_t)vehicleProfileSetting, LV_BTNMATRIX_CTRL_CHECKABLE | LV_BTNMATRIX_CTRL_CHECKED);
                }
                if (metricPickerTitle) lv_label_set_text(metricPickerTitle, "OPEL FAHRZEUG");
                if (metricPickerHint) lv_label_set_text(metricPickerHint, vehicleVinValid ? vehicleVIN : "AUTO liest VIN via OBD 09 02");
                metricPickerEventBusy = false;
                return;
            case 9:
                metricPickerEventBusy = false;
                openDiagnosticPicker();
                return;
            case 10:
                sdLoggingEnabled = !sdLoggingEnabled;
                if (sdLoggingEnabled) sdInitRequested = true;
                else sdStopRequested = true;
                requestDisplaySettingsPersist();
                refreshSettingsPicker();
                metricPickerEventBusy = false;
                return;
            case 11:
                metricPickerEventBusy = false;
                requestStartOtaMode();
                return;
            case 12:
                metricPickerEventBusy = false;
                closeMetricPicker();
                return;
            default:
                metricPickerEventBusy = false;
                return;
        }

        Serial.println("[UI TRACE 21] before applyDisplayBrightness");
    applyDisplayBrightness();
    Serial.println("[UI TRACE 22] after applyDisplayBrightness");
        updateManualLightControl();
        applyLanguageToDashboard();
        requestDisplaySettingsPersist();
        refreshSettingsPicker();
        metricPickerEventBusy = false;
        return;
    }

    if (metricPickerTargetSlot >= DASHBOARD_SLOT_COUNT) { metricPickerEventBusy = false; return; }
    if (selected == METRIC_COUNT) {
        dashboardSlotHidden[metricPickerTargetSlot] = true;
        dashboardSlotLastUiUpdate[metricPickerTargetSlot] = 0;
        applySlotRefreshRatesToPidTable();
        requestDashboardPersist();
        lv_label_set_text(dashboardPanelTitle[metricPickerTargetSlot], uiLanguage == 0 ? "AUSGEBLENDET" : "HIDDEN");
        lv_label_set_text(dashboardPanelValue[metricPickerTargetSlot], "");
        lv_label_set_text(dashboardPanelUnit[metricPickerTargetSlot], "");
        if (dashboardPanelIcon[metricPickerTargetSlot]) lv_obj_add_flag(dashboardPanelIcon[metricPickerTargetSlot], LV_OBJ_FLAG_HIDDEN);
        applyDashboardSlotVisibility(metricPickerTargetSlot);
        metricPickerEventBusy = false;
        closeMetricPicker();
        return;
    }
    if (selected >= METRIC_COUNT) { metricPickerEventBusy = false; return; }

    MetricId chosen = (MetricId)selected;
    dashboardSlotHidden[metricPickerTargetSlot] = false;
    dashboardSlotLastUiUpdate[metricPickerTargetSlot] = 0;
    applyDashboardSlotVisibility(metricPickerTargetSlot);
    dashboardSlotMetric[metricPickerTargetSlot] = chosen;
    requestDashboardPersist();

    lv_label_set_text(dashboardPanelTitle[metricPickerTargetSlot], metricDisplayName(chosen));
    lv_obj_set_style_text_color(dashboardPanelTitle[metricPickerTargetSlot], metricDefs[chosen].color, 0);

    lv_label_set_text(dashboardPanelUnit[metricPickerTargetSlot], metricUnit(chosen));
    lv_label_set_text(dashboardPanelValue[metricPickerTargetSlot], "--");

    updateDashboardPanelIcon(metricPickerTargetSlot, chosen);

    lv_obj_set_style_text_color(dashboardPanelValue[metricPickerTargetSlot], COL_WHITE, 0);

    metricPickerRefreshMode = true;
    metricPickerSettingsMode = false;
    lv_btnmatrix_set_map(metricPickerMatrix, refreshRateMap);
    lv_btnmatrix_clear_btn_ctrl_all(metricPickerMatrix, LV_BTNMATRIX_CTRL_CHECKED);

    uint16_t rateIndex = 0;
    for (uint16_t r = 0; r < (sizeof(refreshRateValues) / sizeof(refreshRateValues[0])); r++) {
        if (refreshRateValues[r] == dashboardSlotRefreshHz[metricPickerTargetSlot]) {
            rateIndex = r;
            break;
        }
    }
    lv_btnmatrix_set_btn_ctrl(metricPickerMatrix, rateIndex, LV_BTNMATRIX_CTRL_CHECKABLE | LV_BTNMATRIX_CTRL_CHECKED);

    if (metricPickerTitle) lv_label_set_text(metricPickerTitle, uiLanguage == 0 ? "AKTUALISIERUNGSRATE" : "REFRESH RATE");
    if (metricPickerHint) lv_label_set_text(metricPickerHint, uiLanguage == 0 ? "Wie oft pro Sekunde aktualisieren?" : "Updates per second");
    metricPickerEventBusy = false;
}

void createMetricPicker() {

    metricPickerOverlay = lv_obj_create(lv_layer_top());

    lv_obj_set_size(metricPickerOverlay, 1024, 600);
    lv_obj_set_pos(metricPickerOverlay, 0, 0);
    lv_obj_set_style_bg_color(metricPickerOverlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(metricPickerOverlay, LV_OPA_70, 0);
    lv_obj_set_style_border_width(metricPickerOverlay, 0, 0);
    lv_obj_set_style_radius(metricPickerOverlay, 0, 0);
    lv_obj_clear_flag(metricPickerOverlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(metricPickerOverlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(metricPickerOverlay, onMetricPickerBgClicked, LV_EVENT_CLICKED, nullptr);

    metricPickerPanel = lv_obj_create(metricPickerOverlay);

    lv_obj_set_size(metricPickerPanel, 700, 500);
    lv_obj_align(metricPickerPanel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(metricPickerPanel, COL_PANEL, 0);
    lv_obj_set_style_bg_grad_color(metricPickerPanel, COL_PANEL2, 0);
    lv_obj_set_style_bg_grad_dir(metricPickerPanel, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_radius(metricPickerPanel, 16, 0);
    lv_obj_set_style_border_width(metricPickerPanel, 1, 0);
    lv_obj_set_style_border_color(metricPickerPanel, lv_color_hex(0x353B47), 0);
    lv_obj_clear_flag(metricPickerPanel, LV_OBJ_FLAG_SCROLLABLE);

    metricPickerTitle = lv_label_create(metricPickerPanel);
    lv_label_set_text(metricPickerTitle, "MESSWERT WAEHLEN");
    lv_obj_set_style_text_font(metricPickerTitle, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(metricPickerTitle, COL_WHITE, 0);
    lv_obj_align(metricPickerTitle, LV_ALIGN_TOP_LEFT, 24, 16);

    metricPickerHint = lv_label_create(metricPickerPanel);
    lv_label_set_text(metricPickerHint, "Messwert fuer das ausgewaehlte Feld");
    lv_obj_set_style_text_font(metricPickerHint, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(metricPickerHint, COL_GREY, 0);
    lv_obj_align(metricPickerHint, LV_ALIGN_TOP_LEFT, 25, 45);

    metricPickerDiagnosticText = lv_label_create(metricPickerPanel);
    lv_label_set_long_mode(metricPickerDiagnosticText, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(metricPickerDiagnosticText, 640);
    lv_obj_set_height(metricPickerDiagnosticText, 270);
    lv_obj_set_style_text_font(metricPickerDiagnosticText, UI_FONT_BODY_16, 0);
    lv_obj_set_style_text_color(metricPickerDiagnosticText, COL_WHITE, 0);
    lv_obj_align(metricPickerDiagnosticText, LV_ALIGN_TOP_LEFT, 24, 84);
    lv_obj_add_flag(metricPickerDiagnosticText, LV_OBJ_FLAG_HIDDEN);

    metricPickerMatrix = lv_btnmatrix_create(metricPickerPanel);
    lv_btnmatrix_set_map(metricPickerMatrix, uiLanguage == 0 ? pickerMapDe : pickerMapEn);
    lv_obj_set_size(metricPickerMatrix, 650, 360);
    lv_obj_align(metricPickerMatrix, LV_ALIGN_TOP_MID, 0, 76);

    lv_obj_set_style_bg_color(metricPickerMatrix, COL_BG, 0);
    lv_obj_set_style_bg_opa(metricPickerMatrix, LV_OPA_100, 0);
    lv_obj_set_style_border_width(metricPickerMatrix, 0, 0);
    lv_obj_set_style_radius(metricPickerMatrix, 10, 0);
    lv_obj_set_style_pad_all(metricPickerMatrix, 8, 0);
    lv_obj_set_style_pad_row(metricPickerMatrix, 8, 0);
    lv_obj_set_style_pad_column(metricPickerMatrix, 8, 0);

    lv_obj_set_style_bg_color(metricPickerMatrix, lv_color_hex(0x171C25), LV_PART_ITEMS);
    lv_obj_set_style_border_width(metricPickerMatrix, 1, LV_PART_ITEMS);
    lv_obj_set_style_border_color(metricPickerMatrix, lv_color_hex(0x303743), LV_PART_ITEMS);
    lv_obj_set_style_radius(metricPickerMatrix, 8, LV_PART_ITEMS);
    lv_obj_set_style_text_font(metricPickerMatrix, &lv_font_montserrat_14, LV_PART_ITEMS);
    lv_obj_set_style_text_color(metricPickerMatrix, COL_WHITE, LV_PART_ITEMS);

    lv_obj_set_style_bg_color(metricPickerMatrix, COL_BRAND_PURPLE, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(metricPickerMatrix, COL_BRAND_PURPLE, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(metricPickerMatrix, COL_BRAND_BLUE, LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_obj_add_event_cb(metricPickerMatrix, onMetricPickerMatrixClicked, LV_EVENT_VALUE_CHANGED, nullptr);
}

void onRpmGaugeLongPress(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_LONG_PRESSED) return;
    if (!metricPickerMatrix) return;

    metricPickerTargetSlot = PICKER_SLOT_RPM_GAUGE;
    metricPickerSettingsMode = false;
    metricPickerVehicleMode = false;
    metricPickerRefreshMode = true;
    metricPickerEventBusy = false;

    lv_btnmatrix_set_map(metricPickerMatrix, refreshRateMap);
    lv_btnmatrix_clear_btn_ctrl_all(metricPickerMatrix, LV_BTNMATRIX_CTRL_CHECKED);

    uint16_t rateIndex = 0;
    for (uint16_t r = 0; r < (sizeof(refreshRateValues) / sizeof(refreshRateValues[0])); r++) {
        if (refreshRateValues[r] == rpmGaugeRefreshHz) {
            rateIndex = r;
            break;
        }
    }
    lv_btnmatrix_set_btn_ctrl(metricPickerMatrix, rateIndex, LV_BTNMATRIX_CTRL_CHECKABLE | LV_BTNMATRIX_CTRL_CHECKED);

    if (metricPickerTitle) lv_label_set_text(metricPickerTitle, uiLanguage == 0 ? "DREHZAHLMESSER" : "RPM GAUGE");
    if (metricPickerHint) lv_label_set_text(metricPickerHint, uiLanguage == 0 ? "Wie oft pro Sekunde aktualisieren?" : "Updates per second");

    if (metricPickerOverlay) {
        lv_obj_clear_flag(metricPickerOverlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(metricPickerOverlay);
    }
}

void onCenterMetricLongPress(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_LONG_PRESSED) return;
    const uint8_t idx = (uint8_t)(intptr_t)lv_event_get_user_data(event);
    if (idx >= 3) return;

    static uint32_t lastToggleAt = 0;
    const uint32_t now = millis();
    if (lastToggleAt != 0 && now - lastToggleAt < 800UL) return;
    lastToggleAt = now;

    centerMetricHidden[idx] = !centerMetricHidden[idx];
    applyCenterMetricVisibility(idx);
    requestDashboardPersist();
}

void onDashboardSlotLongPress(lv_event_t* event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code != LV_EVENT_LONG_PRESSED && code != LV_EVENT_CLICKED) return;

    metricPickerTargetSlot = (uint8_t)(intptr_t)lv_event_get_user_data(event);
    metricPickerSettingsMode = false;
    metricPickerRefreshMode = false;
    metricPickerVehicleMode = false;
    metricPickerEventBusy = false;

    lv_btnmatrix_set_map(metricPickerMatrix, uiLanguage == 0 ? pickerMapDe : pickerMapEn);
    lv_btnmatrix_clear_btn_ctrl_all(metricPickerMatrix, LV_BTNMATRIX_CTRL_CHECKED);
    if (dashboardSlotHidden[metricPickerTargetSlot]) {
        lv_btnmatrix_set_btn_ctrl(metricPickerMatrix, METRIC_COUNT, LV_BTNMATRIX_CTRL_CHECKABLE | LV_BTNMATRIX_CTRL_CHECKED);
    } else {
        lv_btnmatrix_set_btn_ctrl(
            metricPickerMatrix,
            (uint16_t)dashboardSlotMetric[metricPickerTargetSlot],
            LV_BTNMATRIX_CTRL_CHECKABLE | LV_BTNMATRIX_CTRL_CHECKED
        );
    }

    if (metricPickerOverlay) {
        lv_obj_clear_flag(metricPickerOverlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(metricPickerOverlay);
    }
}

lv_obj_t* createMaskImage(lv_obj_t* parent, const lv_img_dsc_t* src, lv_color_t color, int x, int y, uint16_t zoom = 256) {
    lv_obj_t* image = lv_img_create(parent);
    lv_img_set_src(image, src);
    lv_img_set_zoom(image, zoom);
    lv_obj_set_style_img_recolor(image, color, 0);
    lv_obj_set_style_img_recolor_opa(image, LV_OPA_COVER, 0);
    lv_obj_set_pos(image, x, y);
    lv_obj_clear_flag(image, LV_OBJ_FLAG_CLICKABLE);
    return image;
}

void updateDashboardPanelIcon(uint8_t slot, MetricId metric) {
    if (slot >= DASHBOARD_SLOT_COUNT || !dashboardPanelIcon[slot]) return;

    const lv_img_dsc_t* src = nullptr;
    lv_color_t color = metricDefs[metric].color;

    if (dashboardPanelAccent[slot]) lv_obj_set_style_bg_color(dashboardPanelAccent[slot], color, 0);

    switch (metric) {
        case METRIC_SPEED: src = &speed_icon; break;
        case METRIC_RPM: src = &rpm_icon; break;
        case METRIC_BOOST: src = &boost_icon; break;
        case METRIC_LOAD: src = &load_icon; break;
        case METRIC_COOLANT: src = &coolant_icon; break;
        case METRIC_OIL: src = &oil_icon; break;
        case METRIC_INTAKE: src = &intake_icon; break;
        case METRIC_FUEL_LEVEL: src = &fuel_icon; break;
        case METRIC_VOLTAGE: src = &voltage_icon; break;
        case METRIC_THROTTLE: src = &throttle_icon; break;
        case METRIC_LAMBDA: src = &lambda_icon; break;
        case METRIC_MAF: src = &maf_icon; break;
        case METRIC_POWER: src = &power_icon; break;
        case METRIC_CONSUMPTION: src = &consumption_icon; break;
        default: break;
    }

    lv_img_set_src(dashboardPanelIcon[slot], src);
    lv_obj_set_style_img_recolor(dashboardPanelIcon[slot], color, 0);
    lv_obj_set_style_img_recolor_opa(dashboardPanelIcon[slot], LV_OPA_COVER, 0);
    lv_obj_align(dashboardPanelIcon[slot], LV_ALIGN_LEFT_MID, 0, 5);
    lv_obj_clear_flag(dashboardPanelIcon[slot], LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(dashboardPanelIcon[slot], LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(dashboardPanelTitle[slot], LV_ALIGN_TOP_LEFT, 52, 0);
    lv_obj_align(dashboardPanelValue[slot], LV_ALIGN_BOTTOM_LEFT, 52, 0);
}

void createCenterMetricCards() {
    const MetricId metric[3] = { METRIC_THROTTLE, METRIC_LAMBDA, METRIC_MAF };
    const int x[3] = { 280, 437, 594 };

    for (uint8_t i = 0; i < 3; i++) {
        lv_obj_t* card = createPanel(dashboardScreen, x[i], 442, 150, 90);
        lv_obj_set_style_pad_left(card, 10, 0);
        lv_obj_set_style_pad_right(card, 10, 0);
        lv_obj_set_style_border_color(card, lv_color_hex(0x56309A), 0);
        centerMetricCard[i] = card;
        lv_obj_add_event_cb(card, onCenterMetricLongPress, LV_EVENT_LONG_PRESSED, (void*)(intptr_t)i);

        lv_obj_t* centerHotspot = lv_obj_create(dashboardScreen);
        lv_obj_remove_style_all(centerHotspot);
        lv_obj_set_pos(centerHotspot, x[i], 442);
        lv_obj_set_size(centerHotspot, 150, 90);
        lv_obj_clear_flag(centerHotspot, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(centerHotspot, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(centerHotspot, onCenterMetricLongPress, LV_EVENT_LONG_PRESSED, (void*)(intptr_t)i);
        centerMetricHotspot[i] = centerHotspot;
        applyCenterMetricVisibility(i);

        centerMetricTitle[i] = lv_label_create(card);
        lv_label_set_text(centerMetricTitle[i], metricDisplayName(metric[i]));
        lv_obj_set_style_text_font(centerMetricTitle[i], &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(centerMetricTitle[i], COL_BRAND_PURPLE, 0);
        lv_obj_align(centerMetricTitle[i], LV_ALIGN_TOP_LEFT, 0, 0);

        centerMetricValue[i] = lv_label_create(card);
        lv_label_set_text(centerMetricValue[i], "--");
        lv_obj_set_style_text_font(centerMetricValue[i], &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(centerMetricValue[i], COL_WHITE, 0);
        lv_obj_align(centerMetricValue[i], LV_ALIGN_BOTTOM_LEFT, 0, -4);

        centerMetricUnit[i] = lv_label_create(card);
        lv_label_set_text(centerMetricUnit[i], metricUnit(metric[i]));
        lv_obj_set_style_text_font(centerMetricUnit[i], &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(centerMetricUnit[i], COL_BRAND_PURPLE, 0);
        lv_obj_align(centerMetricUnit[i], LV_ALIGN_BOTTOM_RIGHT, 0, -4);
    }
}

void applyLanguageToDashboard() {
    for (uint8_t i = 0; i < DASHBOARD_SLOT_COUNT; i++) {
        if (!dashboardPanelTitle[i]) continue;
        lv_label_set_text(dashboardPanelTitle[i], dashboardSlotHidden[i] ? (uiLanguage == 0 ? "AUSGEBLENDET" : "HIDDEN") : metricDisplayName(dashboardSlotMetric[i]));
    }

    const MetricId centerMetric[3] = { METRIC_THROTTLE, METRIC_LAMBDA, METRIC_MAF };
    for (uint8_t i = 0; i < 3; i++) {
        if (centerMetricTitle[i]) lv_label_set_text(centerMetricTitle[i], metricDisplayName(centerMetric[i]));
    }

    if (settingsTitleLabel) lv_label_set_text(settingsTitleLabel, uiLanguage == 0 ? "EINSTELLUNGEN" : "SETTINGS");
    if (dayBrightnessLabel) lv_label_set_text(dayBrightnessLabel, uiLanguage == 0 ? "HELLIGKEIT TAG" : "DAY BRIGHTNESS");
    if (nightBrightnessLabel) lv_label_set_text(nightBrightnessLabel, uiLanguage == 0 ? "NACHTHELLIGKEIT" : "NIGHT BRIGHTNESS");
    if (languageLabel) lv_label_set_text(languageLabel, uiLanguage == 0 ? "SPRACHE" : "LANGUAGE");
    if (automaticLightLabel) lv_label_set_text(automaticLightLabel, uiLanguage == 0 ? "AUTOMATISCHES LICHT" : "AUTOMATIC LIGHT");
    if (metricPickerMatrix) lv_btnmatrix_set_map(metricPickerMatrix, uiLanguage == 0 ? pickerMapDe : pickerMapEn);
}

void updateManualLightControl() {
    if (!manualLightButton) return;

    if (automaticLightMode) {
        lv_obj_add_flag(manualLightButton, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_label_set_text(manualLightButtonLabel, manualNightMode ? "NACHT" : "TAG");
    lv_obj_set_style_bg_color(manualLightButton, manualNightMode ? COL_BRAND_PURPLE : COL_PANEL2, 0);
    lv_obj_clear_flag(manualLightButton, LV_OBJ_FLAG_HIDDEN);
}

void onManualLightToggle(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;

    manualNightMode = !manualNightMode;
    updateManualLightControl();
    Serial.println("[UI TRACE 21] before applyDisplayBrightness");
    applyDisplayBrightness();
    Serial.println("[UI TRACE 22] after applyDisplayBrightness");
    requestDisplaySettingsPersist();
}

void onSettingsValueChanged(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);
    if (code != LV_EVENT_VALUE_CHANGED && code != LV_EVENT_RELEASED) return;

    dayBrightness = (uint8_t)lv_slider_get_value(dayBrightnessSlider);
    nightBrightness = (uint8_t)lv_slider_get_value(nightBrightnessSlider);
    uiLanguage = (uint8_t)lv_dropdown_get_selected(languageDropdown);
    automaticLightMode = lv_obj_has_state(automaticLightSwitch, LV_STATE_CHECKED);

    char valueBuf[8];

    if (dayBrightnessValueLabel) {
        snprintf(valueBuf, sizeof(valueBuf), "%u%%", dayBrightness);
        lv_label_set_text(dayBrightnessValueLabel, valueBuf);
    }

    if (nightBrightnessValueLabel) {
        snprintf(valueBuf, sizeof(valueBuf), "%u%%", nightBrightness);
        lv_label_set_text(nightBrightnessValueLabel, valueBuf);
    }

    Serial.println("[UI TRACE 21] before applyDisplayBrightness");
    applyDisplayBrightness();
    Serial.println("[UI TRACE 22] after applyDisplayBrightness");
    updateManualLightControl();
    applyLanguageToDashboard();
    requestDisplaySettingsPersist();
}

void closeSettings(lv_event_t* event) {
    if (lv_event_get_code(event) == LV_EVENT_CLICKED && settingsOverlay) lv_obj_add_flag(settingsOverlay, LV_OBJ_FLAG_HIDDEN);
}

void onSettingsButtonClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;
    showSettingsPicker();
}

void createSettingsOverlay() {
    settingsOverlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(settingsOverlay, 1024, 600);
    lv_obj_set_pos(settingsOverlay, 0, 0);
    lv_obj_set_style_bg_color(settingsOverlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(settingsOverlay, LV_OPA_80, 0);
    lv_obj_set_style_border_width(settingsOverlay, 0, 0);
    lv_obj_clear_flag(settingsOverlay, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* panel = lv_obj_create(settingsOverlay);
    lv_obj_set_size(panel, 620, 470);
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, COL_PANEL, 0);
    lv_obj_set_style_border_color(panel, COL_BRAND_PURPLE, 0);
    lv_obj_set_style_border_width(panel, 2, 0);
    lv_obj_set_style_radius(panel, 16, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    settingsTitleLabel = lv_label_create(panel);
    lv_obj_set_style_text_font(settingsTitleLabel, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(settingsTitleLabel, COL_WHITE, 0);
    lv_obj_align(settingsTitleLabel, LV_ALIGN_TOP_LEFT, 20, 16);

    dayBrightnessLabel = lv_label_create(panel);
    lv_obj_set_style_text_font(dayBrightnessLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(dayBrightnessLabel, COL_GREY, 0);
    lv_obj_align(dayBrightnessLabel, LV_ALIGN_TOP_LEFT, 24, 78);

    dayBrightnessSlider = lv_slider_create(panel);
    lv_obj_set_size(dayBrightnessSlider, 530, 14);
    lv_obj_align(dayBrightnessSlider, LV_ALIGN_TOP_MID, 0, 112);
    lv_slider_set_range(dayBrightnessSlider, 10, 100);
    lv_slider_set_value(dayBrightnessSlider, dayBrightness, LV_ANIM_OFF);

    dayBrightnessValueLabel = lv_label_create(panel);
    char brightnessBuf[8];
    snprintf(brightnessBuf, sizeof(brightnessBuf), "%u%%", dayBrightness);
    lv_label_set_text(dayBrightnessValueLabel, brightnessBuf);
    lv_obj_set_style_text_color(dayBrightnessValueLabel, COL_WHITE, 0);
    lv_obj_align(dayBrightnessValueLabel, LV_ALIGN_TOP_RIGHT, -24, 76);

    nightBrightnessLabel = lv_label_create(panel);
    lv_obj_set_style_text_font(nightBrightnessLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(nightBrightnessLabel, COL_GREY, 0);
    lv_obj_align(nightBrightnessLabel, LV_ALIGN_TOP_LEFT, 24, 162);

    nightBrightnessSlider = lv_slider_create(panel);
    lv_obj_set_size(nightBrightnessSlider, 530, 14);
    lv_obj_align(nightBrightnessSlider, LV_ALIGN_TOP_MID, 0, 196);
    lv_slider_set_range(nightBrightnessSlider, 5, 80);
    lv_slider_set_value(nightBrightnessSlider, nightBrightness, LV_ANIM_OFF);

    nightBrightnessValueLabel = lv_label_create(panel);
    snprintf(brightnessBuf, sizeof(brightnessBuf), "%u%%", nightBrightness);
    lv_label_set_text(nightBrightnessValueLabel, brightnessBuf);
    lv_obj_set_style_text_color(nightBrightnessValueLabel, COL_WHITE, 0);
    lv_obj_align(nightBrightnessValueLabel, LV_ALIGN_TOP_RIGHT, -24, 160);

    languageLabel = lv_label_create(panel);
    lv_obj_set_style_text_font(languageLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(languageLabel, COL_GREY, 0);
    lv_obj_align(languageLabel, LV_ALIGN_TOP_LEFT, 24, 246);

    languageDropdown = lv_dropdown_create(panel);
    lv_dropdown_set_options(languageDropdown, "Deutsch\nEnglish");
    lv_dropdown_set_selected(languageDropdown, uiLanguage);
    lv_obj_set_size(languageDropdown, 240, 48);
    lv_obj_align(languageDropdown, LV_ALIGN_TOP_LEFT, 24, 274);

    automaticLightLabel = lv_label_create(panel);
    lv_obj_set_style_text_font(automaticLightLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(automaticLightLabel, COL_GREY, 0);
    lv_obj_align(automaticLightLabel, LV_ALIGN_TOP_LEFT, 24, 340);

    automaticLightSwitch = lv_switch_create(panel);
    lv_obj_align(automaticLightSwitch, LV_ALIGN_TOP_RIGHT, -24, 332);
    if (automaticLightMode) lv_obj_add_state(automaticLightSwitch, LV_STATE_CHECKED);

    lv_obj_t* closeBtn = lv_btn_create(panel);
    lv_obj_set_size(closeBtn, 160, 44);
    lv_obj_align(closeBtn, LV_ALIGN_BOTTOM_RIGHT, -20, -18);
    lv_obj_add_event_cb(closeBtn, closeSettings, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* closeLabel = lv_label_create(closeBtn);
    lv_label_set_text(closeLabel, "OK");
    lv_obj_center(closeLabel);

    lv_obj_add_event_cb(dayBrightnessSlider, onSettingsValueChanged, LV_EVENT_RELEASED, nullptr);
    lv_obj_add_event_cb(nightBrightnessSlider, onSettingsValueChanged, LV_EVENT_RELEASED, nullptr);
    lv_obj_add_event_cb(languageDropdown, onSettingsValueChanged, LV_EVENT_VALUE_CHANGED, nullptr);
    lv_obj_add_event_cb(automaticLightSwitch, onSettingsValueChanged, LV_EVENT_VALUE_CHANGED, nullptr);

    applyLanguageToDashboard();
    lv_obj_add_flag(settingsOverlay, LV_OBJ_FLAG_HIDDEN);
}

void createDashboard() {
    dashboardScreen = lv_obj_create(NULL);
    setScreenBackground(dashboardScreen);

    vehicleModelLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(vehicleModelLabel, "ASTRA");
    lv_obj_set_style_text_font(vehicleModelLabel, UI_FONT_VEHICLE_MODEL, 0);
    lv_obj_set_style_text_color(vehicleModelLabel, COL_WHITE, 0);
    lv_obj_set_pos(vehicleModelLabel, 20, 5);

    vehicleTrimLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(vehicleTrimLabel, "OPC");
    lv_obj_set_style_text_font(vehicleTrimLabel, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(vehicleTrimLabel, COL_BRAND_PURPLE, 0);
    lv_obj_align_to(vehicleTrimLabel, vehicleModelLabel, LV_ALIGN_OUT_RIGHT_MID, 10, 1);
    updateVehicleHeader();

    vehicleSubHeaderLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(vehicleSubHeaderLabel, "2.0 TURBO  |  PERFORMANCE");
    lv_obj_set_style_text_font(vehicleSubHeaderLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(vehicleSubHeaderLabel, COL_GREY, 0);
    lv_obj_set_pos(vehicleSubHeaderLabel, 21, 36);
    updateVehicleHeader();

    createMaskImage(dashboardScreen, &opel_logo_large, COL_WHITE, 473, 0);

    static lv_point_t headerLeft[] = { {0,67}, {452,67}, {462,60}, {473,48} };
    static lv_point_t headerRight[] = { {551,48}, {562,60}, {572,67}, {1023,67} };

    lv_obj_t* headerLineL = lv_line_create(dashboardScreen);
    lv_line_set_points(headerLineL, headerLeft, 4);
    lv_obj_set_style_line_width(headerLineL, 1, 0);
    lv_obj_set_style_line_color(headerLineL, COL_BRAND_PURPLE, 0);

    lv_obj_t* headerLineR = lv_line_create(dashboardScreen);
    lv_line_set_points(headerLineR, headerRight, 4);
    lv_obj_set_style_line_width(headerLineR, 1, 0);
    lv_obj_set_style_line_color(headerLineR, COL_BRAND_BLUE, 0);

    connectionLabel = lv_btn_create(dashboardScreen);
    lv_obj_set_size(connectionLabel, 48, 42);
    lv_obj_align(connectionLabel, LV_ALIGN_TOP_RIGHT, -20, 10);
    lv_obj_set_style_bg_color(connectionLabel, COL_PANEL2, 0);
    lv_obj_set_style_border_width(connectionLabel, 1, 0);
    lv_obj_set_style_border_color(connectionLabel, COL_BRAND_PURPLE, 0);
    lv_obj_set_style_radius(connectionLabel, 10, 0);
    lv_obj_add_event_cb(connectionLabel, onSettingsButtonClicked, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* settingsIcon = lv_label_create(connectionLabel);
    lv_label_set_text(settingsIcon, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_font(settingsIcon, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(settingsIcon, COL_WHITE, 0);
    lv_obj_center(settingsIcon);

    manualLightButton = lv_btn_create(dashboardScreen);
    lv_obj_set_size(manualLightButton, 78, 42);
    lv_obj_align(manualLightButton, LV_ALIGN_TOP_RIGHT, -78, 10);
    lv_obj_set_style_border_width(manualLightButton, 1, 0);
    lv_obj_set_style_border_color(manualLightButton, COL_BRAND_PURPLE, 0);
    lv_obj_set_style_radius(manualLightButton, 10, 0);
    lv_obj_add_event_cb(manualLightButton, onManualLightToggle, LV_EVENT_CLICKED, nullptr);

    manualLightButtonLabel = lv_label_create(manualLightButton);
    lv_obj_set_style_text_font(manualLightButtonLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(manualLightButtonLabel, COL_WHITE, 0);
    lv_obj_center(manualLightButtonLabel);

    lv_obj_t* copyrightLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(copyrightLabel, "(c) Stefan Schieren");
    lv_obj_set_style_text_font(copyrightLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(copyrightLabel, lv_color_hex(0x3D4654), 0);
    lv_obj_align_to(copyrightLabel, manualLightButton, LV_ALIGN_OUT_LEFT_MID, -14, 0);
    updateManualLightControl();

    connectionStatusLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(connectionStatusLabel, "OBD OFFLINE");
    lv_obj_set_style_text_font(connectionStatusLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(connectionStatusLabel, COL_RED, 0);
    lv_obj_add_flag(connectionStatusLabel, LV_OBJ_FLAG_HIDDEN);

    speedLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(speedLabel, "0");
    lv_obj_set_style_text_font(speedLabel, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(speedLabel, COL_WHITE, 0);
    lv_obj_align(speedLabel, LV_ALIGN_TOP_MID, 0, 75);

    speedUnitLabel = lv_label_create(dashboardScreen);
    lv_label_set_text(speedUnitLabel, "KM/H");
    lv_obj_set_style_text_font(speedUnitLabel, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(speedUnitLabel, COL_BRAND_PURPLE, 0);
    lv_obj_align(speedUnitLabel, LV_ALIGN_TOP_MID, 0, 130);

    static lv_point_t speedLineLeft[] = { {338,141}, {376,141}, {383,146}, {435,146} };
    static lv_point_t speedLineRight[] = { {589,146}, {641,146}, {648,141}, {686,141} };

    lv_obj_t* speedLineL = lv_line_create(dashboardScreen);
    lv_line_set_points(speedLineL, speedLineLeft, 4);
    lv_obj_set_style_line_width(speedLineL, 2, 0);
    lv_obj_set_style_line_color(speedLineL, COL_BRAND_PURPLE, 0);

    lv_obj_t* speedLineR = lv_line_create(dashboardScreen);
    lv_line_set_points(speedLineR, speedLineRight, 4);
    lv_obj_set_style_line_width(speedLineR, 2, 0);
    lv_obj_set_style_line_color(speedLineR, COL_BRAND_PURPLE, 0);

    lv_obj_clear_flag(speedLineL, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(speedLineR, LV_OBJ_FLAG_CLICKABLE);

    createRpmGauge();

    const int slotX[DASHBOARD_SLOT_COUNT] = { 20, 20, 20, 20, 754, 754, 754, 754 };
    const int slotY[DASHBOARD_SLOT_COUNT] = { 78, 187, 296, 405, 78, 187, 296, 405 };
    const int slotW[DASHBOARD_SLOT_COUNT] = { 250, 250, 250, 250, 250, 250, 250, 250 };
    const int slotH[DASHBOARD_SLOT_COUNT] = { 102, 102, 102, 102, 102, 102, 102, 102 };

    for (uint8_t i = 0; i < DASHBOARD_SLOT_COUNT; i++) {
        lv_obj_t* panel = createPanel(dashboardScreen, slotX[i], slotY[i], slotW[i], slotH[i]);

        dashboardPanelTitle[i] = createPanelTitle(panel, metricDisplayName(dashboardSlotMetric[i]));
        lv_obj_set_style_text_color(dashboardPanelTitle[i], metricDefs[dashboardSlotMetric[i]].color, 0);

        dashboardPanelValue[i] = createValueLabel(panel, "--", metricDefs[dashboardSlotMetric[i]].color);
        dashboardPanelUnit[i] = createPanelUnit(panel, metricUnit(dashboardSlotMetric[i]));

        dashboardPanelAccent[i] = lv_obj_create(panel);
        lv_obj_set_size(dashboardPanelAccent[i], 5, slotH[i] - 2);
        lv_obj_align(dashboardPanelAccent[i], LV_ALIGN_LEFT_MID, -14, 0);
        lv_obj_set_style_border_width(dashboardPanelAccent[i], 0, 0);
        lv_obj_set_style_radius(dashboardPanelAccent[i], 3, 0);
        lv_obj_clear_flag(dashboardPanelAccent[i], LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_clear_flag(dashboardPanelAccent[i], LV_OBJ_FLAG_CLICKABLE);

        dashboardPanelIcon[i] = lv_img_create(panel);
        lv_obj_add_flag(dashboardPanelIcon[i], LV_OBJ_FLAG_HIDDEN);
        updateDashboardPanelIcon(i, dashboardSlotMetric[i]);

        dashboardPanel[i] = panel;

        lv_obj_t* hotspot = lv_obj_create(dashboardScreen);
        lv_obj_remove_style_all(hotspot);
        lv_obj_set_pos(hotspot, slotX[i], slotY[i]);
        lv_obj_set_size(hotspot, slotW[i], slotH[i]);
        lv_obj_clear_flag(hotspot, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(hotspot, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(hotspot, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(hotspot, onDashboardSlotLongPress, LV_EVENT_CLICKED, (void*)(intptr_t)i);
        dashboardSlotHotspot[i] = hotspot;
        applyDashboardSlotVisibility(i);
        if (dashboardSlotHidden[i]) {
            lv_label_set_text(dashboardPanelTitle[i], uiLanguage == 0 ? "AUSGEBLENDET" : "HIDDEN");
            lv_obj_set_style_text_color(dashboardPanelTitle[i], COL_GREY, 0);
            lv_label_set_text(dashboardPanelValue[i], "");
            lv_label_set_text(dashboardPanelUnit[i], "");
            lv_obj_set_style_bg_color(dashboardPanelAccent[i], lv_color_hex(0x303743), 0);
        }

        lv_obj_add_event_cb(panel, onDashboardSlotLongPress, LV_EVENT_LONG_PRESSED, (void*)(intptr_t)i);
    }

    createCenterMetricCards();
    createStatusBar();
}

void onScanDeviceClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;

    int idx = (int)(intptr_t)lv_event_get_user_data(event);

    bleManualConnectIndex = (uint8_t)idx;
    bleManualConnectRequested = true;

    lv_label_set_text(scanStatusLabel, "Verbinde...");
}

void onRescanClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;

    bleShowingSelector = true;
    bleRescanRequested = true;
    lastReconnectAttempt = millis();

    for (uint8_t i = 0; i < MAX_SCAN_DEVICES; i++) { lv_obj_add_flag(scanListItemBtn[i], LV_OBJ_FLAG_HIDDEN); }

    lv_label_set_text(scanListItemLabel[0], "Scanne...");
    lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_CLICKABLE);

    lv_label_set_text(scanStatusLabel, "Scanne frisch... Vgate wird sofort verbunden");
}

void onSkipScanClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;

    bleShowingSelector = false;
    lv_disp_load_scr(dashboardScreen);
}

void populateScanList() {
    ScannedDevice snapshot[MAX_SCAN_DEVICES];
    uint8_t count = 0;

    if (!scanDataMutex || xSemaphoreTake(scanDataMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        if (scanStatusLabel) lv_label_set_text(scanStatusLabel, "BLE-Daten werden aktualisiert ...");
        return;
    }
    count = min((uint8_t)MAX_SCAN_DEVICES, scannedDeviceCount);
    for (uint8_t i = 0; i < count; i++) snapshot[i] = scannedDevices[i];
    xSemaphoreGive(scanDataMutex);

    if (count == 0) {
        lv_label_set_text(scanListItemLabel[0], "Keine Geraete gefunden");
        lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_CLICKABLE);
        for (uint8_t i = 1; i < MAX_SCAN_DEVICES; i++) lv_obj_add_flag(scanListItemBtn[i], LV_OBJ_FLAG_HIDDEN);
        return;
    }

    for (uint8_t i = 0; i < MAX_SCAN_DEVICES; i++) {
        if (i < count) {
            char label[96];
            snprintf(label, sizeof(label), "%s   [%s]%s",
                snapshot[i].name[0] ? snapshot[i].name : "(unbenannt)",
                snapshot[i].address,
                snapshot[i].hasOBDService ? "  *OBD*" : "");
            lv_label_set_text(scanListItemLabel[i], label);
            lv_obj_clear_flag(scanListItemBtn[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(scanListItemBtn[i], LV_OBJ_FLAG_CLICKABLE);
        } else {
            lv_obj_add_flag(scanListItemBtn[i], LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void populateScanListWithDiagnostics() {
    char buf[sizeof(bleDiagnosticDetail)];

    if (!scanDataMutex || xSemaphoreTake(scanDataMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        if (scanStatusLabel) lv_label_set_text(scanStatusLabel, "BLE-Diagnose wird aktualisiert ...");
        return;
    }
    strncpy(buf, bleDiagnosticDetail, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    xSemaphoreGive(scanDataMutex);

    if (buf[0] == '\0') {
        lv_label_set_text(scanListItemLabel[0], "Keine Diagnosedaten verfuegbar");
        lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_CLICKABLE);
        for (uint8_t i = 1; i < MAX_SCAN_DEVICES; i++) lv_obj_add_flag(scanListItemBtn[i], LV_OBJ_FLAG_HIDDEN);
        return;
    }

    uint8_t idx = 0;
    char* line = strtok(buf, "\n");
    while (line && idx < MAX_SCAN_DEVICES) {
        lv_label_set_text(scanListItemLabel[idx], line);
        lv_obj_clear_flag(scanListItemBtn[idx], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scanListItemBtn[idx], LV_OBJ_FLAG_CLICKABLE);
        idx++;
        line = strtok(NULL, "\n");
    }
    for (uint8_t i = idx; i < MAX_SCAN_DEVICES; i++) lv_obj_add_flag(scanListItemBtn[i], LV_OBJ_FLAG_HIDDEN);
}

void refreshScanSelectorUI() {
    if (ESP.getFreeHeap() < BLE_MIN_FREE_HEAP) {
        appLog("[UI] Skipping list rebuild, free heap too low (%u bytes)", (unsigned)ESP.getFreeHeap());
        lv_label_set_text(scanStatusLabel, "Zu wenig Speicher fuer Listenaufbau - bitte Geraet neu starten");
        return;
    }

    appLog(
        "[UI] Rebuilding scan list (%u devices, free heap: %u bytes)",
        (unsigned)scannedDeviceCount,
        (unsigned)ESP.getFreeHeap()
    );

    if (!scanDataMutex || xSemaphoreTake(scanDataMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
        lv_label_set_text(scanStatusLabel, "BLE-Daten werden aktualisiert ...");
        return;
    }
    const bool hasDetail = bleDiagnosticHasDetail;
    const uint8_t countSnapshot = scannedDeviceCount;
    char summaryCopy[sizeof(bleDiagnosticSummary)];
    strncpy(summaryCopy, bleDiagnosticSummary, sizeof(summaryCopy) - 1);
    summaryCopy[sizeof(summaryCopy) - 1] = '\0';
    xSemaphoreGive(scanDataMutex);

    if (hasDetail) {
        populateScanListWithDiagnostics();
        lv_label_set_text(scanStatusLabel, summaryCopy);
    } else {
        populateScanList();
        lv_label_set_text(
            scanStatusLabel,
            countSnapshot > 0
                ? "BLE-Geraete gefunden. Adapter waehlen:"
                : "Keine BLE-Geraete gefunden. Zuendung an? Adapter eingesteckt?"
        );
    }
}

void showScanSelector() {
    ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
    refreshScanSelectorUI();
    lvgl_port_unlock();
}

void onConnectionLabelClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;
    if (obdConnected) return;

    connectOverlayUserClosed = false;

    bleShowingSelector = true;
    refreshScanSelectorUI();
    lv_disp_load_scr(scanScreen);

    bleRescanRequested = true;
    lastReconnectAttempt = millis();
}

void createScanScreen() {
    scanScreen = lv_obj_create(NULL);
    setScreenBackground(scanScreen);

    lv_obj_t* title = lv_label_create(scanScreen);
    lv_label_set_text(title, "OBD ADAPTER");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COL_GREEN, 0);
    lv_obj_set_pos(title, 30, 20);

    scanStatusLabel = lv_label_create(scanScreen);
    lv_label_set_text(scanStatusLabel, "Scanne...");
    lv_obj_set_style_text_color(scanStatusLabel, COL_GREY, 0);
    lv_obj_set_pos(scanStatusLabel, 30, 58);

    scanListWidget = lv_list_create(scanScreen);
    lv_obj_set_size(scanListWidget, 964, 400);
    lv_obj_align(scanListWidget, LV_ALIGN_TOP_MID, 0, 95);
    lv_obj_set_style_bg_color(scanListWidget, COL_PANEL, 0);
    lv_obj_set_style_border_width(scanListWidget, 0, 0);
    lv_obj_set_style_radius(scanListWidget, 14, 0);

    for (uint8_t i = 0; i < MAX_SCAN_DEVICES; i++) {
        lv_obj_t* item = lv_list_add_btn(scanListWidget, LV_SYMBOL_BLUETOOTH, "");

        lv_obj_add_flag(item, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(item, onScanDeviceClicked, LV_EVENT_CLICKED, (void*)(intptr_t)i);

        scanListItemBtn[i] = item;

        lv_obj_t* label = nullptr;
        uint32_t childCount = lv_obj_get_child_cnt(item);

        for (uint32_t c = 0; c < childCount; c++) {
            lv_obj_t* child = lv_obj_get_child(item, c);

            if (lv_obj_check_type(child, &lv_label_class)) {
                label = child;
                break;
            }
        }

        scanListItemLabel[i] = label;

        if (label) { lv_label_set_long_mode(label, LV_LABEL_LONG_DOT); }
    }

    lv_obj_t* rescanBtn = lv_btn_create(scanScreen);
    lv_obj_set_size(rescanBtn, 300, 60);
    lv_obj_align(rescanBtn, LV_ALIGN_BOTTOM_LEFT, 30, -15);
    lv_obj_set_style_bg_color(rescanBtn, COL_BRAND_PURPLE, 0);
    lv_obj_set_style_bg_grad_color(rescanBtn, COL_BRAND_BLUE, 0);
    lv_obj_set_style_bg_grad_dir(rescanBtn, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_radius(rescanBtn, 12, 0);
    lv_obj_add_event_cb(rescanBtn, onRescanClicked, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* rescanLabel = lv_label_create(rescanBtn);
    lv_label_set_text(rescanLabel, "NEU SCANNEN");
    lv_obj_set_style_text_color(rescanLabel, COL_WHITE, 0);
    lv_obj_center(rescanLabel);

    lv_obj_t* skipBtn = lv_btn_create(scanScreen);
    lv_obj_set_size(skipBtn, 300, 60);
    lv_obj_align(skipBtn, LV_ALIGN_BOTTOM_RIGHT, -30, -15);
    lv_obj_set_style_bg_color(skipBtn, COL_PANEL2, 0);
    lv_obj_set_style_bg_grad_color(skipBtn, COL_PANEL, 0);
    lv_obj_set_style_bg_grad_dir(skipBtn, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_radius(skipBtn, 12, 0);
    lv_obj_add_event_cb(skipBtn, onSkipScanClicked, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* skipLabel = lv_label_create(skipBtn);
    lv_label_set_text(skipLabel, "SPAETER");
    lv_obj_set_style_text_color(skipLabel, COL_WHITE, 0);
    lv_obj_center(skipLabel);
}

void updateUI() {

    const int8_t connectionUiState = (obdConnected && elmInitialized) ? 2 : (obdConnected ? 1 : 0);

    if (connectionUiState != lastConnectionUiState && connectionUiState == 2) {
        lv_label_set_text(connectionStatusLabel, "OBD ONLINE");
        lv_obj_set_style_text_color(connectionStatusLabel, COL_GREEN, 0);

        lv_label_set_text(statusOBDLabel, "OBD-II     ONLINE");
        lv_label_set_text(statusBLELabel, "BLE        CONNECTED");

        lv_obj_set_style_text_color(statusOBDLabel, COL_GREEN, 0);
        lv_obj_set_style_text_color(statusBLELabel, COL_GREEN, 0);
    } else if (connectionUiState != lastConnectionUiState && connectionUiState == 1) {
        lv_label_set_text(connectionStatusLabel, "CONNECTING");
        lv_obj_set_style_text_color(connectionStatusLabel, COL_ORANGE, 0);

        lv_label_set_text(statusOBDLabel, "OBD-II     STARTING");
        lv_label_set_text(statusBLELabel, "BLE        CONNECTED");

        lv_obj_set_style_text_color(statusOBDLabel, COL_ORANGE, 0);
        lv_obj_set_style_text_color(statusBLELabel, COL_GREEN, 0);
    } else if (connectionUiState != lastConnectionUiState) {
        lv_label_set_text(connectionStatusLabel, "OBD OFFLINE");
        lv_obj_set_style_text_color(connectionStatusLabel, COL_RED, 0);

        lv_label_set_text(statusOBDLabel, "OBD-II     OFFLINE");
        lv_label_set_text(statusBLELabel, "BLE        OFFLINE");

        lv_obj_set_style_text_color(statusOBDLabel, COL_RED, 0);
        lv_obj_set_style_text_color(statusBLELabel, COL_RED, 0);
    }
    lastConnectionUiState = connectionUiState;

    {
        const ValueFreshness f = getMetricFreshness(METRIC_SPEED);
        if (f == VALUE_EXPIRED) {
            setLabelTextIfChanged(speedLabel, metricUnavailable(METRIC_SPEED) ? "N/A" : "--");
            if (lastSpeedFreshState != VALUE_EXPIRED) {
                lv_obj_set_style_text_color(speedLabel, COL_GREY, 0);
            }
            lastDisplayedSpeed = INT32_MIN;
        } else if (data.speedOK) {
            const int32_t displayedSpeed = lroundf(data.speed);
            if (displayedSpeed != lastDisplayedSpeed) {
                char buf[16]; snprintf(buf, sizeof(buf), "%ld", (long)displayedSpeed);
                setLabelTextIfChanged(speedLabel, buf); lastDisplayedSpeed = displayedSpeed;
            }
            if (lastSpeedFreshState != (int8_t)f) lv_obj_set_style_text_color(speedLabel, f == VALUE_STALE ? COL_GREY : COL_WHITE, 0);
        }
        lastSpeedFreshState = (int8_t)f;
    }

    {
        const ValueFreshness f = getMetricFreshness(METRIC_RPM);
        if (f == VALUE_EXPIRED) {
            setLabelTextIfChanged(rpmLabel, metricUnavailable(METRIC_RPM) ? "N/A" : "--");
            if (lastRpmFreshState != VALUE_EXPIRED) {
                lv_obj_set_style_text_color(rpmLabel, COL_GREY, 0);
                lv_obj_set_style_text_color(rpmUnitLabel, COL_GREY, 0);
                lv_obj_set_style_line_color(rpmMeter, COL_GREY, 0);
            }
            lastDisplayedRpm = INT32_MIN;
        } else if (data.rpmOK) {
            const int32_t displayedRpm = lroundf(data.rpm);
            if (displayedRpm != lastDisplayedRpm) {
                char buf[16]; snprintf(buf, sizeof(buf), "%ld", (long)displayedRpm);
                setLabelTextIfChanged(rpmLabel, buf); lastDisplayedRpm = displayedRpm;
            }

            const int8_t rpmColorBand = data.rpm >= 6500 ? 3 : data.rpm >= 5500 ? 2 : data.rpm >= 2500 ? 1 : 0;
            const lv_color_t normalColor = rpmColorBand == 3 ? COL_RED : rpmColorBand == 2 ? COL_ORANGE : rpmColorBand == 1 ? COL_GREEN : COL_BLUE;
            const lv_color_t shownColor = f == VALUE_STALE ? COL_GREY : normalColor;
            if (rpmColorBand != lastRpmColorBand || lastRpmFreshState != (int8_t)f) {
                lv_obj_set_style_text_color(rpmLabel, f == VALUE_STALE ? COL_GREY : COL_WHITE, 0);
                lv_obj_set_style_text_color(rpmUnitLabel, shownColor, 0);
                lv_obj_set_style_line_color(rpmMeter, shownColor, 0);
                lastRpmColorBand = rpmColorBand;
            }

            const float shownRpm = constrain(data.rpm, 0.0f, (float)RPM_MAX);
            const float needleDeg = 150.0f + 240.0f * shownRpm / RPM_MAX;
            const float needleRad = needleDeg * PI / 180.0f;
            const lv_coord_t needleX = (lv_coord_t)(RPM_CX + cosf(needleRad) * RPM_RX * 0.70f);
            const lv_coord_t needleY = (lv_coord_t)(RPM_CY + sinf(needleRad) * RPM_RY * 0.70f);
            if (needleX != lastNeedleX || needleY != lastNeedleY) {
                rpmNeedlePoints[1].x = needleX; rpmNeedlePoints[1].y = needleY;
                lv_line_set_points(rpmMeter, rpmNeedlePoints, 2);
                lastNeedleX = needleX; lastNeedleY = needleY;
            }
        }
        lastRpmFreshState = (int8_t)f;
    }

    {
        const ValueFreshness vf = pidFreshness(PID_VOLTAGE);
        char buf[32];
        if (pidTypeUnavailable(PID_VOLTAGE)) snprintf(buf, sizeof(buf), "VOLTAGE    N/A");
        else if (vf == VALUE_EXPIRED || !data.voltageOK) snprintf(buf, sizeof(buf), "VOLTAGE    --.- V");
        else snprintf(buf, sizeof(buf), "VOLTAGE    %.1f V", data.voltage);

        const bool voltageLabelChanged = setLabelTextIfChanged(statusVoltageLabel, buf);
        if (voltageLabelChanged || lastVoltageFreshState != (int8_t)vf) {
            const bool voltageAlert = vf == VALUE_FRESH && data.voltageOK && (data.voltage < 11.5f || data.voltage > 15.0f);
            lv_obj_set_style_text_color(statusVoltageLabel, voltageAlert ? COL_ORANGE : COL_GREY, 0);
            lastVoltageFreshState = (int8_t)vf;
        }
    }

    {
        char buf[32];
        snprintf(buf, sizeof(buf), "LATENCY    %u ms", (unsigned)lastPIDLatencyMs);

        if (setLabelTextIfChanged(statusLatencyLabel, buf)) {
            lv_obj_set_style_text_color(statusLatencyLabel, lastPIDLatencyMs > 150 ? COL_ORANGE : COL_GREY, 0);
        }
    }

    bool blinkPhase = (millis() / 500) % 2 == 0;

    for (uint8_t i = 0; i < DASHBOARD_SLOT_COUNT; i++) {
        if (dashboardSlotHidden[i]) continue;
        const unsigned long slotNow = millis();
        const uint32_t slotInterval = refreshHzToIntervalMs(dashboardSlotRefreshHz[i]);
        if (dashboardSlotLastUiUpdate[i] != 0 && slotNow - dashboardSlotLastUiUpdate[i] < slotInterval) continue;
        dashboardSlotLastUiUpdate[i] = slotNow;

        MetricId metric = dashboardSlotMetric[i];

        float v;
        bool haveValue = getMetricValue(metric, v);
        const ValueFreshness fresh = getMetricFreshness(metric);
        if (fresh == VALUE_EXPIRED) haveValue = false;

        if (haveValue) {
            char buf[24];
            formatMetricNumber(metric, v, buf, sizeof(buf));
            setLabelTextIfChanged(dashboardPanelValue[i], buf);
        } else {
            setLabelTextIfChanged(dashboardPanelValue[i], metricUnavailable(metric) ? "N/A" : "--");
        }

        if (dashboardFreshState[i] != (int8_t)fresh) {
            lv_obj_set_style_text_color(dashboardPanelValue[i], fresh == VALUE_FRESH ? COL_WHITE : COL_GREY, 0);
            dashboardFreshState[i] = (int8_t)fresh;
        }

        setLabelTextIfChanged(dashboardPanelUnit[i], metricUnit(metric));

        bool alert = haveValue && fresh == VALUE_FRESH && isMetricOutOfRange(metric, v);

        const bool alertStyleChanged = dashboardAlertState[i] != (alert ? 1 : 0);
        const bool alertBlinkChanged = alert && blinkPhase != lastAlertBlinkPhase;

        if (alert && (alertStyleChanged || alertBlinkChanged)) {
            lv_obj_set_style_border_width(dashboardPanel[i], 2, 0);
            lv_obj_set_style_border_color(dashboardPanel[i], COL_RED, 0);
            lv_obj_set_style_shadow_width(dashboardPanel[i], blinkPhase ? 14 : 7, 0);
            lv_obj_set_style_shadow_opa(dashboardPanel[i], blinkPhase ? LV_OPA_50 : LV_OPA_20, 0);
            lv_obj_set_style_shadow_color(dashboardPanel[i], COL_RED, 0);
        } else if (!alert && alertStyleChanged) {
            lv_obj_set_style_border_width(dashboardPanel[i], 1, 0);
            lv_obj_set_style_border_color(dashboardPanel[i], lv_color_hex(0x303743), 0);
            lv_obj_set_style_shadow_width(dashboardPanel[i], 8, 0);
            lv_obj_set_style_shadow_opa(dashboardPanel[i], LV_OPA_20, 0);
            lv_obj_set_style_shadow_color(dashboardPanel[i], lv_color_black(), 0);
        }

        dashboardAlertState[i] = alert ? 1 : 0;
    }
    lastAlertBlinkPhase = blinkPhase;

    const MetricId centerMetric[3] = { METRIC_THROTTLE, METRIC_LAMBDA, METRIC_MAF };

    for (uint8_t i = 0; i < 3; i++) {
        float value;
        const ValueFreshness fresh = getMetricFreshness(centerMetric[i]);
        if (centerMetricValue[i] && fresh != VALUE_EXPIRED && getMetricValue(centerMetric[i], value)) {
            char buf[20];
            formatMetricNumber(centerMetric[i], value, buf, sizeof(buf));
            setLabelTextIfChanged(centerMetricValue[i], buf);
            if (centerMetricFreshState[i] != (int8_t)fresh) {
                lv_obj_set_style_text_color(centerMetricValue[i], fresh == VALUE_STALE ? COL_GREY : COL_WHITE, 0);
                centerMetricFreshState[i] = (int8_t)fresh;
            }
        } else if (centerMetricValue[i]) {
            setLabelTextIfChanged(centerMetricValue[i], metricUnavailable(centerMetric[i]) ? "N/A" : "--");
            if (centerMetricFreshState[i] != VALUE_EXPIRED) {
                lv_obj_set_style_text_color(centerMetricValue[i], COL_GREY, 0);
                centerMetricFreshState[i] = VALUE_EXPIRED;
            }
        }
    }
}

void onConnectOverlayCloseClicked(lv_event_t* event) {
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) return;

    connectOverlayUserClosed = true;

    if (connectOverlay) { lv_obj_add_flag(connectOverlay, LV_OBJ_FLAG_HIDDEN); }
}

void createConnectOverlay() {
    connectOverlay = lv_obj_create(lv_layer_top());

    lv_obj_set_size(connectOverlay, 1024, 600);
    lv_obj_set_pos(connectOverlay, 0, 0);
    lv_obj_set_style_bg_color(connectOverlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(connectOverlay, LV_OPA_80, 0);
    lv_obj_set_style_border_width(connectOverlay, 0, 0);
    lv_obj_set_style_radius(connectOverlay, 0, 0);
    lv_obj_clear_flag(connectOverlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(connectOverlay, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t* panel = lv_obj_create(connectOverlay);
    lv_obj_set_size(panel, 700, 480);
    lv_obj_align(panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(panel, COL_PANEL, 0);
    lv_obj_set_style_bg_grad_color(panel, COL_PANEL2, 0);
    lv_obj_set_style_bg_grad_dir(panel, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_radius(panel, 18, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* title = lv_label_create(panel);
    lv_label_set_text(title, "VERBINDUNGSSTATUS");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COL_GREEN, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 20, 16);

    lv_obj_t* closeBtn = lv_btn_create(panel);
    lv_obj_set_size(closeBtn, 140, 44);
    lv_obj_align(closeBtn, LV_ALIGN_TOP_RIGHT, -16, 12);
    lv_obj_set_style_bg_color(closeBtn, COL_PANEL2, 0);
    lv_obj_set_style_radius(closeBtn, 10, 0);
    lv_obj_add_event_cb(closeBtn, onConnectOverlayCloseClicked, LV_EVENT_CLICKED, nullptr);

    lv_obj_t* closeLabel = lv_label_create(closeBtn);
    lv_label_set_text(closeLabel, "SCHLIESSEN");
    lv_obj_set_style_text_color(closeLabel, COL_WHITE, 0);
    lv_obj_center(closeLabel);

    lv_obj_t* container = lv_obj_create(panel);
    lv_obj_set_size(container, 660, 380);
    lv_obj_align(container, LV_ALIGN_BOTTOM_MID, 0, -16);
    lv_obj_set_style_bg_color(container, COL_BG, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_radius(container, 12, 0);
    lv_obj_set_style_pad_all(container, 14, 0);
    lv_obj_set_scroll_dir(container, LV_DIR_VER);

    connectStepsLabel = lv_label_create(container);
    lv_label_set_long_mode(connectStepsLabel, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(connectStepsLabel, 630);
    lv_obj_set_style_text_font(connectStepsLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(connectStepsLabel, COL_WHITE, 0);
    lv_label_set_text(connectStepsLabel, "");
}

void closeSplashTimer(lv_timer_t* timer) {
    if (splashOverlay) {
        lv_obj_del(splashOverlay);
        splashOverlay = nullptr;
        splashStatusLabel = nullptr;
    }
    lv_timer_del(timer);
}

void createSplashOverlay() {
    splashOverlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(splashOverlay, 1024, 600);
    lv_obj_set_pos(splashOverlay, 0, 0);
    lv_obj_set_style_bg_color(splashOverlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(splashOverlay, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(splashOverlay, 0, 0);
    lv_obj_set_style_radius(splashOverlay, 0, 0);
    lv_obj_set_style_pad_all(splashOverlay, 0, 0);
    lv_obj_clear_flag(splashOverlay, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* logo = lv_img_create(splashOverlay);
    lv_img_set_src(logo, &logo_img);

    lv_img_set_zoom(logo, 650);
    lv_obj_align(logo, LV_ALIGN_CENTER, 0, -55);

    splashStatusLabel = lv_label_create(splashOverlay);
    lv_label_set_text(splashStatusLabel, "Display ........ OK\nSettings ....... OK\nSystem ......... STARTING");
    lv_obj_set_style_text_font(splashStatusLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(splashStatusLabel, COL_GREY, 0);
    lv_obj_set_style_text_align(splashStatusLabel, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(splashStatusLabel, LV_ALIGN_BOTTOM_MID, 0, -38);

    lv_obj_t* version = lv_label_create(splashOverlay);
    lv_label_set_text(version, FW_VERSION);
    lv_obj_set_style_text_font(version, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(version, COL_BRAND_PURPLE, 0);
    lv_obj_align(version, LV_ALIGN_BOTTOM_RIGHT, -24, -18);

    lv_timer_create(closeSplashTimer, 5000, nullptr);
}

void bootSetStatus(const char* text) {
    if (!splashOverlay || !splashStatusLabel || !text) return;
    ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
    if (splashStatusLabel) lv_label_set_text(splashStatusLabel, text);
    lvgl_port_unlock();
}

bool initDisplay() {
    Serial.println("[BOOT] LCD: allocating Board");

    displayBoard = new Board();
    if (!displayBoard) {
        Serial.println("[BOOT][FATAL] LCD: Board allocation failed");
        return false;
    }

    Serial.println("[BOOT] LCD: Board::init()");
    displayBoard->init();

    Serial.printf("[BOOT] memory before Board::begin: internal=%u largest=%u psram=%u largestPSRAM=%u\n",
        (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
        (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));

    Serial.println("[BOOT] LCD: Board::begin()");
    if (!displayBoard->begin()) {
        Serial.println("[BOOT][FATAL] LCD: Board::begin() returned false");
        return false;
    }
    Serial.println("[BOOT] LCD: hardware begin OK");

    Serial.printf("[BOOT] memory before LVGL: internal=%u largest=%u psram=%u largestPSRAM=%u\n",
        (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
        (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));

    if (displayBoard->getIO_Expander()) {
        boardIOExpander = displayBoard->getIO_Expander()->getBase();
        if (boardIOExpander) boardIOExpander->pinMode(LIGHT_SENSE_EXIO, INPUT);
    }

    vehicleLightsOn = readVehicleLightInput();
    pendingLightState = vehicleLightsOn;

    printf("\n>>> APP TRACE A: BEFORE lvgl_port_init <<<\n");
    fflush(stdout);

    const bool lvglReady = lvgl_port_init(displayBoard->getLCD(), displayBoard->getTouch());

    printf("\n>>> APP TRACE B: AFTER lvgl_port_init result=%d <<<\n", lvglReady ? 1 : 0);
    fflush(stdout);
    if (!lvglReady) {
        Serial.println("[BOOT][FATAL] LVGL initialization failed");
        return false;
    }

    printf("\n>>> APP TRACE C: BEFORE lvgl_port_lock <<<\n");
    fflush(stdout);

    bool uiLockResult = lvgl_port_lock(-1);

    printf("\n>>> APP TRACE D: AFTER lvgl_port_lock result=%d <<<\n", uiLockResult ? 1 : 0);
    fflush(stdout);

    printf("\n>>> APP TRACE E: BEFORE UI CREATION <<<\n");
    fflush(stdout);

    Serial.println("[BOOT] LVGL: creating UI");
    Serial.println("[UI TRACE 03] before applyDisplayRotation");
    applyDisplayRotation();
    Serial.println("[UI TRACE 04] after applyDisplayRotation");
    Serial.println("[UI TRACE 05] before createDashboard");
    createDashboard();
    Serial.println("[UI TRACE 06] after createDashboard");
    Serial.println("[UI TRACE 07] before createScanScreen");
    createScanScreen();
    Serial.println("[UI TRACE 08] after createScanScreen");
    Serial.println("[UI TRACE 09] before createConnectOverlay");
    createConnectOverlay();
    Serial.println("[UI TRACE 10] after createConnectOverlay");
    Serial.println("[UI TRACE 11] before createMetricPicker");
    createMetricPicker();
    Serial.println("[UI TRACE 12] after createMetricPicker");
    Serial.println("[UI TRACE 13] before createOtaOverlay");
    createOtaOverlay();
    Serial.println("[UI TRACE 14] after createOtaOverlay");

    Serial.println("[UI TRACE 15] before lv_scr_load");
    lv_scr_load(dashboardScreen);
    Serial.println("[UI TRACE 16] after lv_scr_load");
    Serial.println("[UI TRACE 17] before createSplashOverlay");
    createSplashOverlay();
    Serial.println("[UI TRACE 18] after createSplashOverlay");
    Serial.println("[UI TRACE 19] before createBrightnessDimmer");
    createBrightnessDimmer();
    Serial.println("[UI TRACE 20] after createBrightnessDimmer");
    Serial.println("[UI TRACE 21] before applyDisplayBrightness");
    applyDisplayBrightness();
    Serial.println("[UI TRACE 22] after applyDisplayBrightness");

    Serial.println("[UI TRACE 23] before lvgl_port_unlock");

    lvgl_port_unlock();

    Serial.println("[UI TRACE 24] after lvgl_port_unlock");
    Serial.println("[LCD] Ready");
    return true;
}

void setup() {
    Serial.begin(115200);

    delay(1200);
    bootStartedAt = millis();

    Serial.println();
    Serial.println("==========================================");
    Serial.println(" SCHIEREN OPEL PERFORMANCE DISPLAY");
    Serial.println(" V21 RC11b");
    Serial.println(" Designed by Stefan Schieren");
    Serial.println("==========================================");
    Serial.printf("[BOOT] chip=%s rev=%u flash=%uMB psram=%uMB freeHeap=%u\n",
        ESP.getChipModel(), (unsigned)ESP.getChipRevision(),
        (unsigned)(ESP.getFlashChipSize() / (1024UL * 1024UL)),
        (unsigned)(ESP.getPsramSize() / (1024UL * 1024UL)),
        (unsigned)ESP.getFreeHeap());

    scanDataMutex = xSemaphoreCreateMutex();
    if (!scanDataMutex) Serial.println("[BOOT][WARN] scanDataMutex allocation failed");

    Serial.println("[BOOT] loading settings");
    loadSettings();
    Serial.println("[BOOT] settings OK");

    const bool displayReady = initDisplay();
    Serial.printf("[BOOT] displayReady=%d\n", displayReady ? 1 : 0);

    Serial.println("[BOOT] recording health");
    recordBootHealth();
    Serial.println("[BOOT] health OK");

    initProfessionalWatchdog();

    char bootStatus[220];
    snprintf(bootStatus, sizeof(bootStatus), "Display ........ OK\nTouch .......... OK\nSettings ....... OK (schema %u)\nHealth ......... crash streak %u\nSD ............. pruefe ...", CONFIG_SCHEMA_VERSION, crashStreak);
    bootSetStatus(bootStatus);

    if (sdLoggingEnabled) sdInitRequested = true;
    snprintf(bootStatus, sizeof(bootStatus), "Display ........ OK\nTouch .......... OK\nSettings ....... OK (schema %u)\nHealth ......... crash streak %u\nSD ............. %s", CONFIG_SCHEMA_VERSION, crashStreak, sdLoggingEnabled ? "wird spaeter initialisiert" : "Logging AUS");
    bootSetStatus(bootStatus);

    WiFi.mode(WIFI_OFF);

    Serial.println("[BLE] Initializing...");

    BLEDevice::init("ASTRA-OPC");

    BLEScan* scan = BLEDevice::getScan();
    scan->setActiveScan(true);
    scan->setInterval(100);
    scan->setWindow(80);
    scan->setAdvertisedDeviceCallbacks(&obdAdvertisedDeviceCallbacks, false);

    lastReconnectAttempt = millis();
    bleColdStartNotBefore = millis() + BLE_ADAPTER_WARMUP_MS;

    appLog("[SYSTEM] Ready, free heap: %u bytes", (unsigned)ESP.getFreeHeap());
}

void loop() {
    unsigned long now = millis();
    if (professionalWatchdogActive) esp_task_wdt_reset();
    const uint32_t freeHeapNow = ESP.getFreeHeap();
    if (freeHeapNow < minFreeHeapSeen) minFreeHeapSeen = freeHeapNow;
    tuneAdaptiveScheduler(now);
    markHealthyBootIfReady();

    serviceOtaMode();

    if (otaModeActive || otaStartRequested) {
        if (now - lastUIUpdate >= 250) {
            lastUIUpdate = now;
            ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
            updateUI();
            lvgl_port_unlock();
        }
        delay(2);
        return;
    }

    if (automaticLightMode && now - lastLightInputPollAt >= 250) {
        lastLightInputPollAt = now;

        const bool sampledLightState = readVehicleLightInput();

        if (sampledLightState != pendingLightState) {
            pendingLightState = sampledLightState;
            lightStateChangedAt = now;
        } else if (sampledLightState != vehicleLightsOn && now - lightStateChangedAt >= 250) {
            vehicleLightsOn = sampledLightState;
            Serial.println("[UI TRACE 21] before applyDisplayBrightness");
    applyDisplayBrightness();
    Serial.println("[UI TRACE 22] after applyDisplayBrightness");
        }
    }

    if (bleDisconnectedEvent) {
        bleDisconnectedEvent = false;

        appLog("[SYSTEM] BLE connection lost");
        invalidateAllPidFreshness();
        resetSchedulerHealth();

        disconnectOBD();
        elmState = ELM_IDLE;
    }

    if (bleRescanRequested && !obdConnected && !bleScanInProgress) {
        bleRescanRequested = false;
        startBleScanAsync();
    }

    if (bleManualConnectRequested) {
        bleManualConnectRequested = false;

        bool ok = connectToScannedDevice(bleManualConnectIndex);

        lastReconnectAttempt = millis();

        ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);

        if (ok) {
            bleShowingSelector = false;
            lv_disp_load_scr(dashboardScreen);
            lvgl_port_unlock();

            startELMInitialization();
        } else {
            lv_label_set_text(
                scanStatusLabel,
                bleDiagnosticSummary[0] ? bleDiagnosticSummary : "Verbindung fehlgeschlagen - anderes Geraet waehlen"
            );

            if (bleDiagnosticHasDetail) { populateScanListWithDiagnostics(); }

            lvgl_port_unlock();
        }
    }

    if (bleScanInProgress && now - bleScanStartedAt > (BLE_SCAN_SECONDS * 1000UL + 4000UL)) {
        appLog("[BLE] Scan watchdog: no completion callback (free heap: %u bytes)", (unsigned)ESP.getFreeHeap());

        bleScanInProgress = false;

        ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);

        for (uint8_t i = 0; i < MAX_SCAN_DEVICES; i++) { lv_obj_add_flag(scanListItemBtn[i], LV_OBJ_FLAG_HIDDEN); }

        lv_label_set_text(scanListItemLabel[0], "Scan fehlgeschlagen (Timeout)");
        lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scanListItemBtn[0], LV_OBJ_FLAG_CLICKABLE);

        lv_label_set_text(scanStatusLabel, "Scan-Timeout. Bitte NEU SCANNEN druecken.");

        lvgl_port_unlock();
    }

    if (bleEarlyCandidateFound && !bleScanInProgress) {
        bleEarlyCandidateFound = false;

        BLEAdvertisedDevice* device = bleEarlyCandidateDevice;
        bleEarlyCandidateDevice = nullptr;

        if (device) {
            bool ok = bleAttemptConnect(device);

            delete device;

            lastReconnectAttempt = millis();

            ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);

            if (ok) {
                bleShowingSelector = false;
                lv_disp_load_scr(dashboardScreen);
                lvgl_port_unlock();

                startELMInitialization();
            } else {
                lv_label_set_text(
                    scanStatusLabel,
                    bleDiagnosticSummary[0] ? bleDiagnosticSummary : "Verbindung fehlgeschlagen - anderes Geraet waehlen"
                );

                if (bleDiagnosticHasDetail) { populateScanListWithDiagnostics(); }

                lvgl_port_unlock();
            }
        }
    }

    if (bleScanComplete) {
        bleScanComplete = false;

        if (processScanResults()) {
            startELMInitialization();
        }
    }

    static bool lowHeapWarned = false;
    bool heapLow = ESP.getFreeHeap() < BLE_MIN_FREE_HEAP;

    if (heapLow && !lowHeapWarned) {
        lowHeapWarned = true;
        appLog(
            "[SYSTEM] Free heap low (%u bytes) - slowing reconnect retries, not stopping them.",
            (unsigned)ESP.getFreeHeap()
        );
    } else if (!heapLow) {
        lowHeapWarned = false;
    }

    unsigned long retryIntervalMs = heapLow ? (BLE_RECONNECT_DELAY_MS * 4) : BLE_RECONNECT_DELAY_MS;

    if (!obdConnected && !bleShowingSelector && !bleScanInProgress &&
        (int32_t)(now - bleColdStartNotBefore) >= 0 &&
        now - lastReconnectAttempt >= retryIntervalMs) {
        lastReconnectAttempt = now;
        startBleScanAsync();
    }

    if (obdConnected && !elmInitialized) { processELMInitialization(); }
    serviceDiagnosticMode();
    if (obdConnected && elmInitialized && !diagnosticModeActive) {
        if (pidSupportQueryState != PID_SUPPORT_DONE) processPidSupportQuery();
        else if (vinQueryState != VIN_DONE) processVinQuery();
        else processOBD();
    }
    serviceObdDataHealth(now);

    if (now - lastUIUpdate >= UI_UPDATE_INTERVAL_MS) {
        lastUIUpdate = now;

        ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
        updateUI();
        lvgl_port_unlock();
    }

    if (connectOverlayCloseAt != 0 && now >= connectOverlayCloseAt) {
        connectOverlayCloseAt = 0;

        ESP_LOGI("MAINTRACE", "MAIN C: before lvgl_port_lock");
    bool uiLockResult = lvgl_port_lock(-1);
    ESP_LOGI("MAINTRACE", "MAIN D: after lvgl_port_lock, result=%d", uiLockResult ? 1 : 0);
        if (connectOverlay) { lv_obj_add_flag(connectOverlay, LV_OBJ_FLAG_HIDDEN); }
        lvgl_port_unlock();
    }

    serviceDeferredPersistence();
    serviceSdLifecycle();
    enqueueSdLogRecord();
    serviceSdLogging();

    delay(2);
}
