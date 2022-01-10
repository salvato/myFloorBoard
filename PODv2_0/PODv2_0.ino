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


// MIDI Pinout
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
// 4        IN+             +5v               +5v Trough 220 ohm
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


#include <MIDI.h>
#include <LiquidCrystal.h>
#include <InputDebounce.h>


#define ENABLED HIGH
#define DISABLED LOW


///////////////////////////////////////////////////////////////////
// LCD connections: (ATTENTION max contrast is when VO is ground !)
///////////////////////////////////////////////////////////////////

// * LCD VSS pin    (LCD pin  1) to ground
// * LCD VCC pin    (LCD pin  2) to 5V
// * LCD V0 pin     (LCD pin  3) to Potentiometer wiper
// * LCD RS pin     (LCD pin  4) to Arduino pin 7
// * LCD R/W pin    (LCD pin  5) to ground
// * LCD Enable pin (LCD pin  6) to Arduino pin 6
// * LCD D0 pin     (LCD pin  7) Not Connected
// * LCD D1 pin     (LCD pin  8) Not Connected
// * LCD D2 pin     (LCD pin  9) Not Connected
// * LCD D3 pin     (LCD pin 10) Not Connected
// * LCD D4 pin     (LCD pin 11) to Arduino pin 5
// * LCD D5 pin     (LCD pin 12) to Arduino pin 4
// * LCD D6 pin     (LCD pin 13) to Arduino pin 3
// * LCD D7 pin     (LCD pin 14) to Arduino pin 2
// * LCD A pin      (LCD pin 15) to 5V trough a 220 K resitor
// * LCD K pin      (LCD pin 16) to ground
//
// * 10K potentiometer:
// *   ends to +5V and ground
// *   wiper to LCD VO pin (LCD pin 3)


///////////////////////////
// Used MIDI CC for POD v2
///////////////////////////
const byte MIDIwhaValue  =  4;
const byte MIDIvolume    =  7;
const byte MIDIwhaEnable = 43;


/////////////////////////////////////////
// LCD interface pins and corresponding 
// Arduino pin number it is connected to:
/////////////////////////////////////////
const uint8_t rs = 7;
const uint8_t en = 6;
const uint8_t d4 = 5;
const uint8_t d5 = 4;
const uint8_t d6 = 3;
const uint8_t d7 = 2;


/////////////////////////////
// PedalBoard - Arduino pins
/////////////////////////////
const uint8_t ChanA  =  8;
const uint8_t ChanB  =  9;
const uint8_t ChanC  = 10;
const uint8_t ChanD  = 11;
const uint8_t WhaSw  = 12; // Wha On-Off
const uint8_t WhaSt  = 13; // Wha Status Led
const uint8_t BankUp = A0;
const uint8_t BankDn = A1;
const uint8_t WhaVal = A2;
const uint8_t Volume = A3;


///////////////
// Miscellanea
///////////////
const byte podAddress = 1; // This is the MIDI address #1 (the POD default)
const unsigned long debounceTime = 20; // milliseconds
unsigned long now;


                                              //////////////////
                                              // Used variables
                                              //////////////////
volatile bool bPODready = false;
int input; 
                                                
////////////////////////////
// PedalBoard Status Values
////////////////////////////
int statusChanA  = HIGH;
int statusChanB  = HIGH;
int statusChanC  = HIGH;
int statusChanD  = HIGH;
int statusBankUp = HIGH;
int statusBankDn = HIGH;
int statusWhaSw  = LOW; // Wha On-Off (started LOW and so it is not triggered if the pedal is already pushed)
int statusWhaSt  = DISABLED;
int statusWhaVal = 0;
int statusVolume = 0;
int currentBank  = 0; // from 0 to 8 corresponding to POD 1 to 9
int currentChan  = 0; // from 0 to 3 corresponding to POD A to D


////////////////////////////
// Create the used objects 
////////////////////////////
static LiquidCrystal LCD(rs, en, d4, d5, d6, d7);
static InputDebounce buttonBankUp; // not enabled yet, setup() has to be called first
static InputDebounce buttonBankDn;
static InputDebounce buttonWha;
MIDI_CREATE_DEFAULT_INSTANCE();


                                              /////////////////////////
                                              // Button Event Handlers
                                              /////////////////////////
void
BankUp_pressedCallback(uint8_t pinIn) {
    currentBank++;
    if(currentBank > 8) currentBank = 0;
    MIDI.sendProgramChange((currentBank<<2)+currentChan+1, podAddress);
}

