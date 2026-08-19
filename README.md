# Face Recognition Attendance System

A simple attendance management system using **face recognition** and **ESP32-CAM**. The project combines a web application with IoT devices to automate employee check-in/check-out and door access.

## Features

* Face recognition for employee attendance
* Employee registration and management
* Check-in / Check-out tracking
* Attendance history and working-time calculation
* ESP32-CAM for image capture
* ESP32 with Servo Motor for door control
* LCD display for attendance status
* Communication between the server and ESP32 via WebSocket/TCP

## System Overview

```text
ESP32-CAM
    │
    │ Image / WebSocket
    ▼
Spring Boot Application
    │
    ├── Face Recognition
    ├── Attendance Management
    ├── Employee Management
    └── Door Control
    │
    ▼
   MySQL
    │
    ▼
ESP32 ─── Servo Motor
  │
  └── LCD Display
```

## Technologies

* **Backend:** Java, Spring Boot, Spring Data JPA
* **Database:** MySQL
* **Frontend:** HTML, CSS, JavaScript, Bootstrap
* **IoT:** ESP32, ESP32-CAM, Servo Motor, LCD I2C
* **Communication:** REST API, WebSocket, TCP Socket
* **Face Recognition:** Python / Dlib

## Project Structure

```text
face-recognition/
├── attendance/              # Spring Boot application
├── CameraWebServer3/        # ESP32-CAM source code
├── servo_lcd_end.ino        # ESP32 Servo + LCD
└── README.md
```

## Notes

This project was developed mainly for **learning and academic purposes**, focusing on the integration of face recognition, web applications, databases, and IoT devices.

For real-world deployment, authentication, password protection, face anti-spoofing, and secure communication should be improved.
