
// Midi Message Format:

// Within the MIDI Specification there are two basic types of message bytes: the STATUS byte and the DATA byte. 
// The Most Significant Bit of a status byte is always 1, the Most Significant Bit of a data byte is always 0. 
// The maximum length for a standard MIDI message is three bytes but for some types of MIDI message the length
// can be less.

// There are two different classifications of MIDI messages: MIDI Data Messages and MIDI System Messages.
// MIDI Data Messages are channel specific, MIDI System Messages are not channel specific.

// Status Byte Format: 1tttnnnn
// where 1ttt specifies the message type and nnnn specifies the MIDI channel (except for System messages).

// Status: 1000nnnn = Note Off
// Status: 1001nnnn = Note On
// Status: 1010nnnn = Polyphonic Aftertouch
// Status: 1011nnnn = Control Change
// Status: 1100nnnn = Program Change
// Status: 1101nnnn = Channel Aftertouch
// Status: 1110nnnn = Pitch Wheel
// Status: 1111nnnn = System Message


// MIDI Pin Out
//        ------- 
//      /    2   \
//     /  5     4 \
//     | 3       1 |
//      \         /
//       \ __/\__/
        
// Pin No.  IN Signal name  THRU Signal name  Out Signal name
// 1        No Connect      No Connect        No Connect
// 2        No Connect      Shield            Shield
// 3        No Connect      No Connect        No Connect
// 4        IN+             +5v               +5v
// 5        IN-             IN                IN


// MIDI standard: 31250 baud, 1 start bit, 8 data, no parity, 1 stop bit. 
//
// On the transmit side:
// the logic-level output from the transmitting device's UART goes to (typically) an open-collector transistor 
// driver, the other side of the output being a resistor-limited power feed from +Ve in the sending device.
//
// On the receive side:
// an opto-coupler takes in the open-collector drive from the remote device and converts it back to logic level 
// for the input of the UART.

// This scheme ensures that the only 'connection' between the two devices is via the light beams inside the optos 
// at each receiving end of the link, and eliminates hum-loops which might arise if there were two separate ground
// connections between the two items of equipment (one for the audio, and one for the comms data).


// Serial port communicates on digital pins 0 (RX) and 1 (TX) as well as with the computer via USB. 
// Thus, since we use these functions, we cannot also use pins 0 and 1 for digital input or output.

#define WITH_LCD //define this if a LCD is connected 

#include <MIDI.h>

#ifdef WITH_LCD
#include <LiquidCrystal.h>
#endif


//The circuit:(ATTENTION max contrast is when VO is ground !)

// * LCD RS pin to digital pin 12
// * LCD Enable pin to digital pin 11
// * LCD D4 pin to digital pin 5
// * LCD D5 pin to digital pin 4
// * LCD D6 pin to digital pin 3
// * LCD D7 pin to digital pin 2
// * LCD R/W pin to ground
// * LCD VSS pin to ground
// * LCD VCC pin to 5V
// * 10K potentiometer:
// *   ends to +5V and ground
// *   wiper to LCD VO pin (pin 3)


// Used MIDI CC for POD v2
const byte MIDIvolume    = 7;
const byte MIDIwhaValue  = 4;
const byte MIDIwhaEnable = 43;


#ifdef WITH_LCD
// LCD interface pin and corresponding Arduino pin number it is connected to:
const int rs = 7;
const int en = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
#endif


// PedalBoard - Arduino pins
//
const int ChanA  = 8;
const int ChanB  = 9;
const int ChanC  = 10;
const int ChanD  = 11;
const int WhaSw  = 12;// Wha On-Off
const int WhaSt  = 13;// Wha Status Led
#ifdef WITH_LCD
// Warning:
// The Atmega datasheet cautions against switching analog pins in close
// temporal proximity to making A/D readings (analogRead) on other analog pins.
// This can cause electrical noise and introduce jitter in the analog system.
const int BankUp = A0;
const int BankDn = A1;
#else
const int BankUp = 6;
const int BankDn = 7;
#endif
const int WhaVal = A2;
const int Volume = A3;


