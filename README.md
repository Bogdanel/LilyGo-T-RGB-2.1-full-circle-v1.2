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

## 5️⃣ Install from Arduino Library Manager (recommended)

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install [Arduino ESP32 V 2.0.5 or above and below V3.0](https://docs.espressif.com/projects/arduino-esp32/en/latest/) 
3. `Sketch` -> `Include Library` -> `Manage Libraries`
4. `Library Search` -> `LilyGo T-RGB` -> `Install` -> `Install ALL`
5. `Library Search` -> `lvgl` -> `v8.3.11` -> `Install`
5. `Library Search` -> `SensorLib` -> `v0.2.3` -> `Install`
6. `File` -> `Examples` -> `LilyGo T-RGB` -> `Any Examples`
7. `Tools` , Make your selection according to the table below
    | Arduino IDE Setting                  | Value                             |
    | ------------------------------------ | --------------------------------- |
    | Board                                | **ESP32S3 Dev Module**            |
    | Port                                 | Your port                         |
    | USB CDC On Boot                      | Enable                            |
    | CPU Frequency                        | 240MHZ(WiFi)                      |
    | Core Debug Level                     | None                              |
    | USB DFU On Boot                      | Disable                           |
    | Erase All Flash Before Sketch Upload | Disable                           |
    | Events Run On                        | Core1                             |
    | Flash Mode                           | QIO 80MHZ                         |
    | Flash Size                           | **16MB(128Mb)**                   |
    | Arduino Runs On                      | Core1                             |
    | USB Firmware MSC On Boot             | Disable                           |
    | Partition Scheme                     | **16M Flash(3M APP/9.9MB FATFS)** |
    | PSRAM                                | **OPI PSRAM**                     |
    | Upload Mode                          | **UART0/Hardware CDC**            |
    | Upload Speed                         | 921600                            |
    | USB Mode                             | **CDC and JTAG**                  |
    * The options in bold are required, others are selected according to actual conditions.

8. Select `Port` ， select you board port
9. Click `upload` , Wait for compilation and writing to complete
10. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

    **Note that T-RGB depends on lvgl v8.3.11 version. Currently lvgl has released lvgl v9.0 version. When compilation error occurs, please check whether lvgl belongs to v8.3.11**

## 8️⃣ FAQ

1. The board uses USB as the JTAG upload port. When printing serial port information on USB_CDC_ON_BOOT configuration needs to be turned on.
If the port cannot be found when uploading the program or the USB has been used for other functions, the port does not appear.
Please enter the upload mode manually.
   1. Connect the board via the USB cable
   2. Press and hold the BOOT button , While still pressing the BOOT button, press RST
   3. Release the RST
   4. Release the BOOT button
   5. Upload sketch

2. If the above is invalid, burn the [binary file](./firmware/README.MD)  to check whether the hardware is normal
3. If you use external power supply instead of USB-C, please turn off the CDC option. This is because the board will wait for USB access when it starts.
   * For Arduino IDE users, it can be turned off in the options , Please note that turning off USB CDC will turn off Serial redirection to USB-C. At this time, you will not see any Serial message output when opening the port from USB-C, but output from GPIO43 and GPIO44.

    ```c
    Tools -> USB CDC On Boot -> Disable
    ```

   * For Platformio users, you can add the following compilation flags in the ini file

    ```c
    build_flags =
        ; Enable -DARDUINO_USB_CDC_ON_BOOT will start printing and wait for terminal access during startup
        ; -DARDUINO_USB_CDC_ON_BOOT=1

        ; Enable -UARDUINO_USB_CDC_ON_BOOT will turn off printing and will not block when using the battery
        -UARDUINO_USB_CDC_ON_BOOT
    ```
4. When USB is plugged in, the battery voltage cannot be read because the battery voltage is equal to the charging voltage at this time
5. Regarding the esp-idf version example, the latest version is not ready, the previous test version can be found [here](https://github.com/Xinyuan-LilyGO/T-RGB/releases/tag/v0.0.1)
6. The Grove port has been set to I2C, sharing the bus with the touch and cannot be used for other purposes.
7. T-RGB has no free GPIO and cannot be expanded
8. T-RGB hardware connects to RGB666 screens, but ESP32 currently only supports RGB565
9. `Serial.println` has no output. Please refer to the third step of the FAQ to set `USB CDC On Boot` to **Enable**.

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
