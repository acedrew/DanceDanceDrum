#include <Arduino.h>
#include <main.h>
#include <WS2812B.h>

#define DRUM1 PB1
#define DRUM2 PB0
#define DRUM3 PA7
#define DRUM4 PA6
#define DRUM5 PA5

#define PRINT_INTERVAL 250
#define SHOW_INTERVAL 20

#define NUM_LEDS 100

bool drumState[5] = {false};
bool not_scanned = true;
int drumVals[5] = {0};
int drums[5] = {DRUM1, DRUM2, DRUM3, DRUM4, DRUM5};
int drumThresholds[5] = {800, 1200, 1000, 1000, 1000};

uint32_t drumColors[5] = {
    255 << 16,
    255 << 8,
    255,
    (128 << 16) + 128,
    (128 << 8) + 128
};
uint32_t currentColor = 0;
//loop timer state
int lastLoop = 0;
int showLoop = 0;

WS2812B strip = WS2812B(NUM_LEDS);



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
    int loopMillis = millis();
    if (loopMillis % 10 == 0 && not_scanned) {
        checkDrums();
        not_scanned = false;
    } else {
        not_scanned = true;
    }
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
        for(int i = 0; i<5; i++) {
            Serial.print(drumState[i]);
            Serial.print(' ');
            drumState[i] = false;
        }
        Serial.print('\n');
        Serial.println(currentColor);
        checkDrums();
    }
    // put your main code here, to run repeatedly:
}

void checkDrums() {
    for(int i=0; i<5; i++) {
        drumState[i] = (analogRead(drums[i]) > drumThresholds[i]) || drumState[i];
    }
    
}
void checkDrumVals() {
    for(int i=0; i<5; i++) {
        drumVals[i] = analogRead(drums[i]);
    }
    
}