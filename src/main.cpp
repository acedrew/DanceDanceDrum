#include <Arduino.h>
#include <main.h>

#define DRUM1 PB1
#define DRUM2 PB0
#define DRUM3 PA7
#define DRUM4 PA6
#define DRUM5 PA5

#define PRINT_INTERVAL 1000

bool drumState[5] = {false};
bool not_scanned = true;
int drumVals[5] = {0};
int drums[5] = {DRUM1, DRUM2, DRUM3, DRUM4, DRUM5};
int drumThresholds[5] = {800, 1200, 1000, 1000, 1000};
int lastLoop = 0;



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
    // attachInterrupt(DRUM4, checkDrums, RISING);
    // attachInterrupt(DRUM5, checkDrums, RISING);
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

    if(loopMillis - lastLoop > PRINT_INTERVAL){
        lastLoop = loopMillis;
        for(int i = 0; i<5; i++) {
            Serial.print(drumState[i]);
            Serial.print(' ');
            drumState[i] = false;
        }
        Serial.print('\n');
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