// Miscellanea
const byte podAddress = 1;// This is the MIDI address #1 (the POD default)
const unsigned long debounceTime = 10;  // milliseconds


// Used variables
int  input;                                                 

// PedalBoard Status Values
//
int statusChanA  = HIGH;
int statusChanB  = HIGH;
int statusChanC  = HIGH;
int statusChanD  = HIGH;
int statusBankUp = HIGH;
int statusBankDn = HIGH;
int statusWhaSw  = HIGH;// Wha On-Off
int statusWhaSt  = LOW; // Wha Status Led
int statusWhaVal = 0;
#ifdef WITH_LCD
int statusVolume =-1;// Just to check if the POD is connected;
#else
int statusVolume = 0;
#endif
int currentBank  = 0;// from 0 to 8 corresponding to POD 1 to 9
int currentChan  = 0;// from 0 to 3 corresponding to POD A to D


// Create the used objects 
#ifdef WITH_LCD
LiquidCrystal LCD(rs, en, d4, d5, d6, d7);
#endif
MIDI_CREATE_DEFAULT_INSTANCE();


#ifdef WITH_LCD
void
handleControlChange(byte channel, byte number, byte value) {
    if(value & 0x80) 
        return;// Not a valid value;
    if(number == MIDIvolume) {// Channel Volume
        statusVolume = value;
        LCD.setCursor(12, 0);
        LCD.print("   ");
        LCD.setCursor(12, 0);
        LCD.print(statusVolume);
        return;  
    }
    if(number == MIDIwhaEnable) {
        LCD.setCursor(17, 0);
        if(value < 64)
            LCD.print("   ");
        else
            LCD.print("Wha");
        return;
    }
    if(number == MIDIwhaValue) {// Wha Value
        statusWhaVal = value;
        return;
    }
    // else
    LCD.setCursor(4, 1);
    LCD.print("   ");
    LCD.setCursor(4, 1);
    LCD.print(number);
    LCD.setCursor(12, 1);
    LCD.print("   ");
    LCD.setCursor(12, 1);
    LCD.print(value);
}


void
handleProgramChange(byte channel, byte number) {
    currentBank = (number-1) >> 2;
    currentChan = (number-1) & 0x03;
    LCD.setCursor(4, 0);
    LCD.print("   ");
    LCD.setCursor(4, 0);
    LCD.print(currentBank+1);
    LCD.print(char(65+currentChan));
}


void
handleSysEx(byte *array, unsigned size) {
    LCD.setCursor(0, 3);
    LCD.print("   ");
    LCD.setCursor(0, 3);
    LCD.print(size);
}


void 
setupLCD() {
    // set up the LCD
    LCD.begin(20, 4);// Number of Columns and Rows:
    LCD.setCursor(8, 1);
    LCD.print("POD");
    LCD.setCursor(4, 2);
    LCD.print("Disconnected");
}


void
printHeaders() {
    // Print the headers to the LCD.
    LCD.clear(); 
    LCD.setCursor(0, 0);// LCD.setCursor(col, row)   
    LCD.print("Pgm:");
    LCD.print(currentBank+1);
    LCD.print(char(65+currentChan));
    LCD.setCursor(8, 0);
    LCD.print("Vol:");
    LCD.print(statusVolume);
    LCD.setCursor(0, 1);
    LCD.print("CCn:");
    LCD.setCursor(8, 1);
    LCD.print("Val:");
}
#endif

void
setupPedalBoard() {
    // There are 20K pullup resistors built into the Atmega chip...
    pinMode(ChanA,  INPUT_PULLUP);// Channel A  Switch
    pinMode(ChanB,  INPUT_PULLUP);// Channel B  Switch
    pinMode(ChanC,  INPUT_PULLUP);// Channel C  Switch
    pinMode(ChanD,  INPUT_PULLUP);// Channel D  Switch
    pinMode(BankUp, INPUT_PULLUP);// Bank Up    Switch
    pinMode(BankDn, INPUT_PULLUP);// Bank Down  Switch
    pinMode(WhaSw,  INPUT_PULLUP);// Wha On-Off Switch
    pinMode(WhaSt,  OUTPUT);      // Wha Status Led (same pin as the internal Led)
    pinMode(WhaVal, INPUT);       // Wha Potentiometer
    pinMode(Volume, INPUT);       // Volume Potentiometer
}


