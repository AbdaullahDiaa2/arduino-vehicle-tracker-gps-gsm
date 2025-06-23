# Arduino-Based Vehicle Tracker using GPS and GSM

🚗 A real-time vehicle tracking system built with Arduino, GPS, and GSM modules.  
📍 Tracks the live location of a vehicle and sends coordinates via SMS to a designated phone number.  
Useful for vehicle security, fleet management, and remote monitoring.

## 🔧 Features

- 📡 Real-time GPS tracking (latitude & longitude)
- 📲 Sends location via GSM (SMS)
- ⚠️ Can be extended to include theft detection, speed alert, or geofencing
- 💾 Lightweight, low-cost, and customizable

## 🛠️ Components Used

- Arduino Uno (or compatible board)
- NEO-6M GPS module
- SIM800L GSM module (or SIM900)
- Power supply (battery or 12V)
- Wires, breadboard 
![Vehicle Tracker Prototype](/arduino-vehicle-tracker-gps-gsm.jpg)

https://github.com/AbdaullahDiaa2/arduino-vehicle-tracker-gps-gsm/blob/main/arduino-vehicle-tracker-gps-gsm.jpg
## 📘 How it Works

1. The GPS module receives satellite signals to determine the vehicle's location.
2. The Arduino reads the GPS data and formats the latitude/longitude.
3. The GSM module sends the location as an SMS to a preset phone number.
4. You can request location by sending a specific command via SMS (optional feature).

## 🧠 Future Improvements

- Add a mobile app to visualize location on a map
- Use a web server for live map tracking
- Add sensors for engine status or vibration (for theft detection)
- Replace GSM with Wi-Fi/Bluetooth if needed

## 📸 Demo / Screenshot

*(Insert image here if available — breadboard or working prototype)*

## 📄 License

This project is open-source under the MIT License.
