# Kompilieren und flashen

1. In der Arduino IDE das ESP32-Boardpaket von Espressif und die Bibliotheken **LVGL 8.4.x** sowie **ESP32_Display_Panel** mit den angebotenen Abhängigkeiten installieren.
2. [ESP32_OBD_Display.ino](ESP32_OBD_Display.ino) öffnen. Die Dateien in diesem Ordner gehören zusammen. `esp_panel_board_supported_conf.h` wählt das Waveshare 5B, `lv_conf.h` enthält die passende LVGL-Konfiguration.
3. Als Board **ESP32S3 Dev Module** wählen. Die Einstellungen aus dem bisherigen Build verwenden **16 MB Flash**, **QIO**, **OPI PSRAM**, **240 MHz** und das Partitionsschema `app3M_fat9M_16MB`. Für OTA braucht die Partitionstabelle zwei ausreichend große App-Slots.
4. Die erste Installation per USB flashen. Danach den BLE-Adapter am Display auswählen. Für spätere Updates den OTA-Modus am Display öffnen und im angezeigten WLAN die exportierte Anwendungsdatei `.bin` hochladen.

Quellstand **V21-RC11b**, normale Bildschirmvariante. Dieser Veröffentlichungsstand wurde noch nicht neu für das Board kompiliert.
