# SmartSpecs

## 📌 Project Description
SmartSpecs is a low-cost assistive system designed to help visually impaired individuals navigate safely. It uses an ESP32-CAM to detect obstacles in real-time and provides voice feedback through a smartphone.

👉 In simple terms: Camera sees → ESP32 processes → Phone speaks

---

## ⚙️ Technologies Used
- ESP32-CAM
- Arduino IDE
- Embedded C
- WiFi (Web Server)
- Browser Speech API

---

## 🔄 System Workflow

### 📷 Image Capture
- ESP32-CAM continuously captures images
- Uses low resolution for faster processing

### 🧠 Image Processing
- Each pixel is analyzed
- Brightness is calculated from RGB values

### 🚧 Obstacle Detection
- Dark pixels are counted
- If dark pixels exceed a threshold → "Obstacle Ahead"
- Else → "Path Clear"

### 🌐 Data Transmission
- ESP32 runs a web server
- Sends results via WiFi

### 📱 Phone Interaction
- User opens a webpage
- Touch input activates system

### 🔊 Voice Output
- Smartphone converts text to speech
- Uses browser speech API

---

## 🎯 Objectives
- Improve mobility for visually impaired users
- Provide real-time obstacle alerts
- Develop a low-cost and accessible solution

---

## ⭐ Key Features
- Real-time obstacle detection
- Wireless communication
- No mobile app required
- Easy interaction (touch-based)
- Low-cost hardware design

---

## 🔍 Why ESP32-CAM?
- Built-in camera module
- WiFi capability
- Compact size
- Affordable cost

---

## 🧠 Design Decision
Speech processing is handled by the smartphone instead of ESP32 to reduce computational load and improve performance.

---

## ⚠️ Limitations
- Performance depends on lighting conditions
- Cannot identify object types
- No distance measurement

---

## 🚀 Future Scope
- AI-based object detection
- Direction guidance (left/right)
- Integration with ultrasonic sensors
- Wearable smart glasses

---

## 📷 Screenshots
### Arduino Code
![Code](screenshots/code.png)

### Hardware Setup
![Hardware](screenshots/hardware.png)

### Web Interface
![Web](screenshots/web.png)

### Detection
![Detection](screenshots/Detection.jpg)


---

## Author
Praveen Raj K
Oviya E
