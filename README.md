# Secure ESP32 LED Control via Flask Web Interface

## Overview

This project provides a secure web interface to control an LED connected to an ESP32 microcontroller over a local Wi-Fi network. Communication between the Flask web server and the ESP32 is encrypted using the **Ascon authenticated encryption algorithm**, ensuring confidentiality and integrity of control commands.

The system includes:
- A **Flask-based web dashboard** for user authentication and LED control.
- An **ESP32 firmware** that hosts a lightweight HTTP server, decrypts incoming commands, and toggles an onboard LED.
- End-to-end **encryption using Ascon-128**, a lightweight authenticated cipher ideal for embedded systems.

---

## Features

- 🔒 **Secure communication**: All LED control commands are encrypted with Ascon before transmission.
- 🔑 **User authentication**: Web interface requires password login (`adminadmin`) before allowing device interaction.
- 💡 **Simple hardware control**: Toggle an LED on/off via intuitive web buttons.
- 🌐 **Local network operation**: Designed for secure intra-network use (no internet exposure by default).
- ⚡ **Lightweight & efficient**: Optimized for resource-constrained ESP32 devices.

---
