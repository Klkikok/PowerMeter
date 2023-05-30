#include <avr/interrupt.h>
#include <avr/io.h>
#define INT0_vect _VECTOR(1) /* External Interrupt Request 0 */
#define INT1_vect _VECTOR(2) /* External Interrupt Request 1 */
#define PCINT0_vect _VECTOR(3) /* Pin Change Interrupt Request 0 */
#define PCINT1_vect _VECTOR(4) /* Pin Change Interrupt Request 1 */
#define PCINT2_vect _VECTOR(5) /* Pin Change Interrupt Request 2 */
#define WDT_vect _VECTOR(6) /* Watchdog Time-out Interrupt */
#define TIMER2_COMPA_vect _VECTOR(7) /* Timer/Counter2 Compare Match A */
#define TIMER2_COMPB_vect _VECTOR(8) /* Timer/Counter2 Compare Match A */
#define TIMER2_OVF_vect _VECTOR(9) /* Timer/Counter2 Overflow */
#define TIMER1_CAPT_vect _VECTOR(10) /* Timer/Counter1 Capture Event */
#define TIMER1_COMPA_vect _VECTOR(11) /* Timer/Counter1 Compare Match A */
#define TIMER1_COMPB_vect _VECTOR(12) /* Timer/Counter1 Compare Match B */
#define TIMER1_OVF_vect _VECTOR(13) /* Timer/Counter1 Overflow */
#define TIMER0_COMPA_vect _VECTOR(14) /* TimerCounter0 Compare Match A */
#define TIMER0_COMPB_vect _VECTOR(15) /* TimerCounter0 Compare Match B */
#define TIMER0_OVF_vect _VECTOR(16) /* Timer/Couner0 Overflow */
#define SPI_STC_vect _VECTOR(17) /* SPI Serial Transfer Complete */
#define USART_RX_vect _VECTOR(18) /* USART Rx Complete */
#define USART_UDRE_vect _VECTOR(19) /* USART, Data Register Empty */
#define USART_TX_vect _VECTOR(20) /* USART Tx Complete */
#define ADC_vect _VECTOR(21) /* ADC Conversion Complete */
#define EE_READY_vect _VECTOR(22) /* EEPROM Ready */
#define ANALOG_COMP_vect _VECTOR(23) /* Analog Comparator */
#define TWI_vect _VECTOR(24) /* Two-wire Serial Interface */
#define SPM_READY_vect _VECTOR(25) /* Store Program Memory Read */

int analogPin0 = 0;
int analogPin1 = 1;
int analogPin2 = 2;
unsigned int reload = 0x5DB;
volatile int mux = 0;
int readPins[] = {0, 1, 2};
int phase = 1;
int packetId = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = reload;
  TCCR1B = (1<<WGM12) | (1<<CS12);
  TIMSK1 = (1<<OCIE1A);
  sei();
}

#define MAGIC 0xBABADEDA

struct sample_packet
{
	uint32_t magic;
	uint32_t id;
	uint16_t val_array[3];
	uint8_t err;

};

volatile sample_packet pack = {
	.magic = MAGIC,
	.id = 0,
	.val_array = {0},
	.err = 0
};

void loop() {
  // put your main code here, to run repeatedly:
  while(1)
  {
    delay(1000);
    int pin0 = analogRead(analogPin0);
    float voltage = pin0 * (5.0 / 1024.0);
    // Serial.println(voltage);
    delay(1000);
    int pin1 = analogRead(analogPin1);
    voltage = pin1 * (5.0 / 1024.0);
    // Serial.println(voltage);
    delay(1000);
    int pin2 = analogRead(analogPin2);
    voltage = pin2 * (5.0 / 1024.0);
    // Serial.println(voltage);
  }
  


}

ISR(TIMER1_COMPA_vect)
{
  if(phase == 1)
  {
    if(mux > 2)
    {
      mux = 0;
      phase = 2;
    }

    int pin = readPins[mux];
    pack.id = packetId;
    int readValue = analogRead(pin);
    float voltage = readValue * (5.0 / 1024.0);
    pack.val_array[mux] = voltage;

    mux++;
  }
  if(phase == 2)
  {
    Serial.print("Packed no:");
    Serial.println(packetId);
    Serial.write((byte *)&pack, sizeof(sample_packet));
    phase = 1;
    packetId++;
  }
  

}