/*
void
BankUp_releasedCallback(uint8_t pinIn) {
    // handle released state
}


void
BankUp_pressedDurationCallback(uint8_t pinIn, unsigned long duration) {
    // handle still pressed state
}


void
BankUp_releasedDurationCallback(uint8_t pinIn, unsigned long duration) {
    // handle released state
}
*/

void
BankDn_pressedCallback(uint8_t pinIn) {
    currentBank--;
    if(currentBank < 0)
        currentBank = 8;
    MIDI.sendProgramChange((currentBank<<2)+currentChan+1, podAddress);
}

/*
void
BankDn_releasedCallback(uint8_t pinIn) {
    // handle released state
}


void
BankDn_pressedDurationCallback(uint8_t pinIn, unsigned long duration) {
    // handle still pressed state
}


void
BankDn_releasedDurationCallback(uint8_t pinIn, unsigned long duration) {
    // handle released state
}
*/

void
WhaSw_pressedCallback(uint8_t pinIn) {
    LCD.setCursor(17, 0);
    if(statusWhaSt == ENABLED) { // Toggle the wha status
        statusWhaSt = DISABLED;
        MIDI.sendControlChange(MIDIwhaEnable, 0, podAddress);
        LCD.print("   ");
    }
    else { // statusWhaSt == DISABLED
        statusWhaSt = ENABLED;
        MIDI.sendControlChange(MIDIwhaEnable, 127, podAddress);
        LCD.print("Wha");
    }
    digitalWrite(WhaSt, statusWhaSt);
}

/*
void
WhaSw_releasedCallback(uint8_t pinIn) {
    // handle released state
}


void
WhaSw_pressedDurationCallback(uint8_t pinIn, unsigned long duration) {
    // handle still pressed state
}


void
WhaSw_releasedDurationCallback(uint8_t pinIn, unsigned long duration) {
    // handle released state
}
*/

                                            ////////////////////////
                                            // MIDI events handlers
                                            ////////////////////////
void
handleControlChange(byte channel, byte number, byte value) {
    bPODready = true;
    if(value & 0x80) 
        return; // Not a valid value;
    if(number == MIDIvolume) { // Channel Volume
        statusVolume = value;
        LCD.setCursor(12, 0);
        LCD.print("   ");
        LCD.setCursor(12, 0);
        LCD.print(statusVolume);
        return;  
    }
    if(number == MIDIwhaValue) { // Wha Value
        statusWhaVal = value;
        return;
    }
    // else
    LCD.setCursor(4, 3);
    LCD.print("   ");
    LCD.setCursor(4, 3);
    LCD.print(number);
    LCD.setCursor(12, 3);
    LCD.print("   ");
    LCD.setCursor(12, 3);
    LCD.print(value);
}


void
handleProgramChange(byte channel, byte number) {
    bPODready = true;
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
    bPODready = true;
    LCD.setCursor(0, 2);
    LCD.print("   ");
    LCD.setCursor(0, 2);
    LCD.print(size);
}


//////////////////////////////////////////////////    
// Setup LCD and Print "POD Disconnected" Message
//////////////////////////////////////////////////    
void 
setupLCD() {
    // set up the LCD
    LCD.begin(20, 4); // Number of Columns and Rows:
    LCD.setCursor(8, 1); // LCD.setCursor(col, row)
    LCD.print("POD");
    LCD.setCursor(4, 2);
    LCD.print("Disconnected");
}


/////////////////////////////////
// Print the headers on the LCD.
/////////////////////////////////
void
printLcdHeaders() {
    LCD.clear(); 
    LCD.setCursor(0, 0); // LCD.setCursor(col, row)   
    LCD.print("Pgm:");
    LCD.print(currentBank+1);
    LCD.print(char(65+currentChan));
    LCD.setCursor(8, 0);
    LCD.print("Vol:");
    LCD.print(statusVolume);
    LCD.setCursor(0, 3);
    LCD.print("CCn:");
    LCD.setCursor(8, 3);
    LCD.print("Val:");
}


