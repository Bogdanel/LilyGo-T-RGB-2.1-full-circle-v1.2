<h1 align = "center">🌟LilyGO T-RGB🌟</h1>


[![Build PlatformIO](https://github.com/Xinyuan-LilyGO/LilyGo-T-RGB/actions/workflows/pio.yml/badge.svg)](https://github.com/Xinyuan-LilyGO/LilyGo-T-RGB/actions/workflows/pio.yml)
[![Arduino_CI](https://github.com/Xinyuan-LilyGO/LilyGo-T-RGB/actions/workflows/arduino_ci.yml/badge.svg)](https://github.com/Xinyuan-LilyGO/LilyGo-T-RGB/actions/workflows/arduino_ci.yml)


## 1️⃣Support Product

| Product(PinMap)                 | SOC        | Flash | PSRAM    | Resolution | Size     |
| ------------------------------- | ---------- | ----- | -------- | ---------- | -------- |
| [T-RGB 2.1 inch Full Circle][2] | ESP32-S3R8 | 16MB  | 8MB(OPI) | 480x480    | 2.1 Inch |

| Product(PinMap)                 | Display Driver | Touch Driver |
| ------------------------------- | -------------- | ------------ |
| [T-RGB 2.1 inch Full Circle][2] | ST7701S        | CST820       |

[1]: https://www.lilygo.cc/products/t-rgb?variant=42407295877301
[2]: https://www.lilygo.cc/products/t-rgb
[3]: https://www.lilygo.cc/products/t-rgb?variant=42880799441077

## 2️⃣Use reference

|    Product    |                          Youtube  link                           |                         explanation                         | Status |
| :-----------: | :--------------------------------------------------------------: | :---------------------------------------------------------: | :----: |
|    T -RGB     | [Youtube link](https://www.youtube.com/watch?v=hndA0D3F3ug&t=3s) | How to quickly port to LILYGO T-RGB using Squareline Studio |   ✅    |
| LilyGo device |   [Youtube link](https://www.youtube.com/watch?v=f3BybP3L7ls)    |       How to enter boot mode to download the firmware       |   ✅    |


* **BatteryVoltage** When USB is plugged in, the battery voltage cannot be read because the battery voltage is equal to the charging voltage at this time

## 4️⃣ PlatformIO Quick Start (Recommended)

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `LilyGo T-RGB` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click on the `platformio.ini` file, and in the `platformio` column
7. Uncomment one of the lines `src_dir = xxxx` to make sure only one line works
8. Click the (✔) symbol in the lower left corner to compile
9. Connect the board to the computer USB
10. Click (→) to upload firmware
11. Click (plug symbol) to monitor serial output
12. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

## 🔟 Resource

* [schematic](./schematic/T-RGB.pdf)
* [Dimensions](./dimensions/T-RGB-PCB.DXF)
* [PCB 3D](./dimensions/T-RGB-FULL-3D.stp)

## 11 T-RGB datasheet

* [ST7701S Driver datasheet](./datasheet/ST7701S_SPEC_%20V1.1.pdf)
* [ESP32S3-R8](https://www.espressif.com.cn/en/support/documents/technical-documents?keys=&field_type_tid%5B%5D=842)

# 12 Depends on required libraries

* [SensorLib v0.2.3](https://github.com/lewisxhe/SensorsLib)
* [lvgl v8.4.0 or lvgl v9.x.x](https://github.com/lvgl/lvgl)
