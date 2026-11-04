# Project Zen (rev2 PCB) – RP2040 Pro Micro

Zen rev2 PCB with the ATmega32u4 Pro Micro replaced by an RP2040 board in the Pro Micro footprint
(pins: D3→GP0 serial, D1/D0→GP2/GP3 OLED I2C, D4/D2→GP4/GP1 encoder, B6→GP21 WS2812).

Build: `keyboards/zen2040/build.sh` → `zen2040_left.uf2`, `zen2040_right.uf2`.

Flash each half separately (TRRS unplugged): double-tap RESET or hold BOOT while plugging USB,
copy the matching .uf2 to the RPI-RP2 drive. Handedness is stored in EEPROM, so after that either
half can be the USB side. Later keymap-only updates can use the same files.

## TODO: OLED I2C
The OLED currently answers with SDA on GP3 / SCL on GP2 (swapped), so `oled_sw_i2c.c` bit-bangs it.
After fixing the jumpers on the PCB (SDA→GP2, SCL→GP3), go back to hardware I2C1: remove `oled_sw_i2c.c`,
`OLED_TRANSPORT = custom` and the CFLAGS line in `rules.mk`; set `I2C_DRIVER I2CD1`, `I2C1_SDA_PIN GP2`,
`I2C1_SCL_PIN GP3` in `config.h`; add `halconf.h` (`HAL_USE_I2C TRUE`) and `mcuconf.h` (`RP_I2C_USE_I2C1 TRUE`).

## Dokumentacja
- [MAPA_KLAWISZY.md](MAPA_KLAWISZY.md) – ściąga: warstwy, RGB, enkodery, OLED, wgrywanie
- [DOKUMENTACJA.md](DOKUMENTACJA.md) – sprzęt, piny, funkcje, kompilacja, rozwiązywanie problemów
