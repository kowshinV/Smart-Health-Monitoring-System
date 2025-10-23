#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MAX30100_PulseOximeter.h>

// Define the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address, 16 columns, 2 rows

// Define the MAX30100 sensor
MAX30100_PulseOximeter pox;
uint32_t tsLastReport = 0;

// Setup the variables for Heart rate and SpO2
float heartRate = 0;
float SpO2 = 0;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Patient Monitor");

  // Initialize the MAX30100 sensor
  if (!pox.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("MAX30100 Error!");
    while (1);
  }

  // Set the sample rate and other parameters of MAX30100 sensor
  pox.setIRLedCurrent(MAX30100_LED_CURRENT_50MA);
  pox.setPulseAmplitudeModulation(MAX30100_PULSE_AMPLITUDE_MODULATION_AMP_1);
  pox.setSpO2SamplingRate(MAX30100_SPO2_SAMPLING_RATE_100HZ);
  pox.setHeartRateSamplingRate(MAX30100_HEART_RATE_SAMPLING_RATE_100HZ);
  pox.setPulseWidth(MAX30100_PULSE_WIDTH_1600US);
}

void loop() {
  // Update the sensor readings
  pox.update();

  // Check if a new reading is available
  if (millis() - tsLastReport > 1000) {
    tsLastReport = millis();

    // Get the heart rate and SpO2 readings
    heartRate = pox.getHeartRate();
    SpO2 = pox.getSpO2();

    // Print the values on Serial Monitor
    Serial.print("Heart Rate: ");
    Serial.print(heartRate);
    Serial.print(" bpm, SpO2: ");
    Serial.print(SpO2);
    Serial.println(" %");

    // Display the readings on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HR: ");
    lcd.print(heartRate);
    lcd.print(" bpm");
    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(SpO2);
    lcd.print(" %");
  }
}
