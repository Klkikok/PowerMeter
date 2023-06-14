#include <avr/interrupt.h>
#include <avr/io.h>

#define TIMER1_COMPA_vect _VECTOR(11) /* Timer/Counter1 Compare Match A */


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
	uint16_t val_array[6];
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

  }
}

ISR(TIMER1_COMPA_vect)
{
  if(phase == 1)
  {
    if(mux > 5)
    {
      mux = 0;
      phase = 2;
    }

    int pin = readPins[mux];
    pack.id = packetId;
    int readValue = analogRead(pin);
    float voltage = readValue * (5.0 / 1024.0);
    pack.val_array[mux] = readValue;

    mux++;
  }
  if(phase == 2)
  {
    for(int i = 2 ; i < 6 ; i++){
      pack.val_array[i] = 0;
    }

    String line = String(pack.val_array[0]) + "," + String(pack.val_array[1]) + "," + String(pack.val_array[2]) + "," +
                String(pack.val_array[3]) + "," + String(pack.val_array[4]) + "," + String(pack.val_array[5]);

    Serial.println(line);
       
   
    for(int i = 0; i < 6; i++)
    {
      pack.val_array[i] = 0;
    }

    phase = 1;
    packetId++;
  }
}