void
setupMIDI() {
#ifdef WITH_LCD
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleSystemExclusive(handleSysEx);
#endif
    MIDI.begin(podAddress);// Calling MIDI.begin(), the Thru functionality is activated 
    MIDI.turnThruOff();    // Since we don't want to act as a MIDI Thru.
}


void
initPOD() {
#ifdef WITH_LCD
    while(statusVolume < 0) {
        MIDI.sendControlChange(MIDIvolume, 0, podAddress);
        MIDI.read();
    }
    MIDI.sendProgramChange(currentChan+1+(currentBank<<2), podAddress);
#endif
    digitalWrite(WhaSt, statusWhaSt);
}

void 
setup() {
    setupPedalBoard();
#ifdef WITH_LCD
    setupLCD();
#endif
    setupMIDI();
    initPOD();
#ifdef WITH_LCD
    printHeaders();
#endif
}


void 
loop() {
    // Channel Switches
    input = digitalRead(ChanA);
    if(input != statusChanA) {
        statusChanA = input;
        if(input == LOW) {
            MIDI.sendProgramChange(1+(currentBank<<2), podAddress);
#ifdef WITH_LCD
            currentChan = 0;
#endif
        }
    }
    input = digitalRead(ChanB);
    if(input != statusChanB) {
        statusChanB = input;
        if(input == LOW) {
            MIDI.sendProgramChange(2+(currentBank<<2), podAddress);
#ifdef WITH_LCD
            currentChan = 1;
#endif
        }
    }
    input = digitalRead(ChanC);
    if(input != statusChanC) {
        statusChanC = input;
        if(input == LOW) {
            MIDI.sendProgramChange(3+(currentBank<<2), podAddress);
#ifdef WITH_LCD
            currentChan = 2;
#endif
        }
    }
    input = digitalRead(ChanD);
    if(input != statusChanD) {
        statusChanD = input;
        if(input == LOW) {
            MIDI.sendProgramChange(4+(currentBank<<2), podAddress);
#ifdef WITH_LCD
            currentChan = 3;
#endif
        }
    }
    // Bank Switches
    input = digitalRead(BankDn);
    if(input != statusBankDn) {
        statusBankDn = input;
        if(input == LOW) {
            currentBank--;
            if(currentBank < 0)
                currentBank = 8;
            MIDI.sendProgramChange(1+(currentBank<<2)+currentChan, podAddress);
            delay(debounceTime);
        }
    }
    input = digitalRead(BankUp);
    if(input != statusBankUp) {
        statusBankUp = input;
        if(input == LOW) {
            currentBank++;
            if(currentBank > 8) currentBank = 0;
            MIDI.sendProgramChange(1+(currentBank<<2)+currentChan, podAddress);
            delay(debounceTime);
        }
    }
    // Wha
    input = digitalRead(WhaSw);
    if(input != statusWhaSw) {
        statusWhaSw = input;
        if(input == LOW) {// High to LOW transition
            if(statusWhaSt == HIGH) {// Toggle the wha status
                statusWhaSt = LOW;
                MIDI.sendControlChange(MIDIwhaEnable, 0, podAddress);
            }
            else {
                statusWhaSt = HIGH;
                MIDI.sendControlChange(MIDIwhaEnable, 127, podAddress);
            }
            delay(debounceTime);
        }
        digitalWrite(WhaSt, statusWhaSt);
    }
    if(statusWhaSt == HIGH) {
        input = (analogRead(WhaVal) >> 4) << 1;
        if(input != statusWhaVal) {
            statusWhaVal = input;
            MIDI.sendControlChange(MIDIwhaValue, statusWhaVal, podAddress);
        }
    }
    // Volume
    input = (analogRead(Volume) >> 4) << 1;
    if(input != statusVolume) {
        statusVolume = input;
        MIDI.sendControlChange(MIDIvolume, statusVolume, podAddress);
    }

#ifdef WITH_LCD
    // Call MIDI.read() the fastest you can for real-time performance.
    MIDI.read();
#endif
}
