
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

// MIDI CC for POD v2
const byte MIDIvolume    =  7;
const byte MIDIwhaValue  =  4;
const byte MIDIwhaEnable = 43;


// Serial port communicates on digital pins 0 (RX) and 1 (TX) as well as with the computer via USB. 
// Thus, since we use these functions, we cannot also use pins 0 and 1 for digital input or output.

#include <MIDI.h>

// PedalBoard - Arduino pins
// Warning:
// The Atmega datasheet cautions against switching analog pins in close
// temporal proximity to making A/D readings (analogRead) on other analog pins.
// This can cause electrical noise and introduce jitter in the analog system.

// Digital Inputs
const int BankUp = 2;
const int BankDn = 3;
const int ChanA  = 4;
const int ChanB  = 5;
const int ChanC  = 6;
const int ChanD  = 7;
const int WhaSw  = 8;// Wha On-Off

// Digital Outputs
const int WhaSt  = 13;// Wha Status Led (same as LED_BUILTIN)

// Analog Inputs
const int WhaVal = A2;
const int Volume = A3;

const byte podAddress     = 1;// This is the MIDI address #1 (the POD default)

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
int statusVolume = 0;
int currentBank  = 0;
int currentChan  = 1;

// Create the used objects 
MIDI_CREATE_DEFAULT_INSTANCE();


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
    statusVolume = (analogRead(Volume) >> 4) << 1;
    statusWhaVal = (analogRead(WhaVal) >> 4) << 1;
    digitalWrite(WhaSt, statusWhaSt);
}


void
setupMIDI() {
    MIDI.begin(podAddress);// Calling MIDI.begin(), the Thru functionality is activated 
    MIDI.turnThruOff();    // Since we don't want to act as a MIDI Thru.
}


void 
setup() {
    setupPedalBoard();
    setupMIDI();
}


void 
loop() {
    // Channel Switches
    input = digitalRead(ChanA);
    if(input != statusChanA) {
        statusChanA = input;
        if(input == LOW) {
            currentChan = 1;
            MIDI.sendProgramChange(currentBank+currentChan, podAddress);
        }
    }
    input = digitalRead(ChanB);
    if(input != statusChanB) {
        statusChanB = input;
        if(input == LOW) {
            currentChan = 2;
            MIDI.sendProgramChange(currentBank+currentChan, podAddress);
        }
    }
    input = digitalRead(ChanC);
    if(input != statusChanC) {
        statusChanC = input;
        if(input == LOW) {
            currentChan = 3;
            MIDI.sendProgramChange(currentBank+currentChan, podAddress);
        }
    }
    input = digitalRead(ChanD);
    if(input != statusChanD) {
        statusChanD = input;
        if(input == LOW) {
            currentChan = 4;
            MIDI.sendProgramChange(currentBank+currentChan, podAddress);
        }
    }
    // Bank Switches
    input = digitalRead(BankDn);
    if(input != statusBankDn) {
        statusBankDn = input;
        if(input == LOW) {
            currentBank += 4;
            if(currentBank > 36)
                currentBank = 0;
            MIDI.sendProgramChange(currentBank+currentChan, podAddress);
        }
    }
    input = digitalRead(BankUp);
    if(input != statusBankUp) {
        statusBankUp = input;
        if(input == LOW) {
            currentBank -= 4;
            if(currentBank < 0)
                currentBank = 32;
            MIDI.sendProgramChange(currentBank+currentChan, podAddress);
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
}
