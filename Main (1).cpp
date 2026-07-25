#include <Arduino.h>
#include <CAN.h>

// Relay pins
#define RELAY1 16
#define RELAY2 17
#define RELAY3 18
#define RELAY4 19

// Debug LED
#define DEBUG_LED 2

// CAN pins
#define CAN_RX 4
#define CAN_TX 5

// CAN control ID
#define CAN_CONTROL_ID 0x100

void setup()
{
    Serial.begin(115200);

    // Setup relay outputs
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    pinMode(RELAY3, OUTPUT);
    pinMode(RELAY4, OUTPUT);

    // Debug LED
    pinMode(DEBUG_LED, OUTPUT);
    digitalWrite(DEBUG_LED, LOW);

    // Start CAN
    CAN.setPins(CAN_RX, CAN_TX);

    if (!CAN.begin(500E3))
    {
        Serial.println("CAN init failed");
        while (1);
    }

    Serial.println("CAN started");
}

void loop()
{
    int packetSize = CAN.parsePacket();

    if (packetSize)
    {
        // blink debug LED
        digitalWrite(DEBUG_LED, HIGH);

        int id = CAN.packetId();

        if (id == CAN_CONTROL_ID)
        {
            int relayState = CAN.read();

            digitalWrite(RELAY1, relayState & 0x01);
            digitalWrite(RELAY2, relayState & 0x02);
            digitalWrite(RELAY3, relayState & 0x04);
            digitalWrite(RELAY4, relayState & 0x08);
        }

        delay(20);
        digitalWrite(DEBUG_LED, LOW);
    }
}
