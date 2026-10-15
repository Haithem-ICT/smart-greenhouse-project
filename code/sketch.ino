#include <Keypad.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {16, 17, 18, 19};
byte colPins[COLS] = {21, 22, 23, 25};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// DHT22 configuration
#define DHTPIN 27
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

String correctPIN = "1234";
String enteredPIN = "";
bool unlocked = false;
int failedAttempts = 0;
bool isLockedOut = false;
unsigned long lockoutStartTime;
int soilValue = 0;
int lightValue = 0;
float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C and LCD
  Wire.begin(32, 33);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0, 1);
  lcd.print(">");
  
  pinMode(13, OUTPUT);  // Pump LED
  pinMode(12, OUTPUT);  // Grow light LED
  pinMode(14, OUTPUT);  // Fan LED
  pinMode(26, OUTPUT);  // Buzzer
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
  noTone(26);
  
  dht.begin();
  
  Serial.println("==== SMART GREENHOUSE ====");
  Serial.println("Enter PIN on Keypad to unlock door:");
  Serial.println("Press # to submit, * to clear");
  Serial.println("3 failed attempts = 5 min lockout + ALARM");
  Serial.println("");
  Serial.println("📱 A notification would be sent once the door is unlocked");
  
}

void loop() {
  // Handle lockout state
  if (isLockedOut) {
    unsigned long elapsed = millis() - lockoutStartTime;
    if (elapsed < 300000) {
      tone(26, 1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SYSTEM LOCKED!");
      lcd.setCursor(0, 1);
      lcd.print("ALARM ACTIVE");
      Serial.print("🔒 SYSTEM LOCKED! Alarm active for ");
      Serial.print((300000 - elapsed) / 1000);
      Serial.println(" seconds");
      delay(1000);
      return;
    } else {
      isLockedOut = false;
      failedAttempts = 0;
      noTone(26);
      Serial.println("✅ Lockout expired. You may try again.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter PIN:");
      lcd.setCursor(0, 1);
      lcd.print(">");
    }
  }
  
  // Authentication phase
  if (unlocked == false) {
    char key = keypad.getKey();
    
    if (key) {
      if (key >= '0' && key <= '9') {
        enteredPIN += key;
        Serial.print("*");
        lcd.setCursor(enteredPIN.length(), 1);
        lcd.print("*");
      }
      else if (key == '#') {
        Serial.println();
        if (enteredPIN == correctPIN) {
          Serial.println("✅ ACCESS GRANTED!");
          Serial.println("🔓 Greenhouse's Door UNLOCKED!");
          Serial.println("📱 Notification sent successfully");
          
          unlocked = true;
          failedAttempts = 0;
          noTone(26);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ACCESS GRANTED!");
          lcd.setCursor(0, 1);
          lcd.print("Door Unlocked!");
          delay(1500);
        } else {
          failedAttempts++;
          Serial.print("❌ ACCESS DENIED! Failed attempts: ");
          Serial.println(failedAttempts);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ACCESS DENIED!");
          lcd.setCursor(0, 1);
          lcd.print("Attempts: ");
          lcd.print(failedAttempts);
          
          if (failedAttempts >= 3) {
            isLockedOut = true;
            lockoutStartTime = millis();
            Serial.println("🔔🔔🔔 TOO MANY FAILED ATTEMPTS! 🔔🔔🔔");
            Serial.println("⚙️ SYSTEM LOCKED for 5 minutes!");
            Serial.println("🚨 ALARM ACTIVATED!");
            tone(26, 1000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("SYSTEM LOCKED!");
            lcd.setCursor(0, 1);
            lcd.print("5 MIN ALARM!");
          }
          enteredPIN = "";
          Serial.println("🔐 Enter PIN (press # to submit):");
          delay(1500);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter PIN:");
          lcd.setCursor(0, 1);
          lcd.print(">");
        }
      }
      else if (key == '*') {
        enteredPIN = "";
        Serial.println();
        Serial.println("⌨️ PIN cleared. Enter again:");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter PIN:");
        lcd.setCursor(0, 1);
        lcd.print(">");
      }
    }
    return;
  }
  
  // Read sensors
  soilValue = analogRead(34);
  lightValue = analogRead(35);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    humidity = 0;
    temperature = 0;
  }
  
  // Control actuators
  digitalWrite(13, soilValue > 2000 ? HIGH : LOW);
  digitalWrite(12, lightValue < 1500 ? HIGH : LOW);
  digitalWrite(14, temperature > 30 ? HIGH : LOW);
  
  // LCD Display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(soilValue > 2000 ? "DRY " : "WET ");
  lcd.print("L:");
  lcd.print(lightValue < 1500 ? "DRK" : "BRT");
  
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");
  
  // Serial Monitor output
  Serial.print("Soil: ");
  Serial.print(soilValue);
  Serial.print(soilValue > 2000 ? " DRY🥀" : " WET💧");
  Serial.print(" | Light: ");
  Serial.print(lightValue);
  Serial.print(lightValue < 1500 ? " DARK🌃" : " BRIGHT💡");
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.print(temperature > 30 ? "°C HOT🔥" : "°C ❄️");
  Serial.print(" | Humidity: ");
  Serial.print(humidity);
  Serial.println("% 💦");
  
  delay(2000);
}
