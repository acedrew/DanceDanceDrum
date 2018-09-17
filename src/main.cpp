#include <Arduino.h>
#include <libmaple/usb_cdcacm.h>
#include <main.h>
#include <WS2812B.h>

#define DRUM1 PB1
#define DRUM2 PB0
#define DRUM3 PA7
#define DRUM4 PA6
#define DRUM5 PA5

#define PUMP PA0

#define PRINT_INTERVAL 250
#define SHOW_INTERVAL 20

#define NUM_LEDS 100

#define PUMPMAX 3200
#define PUMPMIN 1000

bool drumState[5] = {false};
bool not_scanned = true; 
int drumVals[5] = {0};
int drums[5] = {DRUM1, DRUM2, DRUM3, DRUM4, DRUM5};
int drumThresholds[5] = {1500, 1300, 1310, 1300, 1300};
int drumTimes[5] = {0};

uint32_t drumColors[5] = {
    0xfe0000,
    0xf9fe00,
    0xfe3000,
    0x0003fe,
    0x9d00fe
};
uint32_t currentColor = 0;
//loop timer state
int lastLoop = 0;
int showLoop = 0;

WS2812B strip = WS2812B(NUM_LEDS);

static const usb_descriptor_string usbVcomDescriptor_iManufacturer = {
    .bLength = USB_DESCRIPTOR_STRING_LEN(8),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_STRING,
    .bString = {'L', 0, 'e', 0, 'a', 0, 'f', 0,
                'L', 0, 'a', 0, 'b', 0, 's', 0},
};

static const usb_descriptor_string usbVcomDescriptor_iProduct = {
    .bLength = USB_DESCRIPTOR_STRING_LEN(5),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_STRING,
    .bString = {'D', 0, 'r', 0, 'u', 0, 'm', 0, 's', 0},
};


void setup() {
    // put your setup code here, to run once:
    pinMode(DRUM1, INPUT_ANALOG);
    pinMode(DRUM2, INPUT_ANALOG);
    pinMode(DRUM3, INPUT_ANALOG);
    pinMode(DRUM4, INPUT_ANALOG);
    pinMode(DRUM5, INPUT_ANALOG);

    // attachInterrupt(DRUM1, checkDrums, RISING);
    // attachInterrupt(DRUM2, checkDrums, RISING);
    // attachInterrupt(DRUM3, checkDrums, RISING);
    // attachInterrupt(DRUM4, checkDrums, RISING); // attachInterrupt(DRUM5, checkDrums, RISING);
    strip.begin();
    Serial.begin(115200);

}

void loop() {
    checkDrums();
    int loopMillis = millis();
    if (loopMillis >= showLoop + SHOW_INTERVAL) {
        showLoop = loopMillis;
        for(int i = 0; i < 5; i++) {
            if (drumState[i]) {
                currentColor = drumColors[i];
            }
        }
        for(int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, currentColor); 
        }
        strip.show();
        currentColor = 0;
    }

    if(loopMillis - lastLoop > PRINT_INTERVAL){
        lastLoop = loopMillis;
        Serial.write(4);
        Serial.flush();
        Serial.write(constrain(map(analogRead(PUMP), PUMPMIN, PUMPMAX, 255, 0), 0, 255));
        Serial.flush();
    }
}

void checkDrums() {
    int drumMillis = millis();
    for(int i=0; i<5; i++) {
        int currentDrumVal = analogRead(drums[i]);
        if (drumMillis > drumTimes[i] + 200) {
            if (currentDrumVal > drumThresholds[i]) {
                // Serial.println(currentDrumVal);
                // Serial.println(drumThresholds[i]);
                // Serial.println(i);
                drumState[i] = true;
                drumTimes[i] = drumMillis;
                Serial.write(2);
                Serial.flush();
                Serial.write(i);
                Serial.flush();
            } else {
                drumState[i] =  false;
            }
        }
    }
    
}
void checkDrumVals() {
    for(int i=0; i<5; i++) {
        drumVals[i] = analogRead(drums[i]);
    }
    
}