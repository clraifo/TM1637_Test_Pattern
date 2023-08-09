#define CLK 2
#define DIO 3

// Lookup table for numbers and some characters. 
// Format: dpGFEDCBA
const uint8_t SEGMENT_LOOKUP[] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F, // 9
  0x77, // A
  0x7C, // b
  0x39, // C
  0x5E, // d
  0x79, // E
  0x71, // F
  0x31, // L
  0x38, // U
  0x3E, // V
  0x76, // H
  0x54, // h
  0x5C, // n
  0x73, // P
  0x6E, // t
  0x3B, // Y
  0x1E  // J
};

void start() {
  digitalWrite(CLK, HIGH);
  digitalWrite(DIO, HIGH);
  digitalWrite(DIO, LOW);
  digitalWrite(CLK, LOW);
}

void stop() {
  digitalWrite(CLK, LOW);
  digitalWrite(DIO, LOW);
  digitalWrite(CLK, HIGH);
  digitalWrite(DIO, HIGH);
}

bool writeByte(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(CLK, LOW);
    if (data & 0x01) {
      digitalWrite(DIO, HIGH);
    } else {
      digitalWrite(DIO, LOW);
    }
    digitalWrite(CLK, HIGH);
    data >>= 1;
  }

  // Wait for ACK
  digitalWrite(CLK, LOW);
  pinMode(DIO, INPUT);
  digitalWrite(CLK, HIGH);
  bool ack = !digitalRead(DIO);
  pinMode(DIO, OUTPUT);
  return ack;
}

void displaySegment(uint8_t position, uint8_t segment) {
  start();
  writeByte(0x44); // Fixed address mode
  stop();

  start();
  writeByte(0xC0 | position); // Set address
  writeByte(segment); // Write segment data
  stop();

  start();
  writeByte(0x8F); // Display control command
  stop();
}

void displayCharacter(uint8_t position, char ch) {
  uint8_t segment = 0; // Blank by default
  if (ch >= '0' && ch <= '9') {
    segment = SEGMENT_LOOKUP[ch - '0'];
  } else if (ch >= 'A' && ch <= 'Z') {
    segment = SEGMENT_LOOKUP[10 + ch - 'A'];
  } else if (ch >= 'a' && ch <= 'z') {
    segment = SEGMENT_LOOKUP[10 + ch - 'a']; // Note: Not all lowercase letters have representations!
  }
  displaySegment(position, segment);
}

void clearDisplay() {
  for (uint8_t position = 0; position < 4; position++) {
    displaySegment(position, 0); // Clear each position
  }
}


void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  digitalWrite(CLK, HIGH);
  digitalWrite(DIO, HIGH);
}

void loop() {
  // Step 1, 2, and 3 for all positions
  for (int position = 0; position < 4; position++) {
    // Step 1: Actuate each segment in the current position
    for (uint8_t segment = 0x01; segment <= 0x7F; segment <<= 1) {
      displaySegment(position, segment);
      delay(10000 / (4 * (7 + 26 + 10)));  // Total segments + alphabet + numbers
    }

    // Step 2: Write the lowercase English alphabet
    const char alphabet[] = "abcdefghijlnoprstuvyz";
    for (uint8_t i = 0; i < sizeof(alphabet) - 1; i++) {
      displayCharacter(position, alphabet[i]);
      delay(10000 / (4 * (7 + 26 + 10)));
    }

    // Step 3: Write numbers 0-9
    for (int num = 0; num <= 9; num++) {
      displayCharacter(position, '0' + num);
      delay(10000 / (4 * (7 + 26 + 10)));
    }
  }

// Set all digits to "9"
for (uint8_t position = 0; position < 4; position++) {
    displaySegment(position, 0x6F); // Number 9
}

// Step 5: Blink only the colon between positions 2 and 3
for (int i = 0; i < 4; i++) {
    displaySegment(1, 0x80 | 0x6F); // Colon ON with number 9
    delay(10000 / (8 * 4));  // 4 blinks, ON and OFF
    displaySegment(1, 0x6F); // Colon OFF (but number 9 remains)
    delay(10000 / (8 * 4));
}


  // Step 6: Blink the entire TM1637 three times
  for (int i = 0; i < 3; i++) {
    for (uint8_t position = 0; position < 4; position++) {
      displaySegment(position, 0x7F);
    }
    delay(10000 / (6 * 3));  // 3 blinks, ON and OFF
    clearDisplay();
    delay(10000 / (6 * 3));
  }

  // Step 7: Set the entire module to LOW
  clearDisplay();

  delay(2000);  // Wait for 2 seconds before restarting the test pattern (to fill up the remaining time and easily distinguish between tests)
}



