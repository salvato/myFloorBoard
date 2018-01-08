
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
// Pin No.  IN Signal name  THRU Signal name  Out Signal name
// 1        No Connect      No Connect        No Connect
// 2        No Connect      Shield            Shield
// 3        No Connect      No Connect        No Connect
// 4        IN+             +5v               +5v
// 5        IN-             IN                IN


// MIDI standard: 31250 baud, 1 start bit, 8 data, no parity, 1 stop bit. 
// On the transmit side:
// the logic-level output from the transmitting device's UART goes to (typically) an open-collector transistor 
// driver, the other side of the output being a resistor-limited power feed from +Ve in the sending device.

// On the receive side:
// an opto-coupler takes in the open-collector drive from the remote device and converts it back to logic level 
// for the input of the UART.

// This scheme ensures that the only 'connection' between the two devices is via the light beams inside the optos 
// at each receiving end of the link, and eliminates hum-loops which might arise if there were two separate ground
// connections between the two items of equipment (one for the audio, and one for the comms data).


// Serial port communicates on digital pins 0 (RX) and 1 (TX) as well as with the computer via USB. 
// Thus, since we use these functions, we cannot also use pins 0 and 1 for digital input or output.


long    baudRate       = 31250; // The MIDI Baud Rate
int     maxLen         = 200;   // Maximum input string length
byte    inputString[200];       // A string to hold incoming data
boolean stringComplete = false; // Whether the string is complete
int     iPos;                   // The current position in the receiving buffer
byte    podAddress     = 0;     // This is the MIDI address #1 (the POD default)


void 
setup() {
  Serial.begin(baudRate, SERIAL_8N1); // 8 data bits, no parity, one stop bit.
}


// SerialEvent occurs whenever a new data comes in the hardware serial RX.
// This routine is run between each time loop() runs, so using delay inside loop can delay response.
// Multiple bytes of data may be available.
void 
serialEvent() {
  while(Serial.available()) {
    byte inChar = (byte)Serial.read();
    inputString[iPos++] = inChar;
    if (inChar == EOS || iPos == maxLen) {
      stringComplete = true;
    }
  }
}



void 
loop() {
  serialEvent(); //call the function
}
