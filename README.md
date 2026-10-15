# 🌱 Smart Greenhouse Project

An IoT-based Smart Greenhouse system designed to monitor and automate environmental conditions (such as temperature, humidity, and soil moisture) with a strong focus on security and fail-safe operation.

## 🔗 Live Simulation
You can test and view the circuit simulation live on Wokwi:
👉 [View Wokwi Simulation](https://wokwi.com/projects/462630950999949313)

## 📊 Presentation
The project presentation slides can be found in the [presentation folder](./presentation) or downloaded directly from the repository.

## 🛠️ Features
- Automated climate and soil monitoring
- Real-time sensor data tracking
- Virtual hardware prototyping via Wokwi
- 4-layer security system (PIN, lockout, alarm, fail-safe)
- Automatic watering, lighting, and cooling
- LCD display for real-time sensor data
- Buzzer alarm after 3 failed PIN attempts

## 🔐 Security Features
- **PIN Authentication**: 4-digit code (default: 1234) required to unlock the greenhouse door
- **Brute Force Protection**: 3 failed attempts trigger a 5-minute lockout
- **Audible Alarm**: Buzzer sounds continuously during lockout
- **Fail-Safe Design**: All actuators start in the OFF position for safety
- **Local Processing**: System operates without cloud dependency

## ⚙️ Components Used
| Component | Purpose |
|-----------|---------|
| ESP32 | Main controller (brain) |
| 4x4 Keypad | PIN entry for door access |
| Soil moisture sensor | Detects dry/wet soil |
| Light sensor (LDR) | Detects dark/bright conditions |
| DHT22 sensor | Temperature & humidity monitoring |
| LCD 16x2 I2C | Display sensor data |
| LEDs (3x) | Simulate pump, grow light, and fan |
| Buzzer | Alarm on 3 failed PIN attempts |

## 🏗️ System Architecture
The system follows a 4-layer architecture:
1. **Perception Layer**: Sensors and keypad collect data
2. **Network Layer**: WiFi and cloud connectivity (Blynk)
3. **Processing Layer**: ESP32 authenticates PIN and makes automation decisions
4. **Application Layer**: LCD display and phone notifications

## 🚀 How It Works
1. System starts in a **locked** state (all actuators OFF)
2. User enters 4-digit PIN on keypad
3. Wrong PIN → ACCESS DENIED (3 attempts max)
4. 3 wrong PINs → 5-minute lockout + buzzer alarm
5. Correct PIN → Door unlocks, automation begins
6. Sensors control actuators automatically:
   - Soil dry → Water pump ON
   - Dark → Grow light ON
   - Temperature > 30°C → Cooling fan ON
7. LCD displays real-time sensor data

## 🧪 Simulation Results
✅ Keypad PIN authentication working  
✅ 3 failed attempts = lockout + buzzer alarm  
✅ Soil sensor controls pump LED  
✅ Light sensor controls grow light LED  
✅ DHT22 controls cooling fan LED  
✅ LCD displaying real-time data  

## 🔮 Future Improvements
- Implement on real ESP32 hardware
- Add solar power for energy efficiency
- Connect actual water pump, grow light, and fan
- Mobile dashboard via Blynk app
- Cloud data logging and historical analysis
- AI-based predictive watering

## 📚 Technologies Used
- **ESP32** (Arduino framework)
- **Wokwi** (IoT simulation)
- **Blynk** (Cloud integration - code ready)
- **C++** (Arduino programming)

## 👤 Author
**Tarhouni Haithem**  
**Tarhouni04haithem@gmail.com**
**https://github.com/Haithem-ICT**

## 📄 License
This project is for educational purposes.
