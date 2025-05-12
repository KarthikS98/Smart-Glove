#include <DFPlayer.h>
#include <LiquidCrystal.h>

// Flex sensor pins
const int thumbPin = PA0;
const int indexPin = PA1;
const int middlePin = PA2;
const int ringPin = PA3;

// Threshold values for flex sensors
const int thumbThreshold = 1200;
const int indexThreshold = 1120;
const int middleThreshold = 1950;
const int ringThreshold = 1210;

// Pins for DFPlayer Mini
#define MP3_SERIAL_SPEED 9600
#define MP3_RX_PIN PA10
#define MP3_TX_PIN PA9

// Pins for LCD
#define LCD_RS PB3
#define LCD_EN PB4
#define LCD_D4 PB5
#define LCD_D5 PB6
#define LCD_D6 PB7
#define LCD_D7 PB8

// Pins for relay control
const int relayChannel1Pin = PB0;
const int relayChannel2Pin = PB1;

// Define LCD object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// DFPlayer object
DFPlayer mp3;

// Mode variables
int currentMode = 1; // 1: Mode 1 (Sign Language), 2: Mode 2 (Relay Control)

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Initialize Serial1 with specified baud rate
  Serial1.begin(MP3_SERIAL_SPEED);
  
  // Initialize DFPlayer with Serial1
  mp3.begin(Serial1);

  // Reset and configure DFPlayer
  mp3.stop();
  mp3.reset();
  mp3.setSource(2);  // Set the source to SD card
  mp3.setEQ(0);      // Set EQ to off
  mp3.setVolume(30); // Set volume level (0..30)
  mp3.sleep();       // Enter sleep mode

  // Set up the LCD
  lcd.begin(16, 2);

  // Print initial message on LCD
  lcd.print("Selected Mode:");
  lcd.setCursor(0, 1);
  lcd.print("Mode ");
  lcd.print(currentMode);

  // Set relay pins as output
  pinMode(relayChannel1Pin, OUTPUT);
  pinMode(relayChannel2Pin, OUTPUT);
}

