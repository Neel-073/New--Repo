# Gesture-Controlled Car Using ESP8266 and MPU6050 🚗  

## Overview  
This project demonstrates a **gesture-controlled car** that operates using **ESP8266 modules communicating via ESP-NOW**. The system consists of:  
- A **transmitter module** (ESP8266 + MPU6050) that detects hand gestures  
- A **receiver module** (ESP8266 + L298N + DC Motors) that moves the car accordingly  

## Features  
✅ **Wireless communication** using ESP-NOW  
✅ **Real-time gesture control** with MPU6050  
✅ **Motor control via L298N driver**  
✅ **Low latency & efficient response**  


## RECEIVER CONNECTIONS:

**BACK_RIGHT_MOTOR:**

  IN1 → D0 (GPIO16) 
  IN2 → D1 (GPIO5) 
  EN → D2 (GPIO4) 

**BACK_LEFT_MOTOR:** 

  IN1 → D3 (GPIO0) 
  IN2 → D4 (GPIO2) 
  EN → D5 (GPIO14) 

**FRONT_RIGHT_MOTOR:**

  IN1 → D6 (GPIO12) 
  IN2 → D7 (GPIO13) 
  EN → D8 (GPIO15) 

**FRONT_LEFT_MOTOR:**

  IN1 → D3 (GPIO0) 
  IN2 → D4 (GPIO2) 
  EN → D5 (GPIO14) 

## TRANSMITTER CONNECTIONS:
MPU6050 Pin-->ESP8266 Pin

VCC --> 3V3 (3.3V)

GND --> GND

SDA --> D2 (GPIO 4)

SCL --> D1 (GPIO 5)




