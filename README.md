# Tamagotchi-LPC1768 - Interactive Virtual *Totoro*

## Overview
This project was developed for the course *Computer architectures* at Politecnico di Torino. Tamagotchi-LPC1768 is a virtual pet simulation designed to run on the LPC1768 microcontroller and the LANDTIGER Board. It provides an interactive experience where users can engage with a digital pet through a touchscreen interface. The project utilizes Keil µVision for development and debugging, allowing for precise emulation of the hardware environment.

The virtual pet featured in this project is inspired by *Totoro*, the beloved character from *Studio Ghibli*, bringing a charming and recognizable touch to the simulation.

## Features
- **Keil µVision Emulation**: Uses the LANDTIGER emulator (or real hardware) for implementation.
- **Touchscreen Integration**: Implements a touchscreen interface using the TouchPanel library.
- **Calibration Mechanism**: Includes a calibration process to improve coordinate accuracy and reduce distortions.
- **Improved Interactivity**: Enhancements in animations and user interactions based on the touch input.

## Project Structure
- `tamagotchi_lpc1768_board/` - Contains the source code and project files.
- `Application_Note.pdf` - Detailed technical documentation of the implementation.
- `Project_Documentation.pdf` - Project description and requirements.

## Setup & Usage
1. Open the project in Keil µVision.
2. Ensure the LANDTIGER board or emulator is configured.
3. Run the calibration function `TouchPanel_Calibrate()` for accurate touch input.
4. Deploy and test the virtual pet application.

## Requirements
- Keil µVision IDE
- LPC1768 / LANDTIGER Board (or emulator)

## Notes
This project explores embedded system development and user interaction through a touchscreen interface, providing an engaging experience with a digital pet.
