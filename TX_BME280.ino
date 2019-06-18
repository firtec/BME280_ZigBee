/**********************************************************************
** Descripción  : Lee un sensor BME280 y envía los datos por ZigBee            
**                usando el controlador MRF24J40MA a 2.4 Ghz
**                
**  Target       : Arduino UNO
**  ToolChain    : Arduino IDE 1.8.9 bajo Linux Debian
**  www.firtec.com.ar 
**********************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <mrf24j.h>

#include "SparkFunBME280.h"
BME280 Sensor; 

#define pin_reset 6
#define pin_cs 10   // Pin CS por defecto para ATmega 328
#define pin_irq  2  // IRQ para ATmega 328


Mrf24j mrf(pin_reset, pin_cs, pin_irq);
long tiempo_anterior;
long temporizador = 3000;
char buffer[20]=" ";
byte contador = 0;

void setup()
{
  Serial.begin(9600);
  mrf.reset();  // Inicia el reset del modulo ZegBee
  mrf.init();   // Configuración inicial del modulo
  mrf.set_pan(0x40F3);  // ID de la red ZigBee
  Wire.begin();

  Sensor.setI2CAddress(0x76); //Connect to a second sensor
  if(Sensor.beginI2C() == false) Serial.println("Sensor ERROR!!");
}

void loop(){
     unsigned long tiempo_actual = millis();
    if (tiempo_actual - tiempo_anterior > temporizador) {
        tiempo_anterior = tiempo_actual;
  
   switch(contador){
   case 0:{
        dtostrf(Sensor.readTempC(),2,1,buffer);
        mrf.send16(0x4202, buffer);  // Envía buffer al nodo destino (0x4202)
        Serial.println(buffer);
        contador++; 
        break; 
        }
        
       case 1:{       
        dtostrf(Sensor.readFloatHumidity(),2,1,buffer);
        mrf.send16(0x4202, buffer); // Envía buffer al nodo destino (0x4202)
        Serial.println(buffer);
        contador++;
        break; 
        }
       case 2:{
         dtostrf((Sensor.readFloatPressure()/100),4,0,buffer);
         mrf.send16(0x4202, buffer);   // Envía buffer al nodo destino (0x4202)
         Serial.println(buffer);
         contador = 0;
         break;  
       }
    }           
  }
}
