/**
 * YM2612 test code for AVR.
 * 
 * This program is a simple test code for the YM2612 FM sound chip using an AVR ATmega328p mcu.
 * This program configure the YM2612 to sound like a "grand piano" and play note on / note off in loop.
 * For more informations about wiring please see: http://en.wikipedia.org/wiki/Yamaha_YM2612
 * For more informations about YM2612 registers please see: http://www.smspower.org/maxim/Documents/YM2612
 *
 * @remarks This test code is heavly based on Furrtek's YM2612 test code. Big thanks Furrtek for the help !
 * @warning This test code is made to run on an ATmega328/ATmega168 mcu with a 16MHz external crystal.
 * 
 * @author Fabien Batteix <skywodd@gmail.com>
 * @link http://skyduino.wordpress.com My Blog about electronics
 */

/* Dependencies */
#include <avr/io.h>     // For I/O and other AVR registers
#include <util/delay.h> // For timing

/* Pinmap */
#define YM_IC (5) // PC5 (= pin A5 for Arduino UNO)
#define YM_CS (4) // PC4 (= pin A4 for Arduino UNO)
#define YM_WR (3) // PC3 (= pin A3 for Arduino UNO)
#define YM_RD (2) // PC2 (= pin A2 for Arduino UNO)
#define YM_A0 (1) // PC1 (= pin A1 for Arduino UNO)
#define YM_A1 (0) // PC0 (= pin A0 for Arduino UNO)
#define YM_CTRL_DDR DDRC
#define YM_CTRL_PORT PORTC
#define YM_DATA_DDR DDRA
#define YM_DATA_PORT PORTA // Whole PORT A for data bus (= pins D0 to D7 for Arduino UNO)
#define YM_MCLOCK (5) // PB1 = OC1A (= pin D9 for Arduino UNO)
#define YM_MCLOCK_DDR DDRB

/* ----- BIG WARNING FOR ARDUINO USERS -----
 * Normally ICSP port is used to program the AVR chip (see the makefile for details). 
 * So, if you use an arduino UNO board to run this test code BE VERY CAREFULL.
 * If you don't known what you make you will be in trouble.
 *
 * To avoid problems you can compile and upload this code using the Arduino IDE BUT you will need to disconnect pins Rx/Tx before upload !
 */

/**
 * Send raw data to the YM2612
 * 
 * @author Furrtek
 * @param data Data to write
 */
static void write_ym(uint8_t data) {
  YM_CTRL_PORT &= ~_BV(YM_CS); // CS LOW
  YM_DATA_PORT = data;
  _delay_us(1);
  YM_CTRL_PORT &= ~_BV(YM_WR); // Write data
  _delay_us(5);
  YM_CTRL_PORT |= _BV(YM_WR);
  _delay_us(5);
  YM_CTRL_PORT |= _BV(YM_CS); // CS HIGH
}

/**
 * Write data into a specific register of the YM2612
 *
 * @author Furrtek
 * @param reg Destination register address
 * @param data Data to write
 */
static void setreg(uint8_t reg, uint8_t data) {
  YM_CTRL_PORT &= ~_BV(YM_A0); // A0 low (select register)
  write_ym(reg);
  YM_CTRL_PORT |= _BV(YM_A0);  // A0 high (write register)
  write_ym(data);
}

void parseNPlay(unsigned char data[], int length)
{
  for (unsigned int i = 0; i < length; i += 3) 
  {
    setreg(data[i+1], data[i+2]);
  }
}

void setPianoTest()
{
  //YM2612 Test Code
  setreg(0x22, 0x00); // LFO off
  setreg(0x27, 0x00); // Note off (channel 0)
  setreg(0x28, 0x01); // Note off (channel 1)
  setreg(0x28, 0x02); // Note off (channel 2)
  setreg(0x28, 0x04); // Note off (channel 3)
  setreg(0x28, 0x05); // Note off (channel 4)
  setreg(0x28, 0x06); // Note off (channel 5)
  setreg(0x2B, 0x00); // DAC off
  setreg(0x30, 0x71); //
  setreg(0x34, 0x0D); //
  setreg(0x38, 0x33); //
  setreg(0x3C, 0x01); // DT1/MUL
  setreg(0x40, 0x23); //
  setreg(0x44, 0x2D); //
  setreg(0x48, 0x26); //
  setreg(0x4C, 0x00); // Total level
  setreg(0x50, 0x5F); //
  setreg(0x54, 0x99); //
  setreg(0x58, 0x5F); //
  setreg(0x5C, 0x94); // RS/AR 
  setreg(0x60, 0x05); //
  setreg(0x64, 0x05); //
  setreg(0x68, 0x05); //
  setreg(0x6C, 0x07); // AM/D1R
  setreg(0x70, 0x02); //
  setreg(0x74, 0x02); //
  setreg(0x78, 0x02); //
  setreg(0x7C, 0x02); // D2R
  setreg(0x80, 0x11); //
  setreg(0x84, 0x11); //
  setreg(0x88, 0x11); //
  setreg(0x8C, 0xA6); // D1L/RR
  setreg(0x90, 0x00); //
  setreg(0x94, 0x00); //
  setreg(0x98, 0x00); //
  setreg(0x9C, 0x00); // Proprietary
  setreg(0xB0, 0x32); // Feedback/algorithm
  setreg(0xB4, 0xC0); // Both speakers on
  setreg(0x28, 0x00); // Key off
  setreg(0xA4, 0x22); // 
  setreg(0xA0, 0x69); // Set frequency
}

