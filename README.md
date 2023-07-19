# RFID-based Attendance System

This repository contains the source code and documentation for an RFID-based attendance system using NodeMCU ESP8266, 1.8" TFT display, RDM6300 RFID module, DS3231 RTC module, SD card sensor, and more.

## Table of Contents

- [Introduction](#introduction)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The RFID-based attendance system is a project designed to automate attendance tracking using radio frequency identification (RFID) technology. It utilizes various hardware components to create a complete system capable of accurately recording attendance information.

This project utilizes the following components:
- NodeMCU ESP8266: A popular development board based on the ESP8266 Wi-Fi module.
- 1.8" TFT Display: A small color display used for visual feedback and user interaction.
- RDM6300 RFID Module: An RFID reader module that communicates with RFID cards or tags.
- DS3231 RTC Module: A real-time clock module used to maintain accurate time for attendance logging.
- SD Card Sensor: A sensor for reading and writing data to an SD card.

## Hardware Requirements

To replicate this project, you will need the following hardware components:

- NodeMCU ESP8266
- 1.8" TFT Display
- RDM6300 RFID Module
- DS3231 RTC Module
- SD Card Sensor
- Breadboard and jumper wires
- RFID cards or tags (compatible with RDM6300 module)

Ensure that you have all the required hardware components before proceeding with the installation and usage steps.

## Software Requirements

The software requirements for this project are as follows:

- Arduino IDE: The Arduino development environment used to program the NodeMCU ESP8266 board.
- Required Libraries: Make sure to install the necessary libraries for each component, such as the TFT display library, RTC library, and RFID library.

## Installation

To install and set up the RFID-based attendance system, follow these steps:

1. Clone or download this repository to your local machine:

```bash
git clone https://github.com/avrojit-ovi/RFID-based-attendance-system.git
```

2. Open the Arduino IDE and install any required libraries mentioned in the project's documentation.
3. Connect the hardware components as per the circuit diagram provided in the project.
4. Connect the NodeMCU ESP8266 board to your computer via USB.
5. Open the `attendance_system.ino` file in the Arduino IDE.
6. Select the appropriate board (NodeMCU ESP8266) and port in the Arduino IDE.
7. Upload the code to the NodeMCU board.
8. Once the code is successfully uploaded, open the serial monitor to see the system's output.
9. Follow the instructions provided in the project documentation to use the RFID-based attendance system effectively.

## Usage

To use the RFID-based attendance system, follow these guidelines:

1. Ensure that the system is properly set up and powered.
2. The TFT display will show relevant information and instructions.
3. When a user scans an RFID card or tag, the system will read the card's information.
4. The system will compare the scanned card information with the stored database to identify the user.
5. If the user is recognized, the system will log their attendance and display appropriate messages on the TFT display.
6. The attendance data can be stored on the SD card for further analysis or retrieval.

Please refer to the project's documentation or comments within the source code for more detailed usage instructions.

## Contributing

Contributions to this project are welcome! If you find any issues or have ideas for improvements, please submit a pull request or open an issue on the GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use and modify the code as per the terms of the license.