void loop() {
  // Read flex sensor values
  int thumbValue = analogRead(thumbPin);
  int indexValue = analogRead(indexPin);
  int middleValue = analogRead(middlePin);
  int ringValue = analogRead(ringPin);

  // Print flex sensor values on serial monitor
  Serial.print("Thumb: ");
  Serial.print(thumbValue);
  Serial.print(", Index: ");
  Serial.print(indexValue);
  Serial.print(", Middle: ");
  Serial.print(middleValue);
  Serial.print(", Ring: ");
  Serial.println(ringValue);

  // Toggle mode when all fingers are bent
  if (thumbValue < thumbThreshold && indexValue < indexThreshold &&
      middleValue < middleThreshold && ringValue < ringThreshold) {
    currentMode = (currentMode == 1) ? 2 : 1; // Toggle between modes
    
    // Update LCD to display selected mode
    lcd.clear();
    lcd.print("Selected Mode:");
    lcd.setCursor(0, 1);
    lcd.print("Mode ");
    lcd.print(currentMode);
    
    delay(3000); // Wait for 3 seconds after selecting mode
    
    // Turn off relays if switching to Mode 1
    if (currentMode == 1) {
      digitalWrite(relayChannel1Pin, LOW);
      digitalWrite(relayChannel2Pin, LOW);
    }
    
    // Stop any ongoing audio playback when switching to Mode 1
    if (currentMode == 1) {
      mp3.stop();
    }

    delay(500); // Debouncing delay
  }

// Mode 1: Sign Language Mode
if (currentMode == 1) {
  static int lastPlayedAudio = 0; // Variable to store the last played audio file number

  // Single Finger Actions
  if (thumbValue < thumbThreshold && indexValue >= indexThreshold && middleValue >= middleThreshold && ringValue >= ringThreshold) {
    if (lastPlayedAudio != 1) {
      lcd.clear();
      lcd.print("Hello");
      mp3.playFolder(1, 1); // Play audio file 001 from folder 1
      lastPlayedAudio = 1; // Update last played audio file number
      delay(1000); // Wait for the audio to complete
    }
  } else if (indexValue < indexThreshold && thumbValue >= thumbThreshold && middleValue >= middleThreshold && ringValue >= ringThreshold) {
    if (lastPlayedAudio != 2) {
      lcd.clear();
      lcd.print("Help me.");
      mp3.playFolder(1, 2); // Play audio file 002 from folder 1
      lastPlayedAudio = 2;
      delay(1000);
    }
  } else if (middleValue < middleThreshold && thumbValue >= thumbThreshold && indexValue >= indexThreshold && ringValue >= ringThreshold) {
    if (lastPlayedAudio != 3) {
      lcd.clear();
      lcd.print("I'm hungry.");
      mp3.playFolder(1, 3); // Play audio file 003 from folder 1
      lastPlayedAudio = 3;
      delay(1000);
    }
  } else if (ringValue < ringThreshold && thumbValue >= thumbThreshold && indexValue >= indexThreshold && middleValue >= middleThreshold) {
    if (lastPlayedAudio != 4) {
      lcd.clear();
      lcd.print("Thank you.");
      mp3.playFolder(1, 4); // Play audio file 004 from folder 1
      lastPlayedAudio = 4;
      delay(1000);
    }
  }
  // Two Finger Combinations
  else if (thumbValue < thumbThreshold && indexValue < indexThreshold && middleValue >= middleThreshold && ringValue >= ringThreshold) {
    if (lastPlayedAudio != 5) {
      lcd.clear();
      lcd.print("Please help.");
      mp3.playFolder(1, 5); // Play audio file 005 from folder 1
      lastPlayedAudio = 5;
      delay(1000);
    }
  } else if (thumbValue < thumbThreshold && middleValue < middleThreshold && indexValue >= indexThreshold && ringValue >= ringThreshold) {
    if (lastPlayedAudio != 6) {
      lcd.clear();
      lcd.print("I'm in pain.");
      mp3.playFolder(1, 6); // Play audio file 006 from folder 1
      lastPlayedAudio = 6;
      delay(1000);
    }
  } else if (thumbValue < thumbThreshold && ringValue < ringThreshold && indexValue >= indexThreshold && middleValue >= middleThreshold) {
    if (lastPlayedAudio != 7) {
      lcd.clear();
      lcd.print("I'm tired.");
      mp3.playFolder(1, 7); // Play audio file 007 from folder 1
      lastPlayedAudio = 7;
      delay(1000);
    }
  } else if (indexValue < indexThreshold && middleValue < middleThreshold && thumbValue >= thumbThreshold && ringValue >= ringThreshold) {
    if (lastPlayedAudio != 8) {
      lcd.clear();
      lcd.print("I'm thirsty.");
      mp3.playFolder(1, 8); // Play audio file 008 from folder 1
      lastPlayedAudio = 8;
      delay(1000);
    }
  } else if (indexValue < indexThreshold && ringValue < ringThreshold && thumbValue >= thumbThreshold && middleValue >= middleThreshold) {
    if (lastPlayedAudio != 9) {
      lcd.clear();
      lcd.print("I need rest.");
      mp3.playFolder(1, 9); // Play audio file 009 from folder 1
      lastPlayedAudio = 9;
      delay(1000);
    }
  } else if (middleValue < middleThreshold && ringValue < ringThreshold && thumbValue >= thumbThreshold && indexValue >= indexThreshold) {
    if (lastPlayedAudio != 10) {
      lcd.clear();
      lcd.print("I'm feeling cold.");
      mp3.playFolder(1, 10); // Play audio file 010 from folder 1
      lastPlayedAudio = 10;
      delay(1000);
    }
  }
  // Three Finger Combinations
  else if (thumbValue < thumbThreshold && indexValue < indexThreshold && middleValue < middleThreshold && ringValue >= ringThreshold) {
    if (lastPlayedAudio != 11) {
      lcd.clear();
      lcd.print("Yes, I understand.");
      mp3.playFolder(1, 11); // Play audio file 011 from folder 1
      lastPlayedAudio = 11;
      delay(1000);
    }
  } else if (thumbValue < thumbThreshold && indexValue < indexThreshold && ringValue < ringThreshold && middleValue >= middleThreshold) {
    if (lastPlayedAudio != 12) {
      lcd.clear();
      lcd.print("No, I don't understand.");
      mp3.playFolder(1, 12); // Play audio file 012 from folder 1
      lastPlayedAudio = 12;
      delay(1000);
    }
  } else if (thumbValue < thumbThreshold && middleValue < middleThreshold && ringValue < ringThreshold && indexValue >= indexThreshold) {
    if (lastPlayedAudio != 13) {
      lcd.clear();
      lcd.print("Sorry, I made a mistake.");
      mp3.playFolder(1, 13); // Play audio file 013 from folder 1
      lastPlayedAudio = 13;
      delay(1000);
    }
  } else if (indexValue < indexThreshold && middleValue < middleThreshold && ringValue < ringThreshold && thumbValue >= thumbThreshold) {
    if (lastPlayedAudio != 14) {
      lcd.clear();
      lcd.print("Excuse me.");
      mp3.playFolder(1, 14); // Play audio file 014 from folder 1
      lastPlayedAudio = 14;
      delay(1000);
    }
  } else if (thumbValue < thumbThreshold && middleValue < middleThreshold && ringValue < ringThreshold && indexValue >= indexThreshold) {
    if (lastPlayedAudio != 15) {
      lcd.clear();
      lcd.print("Goodbye.");
      mp3.playFolder(1, 15); // Play audio file 015 from folder 1
      lastPlayedAudio = 15;
      delay(1000);
    }
  }
}


  // Mode 2: Relay Control Mode
else {
    // Control the relays based on finger gestures
    if (thumbValue < thumbThreshold) {
      digitalWrite(relayChannel1Pin, HIGH); // Turn on relay channel 1
    } else if (indexValue < indexThreshold) {
      digitalWrite(relayChannel1Pin, LOW); // Turn off relay channel 1
    }

    if (middleValue < middleThreshold) {
      digitalWrite(relayChannel2Pin, HIGH); // Turn on relay channel 2
    } else if (ringValue < ringThreshold) {
      digitalWrite(relayChannel2Pin, LOW); // Turn off relay channel 2
    }
}


  delay(500); // Delay to debounce inputs
}