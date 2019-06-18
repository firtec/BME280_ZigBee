/**********************************************************************
** Descripción  : Recibe los datos de un sensr BME280 por ZigBee            
**                usando el controlador MRF24J40MA a 2.4 Ghz
**                
**  Target       : Arduino UNO
**  ToolChain    : Arduino IDE 1.8.9 bajo Linux Debian
**  www.firtec.com.ar 
**********************************************************************/
#include <SPI.h>
#include <mrf24j.h>

#define pin_reset   6
#define pin_cs      10  // Pin CS para ATmega 328
#define pin_ISR     2   // Pin IRQ para ATmega 328

Mrf24j mrf(pin_reset, pin_cs, pin_ISR);

void setup() {
  Serial.begin(9600);
  
  mrf.reset();
  mrf.init();
  
  mrf.set_pan(0x40F3);  // Identificador de la red ZigBee
  mrf.address16_write(0x4202); // Identificador del dispositivo en la red
  attachInterrupt(0, Rutina_ISR, CHANGE); // IRQ 0 equivale al pin 2(INT0) en ATmega 328
  interrupts(); // Interrupciones activas
}

void Rutina_ISR() {
  mrf.interrupt_handler(); // mrf24 manejador de interrupciones del modulo zigbee
}

void loop() {
    mrf.check_flags(&vector_rx, &vector_tx);
}

void vector_rx() {
  Serial.println("");
   
    for (int i = 0; i < mrf.rx_datalength(); i++) {
        Serial.write(mrf.get_rxinfo()->rx_data[i]);
        }
}

void vector_tx() {
    // IRQ del transmisor (no se usa en este ejemplo)
}
