# ESP32-OBD-Display von ihmSelbst

Ich habe das Display für meinen Astra J OPC gebaut. Auf dem 5-Zoll-Touchscreen sehe ich Drehzahl, Geschwindigkeit, Ladedruck und Temperaturen auf einen Blick. Die Daten kommen über einen BLE-Adapter an der OBD-II-Buchse.

## Was auf dem Display läuft

- Drehzahlmesser, Geschwindigkeitsanzeige und konfigurierbare Kacheln für Temperatur, Last, Ladedruck, Verbrauch und weitere Messwerte.
- BLE-Adaptersuche und automatische Wiederverbindung. Die Firmware lernt, welche PIDs das Fahrzeug liefert, und fragt fehlende Werte seltener oder gar nicht mehr ab.
- Opel-Fahrzeugprofile mit automatischer Erkennung über die Fahrgestellnummer und manueller Auswahl.
- OBD-Fehlercodes lesen und nach Bestätigung löschen. Messwerte lassen sich auf microSD protokollieren.
- Firmware-Update im lokalen WLAN des Displays. Das Display zeigt die Zugangsdaten für die jeweilige Sitzung an.

## Aufbau

| Teil | Ausführung |
| --- | --- |
| Display | Waveshare ESP32-S3-Touch-LCD-5B, 1024 × 600, GT911 |
| ESP32-S3 | 16 MB Flash, 8 MB PSRAM |
| OBD-Adapter | Vgate iCar Pro 2S BLE oder ein kompatibler ELM327-BLE-Adapter |

Die verfügbaren Messwerte hängen vom Fahrzeug und vom Adapter ab. Die Anleitung zum Kompilieren und Flashen steht im [Sketch-Ordner](ESP32_OBD_Display/README.md).

## Lizenz

Der eigene Projektcode steht unter der [MIT-Lizenz](LICENSE). Die mitgelieferten Konfigurations- und Portierungsdateien behalten ihre jeweiligen Lizenzen.

| Dateien | Herkunft und Lizenz |
| --- | --- |
| `esp_panel_board_supported_conf.h` | Espressif Systems (Shanghai), [Apache 2.0](LICENSE-Apache-2.0.txt), für das Waveshare 5B angepasst |
| `lvgl_v8_port.cpp`, `lvgl_v8_port.h` | Espressif Systems (Shanghai), [CC0 1.0](LICENSE-CC0-1.0.txt) |
| `lv_conf.h` | Angepasste LVGL-Konfiguration, [MIT · LVGL Kft](LICENSE-LVGL.txt) |