///////////////////////////////////
// Configure Input and Output Pins
// and also the debounced buttons
///////////////////////////////////
void
setupPedalBoard() {
    // Register callback functions (shared, used by all buttons)
    buttonBankUp.registerCallbacks(BankUp_pressedCallback,
                                   NULL,
                                   NULL,
                                   NULL);
                                   
    buttonBankDn.registerCallbacks(BankDn_pressedCallback,
                                   NULL,
                                   NULL,
                                   NULL);

    buttonWha.registerCallbacks(WhaSw_pressedCallback,
                                NULL,
                                NULL,
                                NULL);
                                   
    // There are 20K pullup resistors built into the Atmega chip...
    pinMode(ChanA,  INPUT_PULLUP); // Channel A  Switch
    pinMode(ChanB,  INPUT_PULLUP); // Channel B  Switch
    pinMode(ChanC,  INPUT_PULLUP); // Channel C  Switch
    pinMode(ChanD,  INPUT_PULLUP); // Channel D  Switch
    pinMode(WhaVal, INPUT_PULLUP); // Wha Potentiometer
    pinMode(Volume, INPUT_PULLUP); // Volume Potentiometer
    pinMode(WhaSt,  OUTPUT);       // Wha Status Led (same pin as the internal Led)
    
    // debounced buttons setup
    buttonBankUp.setup(BankUp,
                       debounceTime,
                       InputDebounce::PIM_INT_PULL_UP_RES);
                       
    buttonBankDn.setup(BankDn,
                       debounceTime,
                       InputDebounce::PIM_INT_PULL_UP_RES);

    buttonWha.setup(WhaSw,
                    debounceTime,
                    InputDebounce::PIM_INT_PULL_UP_RES);
    
}


void
setupMIDI() {
    // Setup Various Callbacks
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleSystemExclusive(handleSysEx);
    // Start MIDI processing...
    MIDI.begin(podAddress); // Calling MIDI.begin() also activate the Through functionality 
    MIDI.turnThruOff();     // Since we don't want to act as a MIDI Through.
}


///////////////////////////////////////////
// Wait until POD is Ready and then send
// the Volume and Wha values corresponding
// to the Potentiometers positions
///////////////////////////////////////////
void
initPOD() {
    while(!bPODready) {
        delay(200); // Not too fast please !
        MIDI.read();
    }
    
    // Reflect the pedal volume value... 
    statusVolume = (analogRead(Volume) >> 4) << 1; // the analogRead() function takes 100 microseconds
    MIDI.sendControlChange(MIDIvolume, statusVolume, podAddress);
    
    // and the status of the Wha pedal
    digitalWrite(WhaSt, statusWhaSt);
    statusWhaVal = (analogRead(WhaVal) >> 4) << 1; // the analogRead() function takes 100 microseconds
    MIDI.sendControlChange(MIDIwhaValue, statusWhaVal, podAddress);
}


//////////
// Setup
//////////
void 
setup() {
    setupPedalBoard(); // Configure Input and Output Pins
    setupLCD();        // Setup LCD and Show the "POD Disconnected" Message
    setupMIDI();
    initPOD();         // Blocks until POD is Ready
    printLcdHeaders(); // Prepare the fixed headers
}


/////////
// Loop
/////////
void 
loop() {
    // Read the Channel Switches Values
    input = digitalRead(ChanA);
    if(input != statusChanA) {
        statusChanA = input;
        if(input == LOW) {
            MIDI.sendProgramChange(1+(currentBank<<2), podAddress);
            currentChan = 0;
        }
    }
    input = digitalRead(ChanB);
    if(input != statusChanB) {
        statusChanB = input;
        if(input == LOW) {
            MIDI.sendProgramChange(2+(currentBank<<2), podAddress);
            currentChan = 1;
        }
    }
    input = digitalRead(ChanC);
    if(input != statusChanC) {
        statusChanC = input;
        if(input == LOW) {
            MIDI.sendProgramChange(3+(currentBank<<2), podAddress);
            currentChan = 2;
        }
    }
    input = digitalRead(ChanD);
    if(input != statusChanD) {
        statusChanD = input;
        if(input == LOW) {
            MIDI.sendProgramChange(4+(currentBank<<2), podAddress);
            currentChan = 3;
        }
    }
    
    // Check the Banks and Wha Switches
    now = millis();
    buttonBankUp.process(now);
    buttonBankDn.process(now);
    buttonWha.process(now);

    // Wha pedal value
    if(statusWhaSt == ENABLED) {        
        input = (analogRead(WhaVal) >> 4) << 1; // analogRead() takes 100 microseconds
        if(input != statusWhaVal) {
            statusWhaVal = input;
            MIDI.sendControlChange(MIDIwhaValue, statusWhaVal, podAddress);
        }
    }
    
    // Volume pedal value
    input = (analogRead(Volume) >> 4) << 1; // analogRead() takes 100 microseconds
    if(input != statusVolume) {
        statusVolume = input;
        MIDI.sendControlChange(MIDIvolume, statusVolume, podAddress);
    }

    // Call MIDI.read() as fast as possible for real-time performance.
    MIDI.read();
    // No need to check for incoming messages: they are bound to a Callback function.
}