/** Program entry point */
int main(void) {

unsigned char rawData[149] = {
  0x52, 0x22, 0x08, 0x52, 0x27, 0x00, 0x52, 0xB4, 0xC0, 0x52, 0xB5, 0xC0,
  0x52, 0xB6, 0xC0, 0x53, 0xB4, 0xC0, 0x53, 0xB5, 0xC0, 0x53, 0xB6, 0xC0,
  0x52, 0x28, 0x00, 0x52, 0x28, 0x01, 0x52, 0x28, 0x02, 0x52, 0x28, 0x03,
  0x52, 0x28, 0x04, 0x52, 0x28, 0x05, 0x52, 0x28, 0x06, 0x50, 0x9F, 0x50,
  0xBF, 0x50, 0xDF, 0x50, 0xFF, 0x52, 0x30, 0x01, 0x52, 0x40, 0x07, 0x52,
  0x50, 0x1F, 0x52, 0x60, 0x09, 0x52, 0x70, 0x00, 0x52, 0x80, 0xFF, 0x52,
  0x90, 0x00, 0x52, 0x34, 0x01, 0x52, 0x44, 0x11, 0x52, 0x54, 0x1F, 0x52,
  0x64, 0x09, 0x52, 0x74, 0x00, 0x52, 0x84, 0xFF, 0x52, 0x94, 0x00, 0x52,
  0x38, 0x01, 0x52, 0x48, 0x08, 0x52, 0x58, 0x1F, 0x52, 0x68, 0x09, 0x52,
  0x78, 0x00, 0x52, 0x88, 0xFF, 0x52, 0x98, 0x00, 0x52, 0x3C, 0x00, 0x52,
  0x4C, 0x24, 0x52, 0x5C, 0x1F, 0x52, 0x6C, 0x09, 0x52, 0x7C, 0x00, 0x52,
  0x8C, 0xFF, 0x52, 0x9C, 0x00, 0x52, 0xB0, 0x24, 0x52, 0xA4, 0x13, 0x52,
  0xA0, 0x8E, 0x52, 0x28, 0xF0
};

/*There are 3 registers for PORTB
  PORTB
  DDRB
  PINB

  PORTB controls the drive state of the pins if set as an output via DDRB, and controls the enabling of the pull-up if configured as an input via DDRB.

  DDRB controls if a pin is used as an output (bit=1), or as an input (bit=0)

  PINB is read-only, and returns the electrical state sensed at the pin.

  To set up a pin as an input, with the pullup enabled you only do the following. (using PB0 as an example here)

  DDRB &= ~(1<<0); // configure PB0 as an input
  PORTB |= (1<<0); // enable the pull-up on PB0

  to read the pin you simply do the following:
  myVar = PINB & (1<<0); // read the state of PB0 only
*/

  //Will be using port E for the input buttons
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  /* Pins setup */
  YM_CTRL_DDR |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD) | _BV(YM_A0) | _BV(YM_A1);  // Coty: Check if this is a problem when swapping to MEGA, because of port sizes. may know more after testing on uno
  YM_DATA_DDR = 0xFF;
  YM_MCLOCK_DDR |= _BV(YM_MCLOCK);
  YM_CTRL_PORT |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD); /* IC, CS, WR and RD HIGH by default */
  YM_CTRL_PORT &= ~(_BV(YM_A0) | _BV(YM_A1)); /* A0 and A1 LOW by default */
  
  /* F_CPU / 2 clock generation */
  TCCR1A = _BV(COM1A0);            /* Toggle OCA1 on compare match */
  TCCR1B = _BV(WGM12) | _BV(CS10); /* CTC mode with prescaler /1 */
  TCCR1C = 0;                      /* Flag reset */
  TCNT1 = 0;                       /* Counter reset */
  OCR1A = 0;                       /* Divide base clock by two */
  
  /* Reset YM2612 */
  YM_CTRL_PORT &= ~_BV(YM_IC);
  _delay_ms(10);
  YM_CTRL_PORT |= _BV(YM_IC);
  _delay_ms(10);

  setPianoTest();

  parseNPlay(rawData, 149);
 
   /* Program loop */
  for(;;) {
    _delay_ms(1000);
    setreg(0x28, 0xF0); // Key on
    _delay_ms(1000);
    setreg(0x28, 0x00); // Key off
  }
  
  /* Compiler fix */
  return 0;
}
