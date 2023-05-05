
#include <PCD8544.h> //Incluyo libreria PCD8544
#include <math.h>
#include <stdio.h>


// Declaracion de Hardware y variables necesarias
static PCD8544 lcd;
// Definimos los nombres de los pines analogicos para la lectura del voltaje
int a_input3 = 3;             //int analogInput5 = 5; // PIN A5
int a_input2 = 2;             //int analogInput4 = 4; // PIN A4
int a_input1 = 1;             //int analogInput2 = 2; // PIN A2
int a_input0 = 0;             //int analogInput0 = 0; // PIN A0
// LEDS de aviso conectados a cada pin numerado
int l_led = 12; // Alarma para canal 1
//int alarma_led_2 = 10; // Alarma para canal 2
//int alarma_led_3 = 11;
//int alarma_led_4 = 12;
// Variables para el circuito
float R1 = 25000.0;  //  R1 (200K) Valor de la resistencia R1 del divisor de tension
float R2 = 5000.0; //  R2 (1M) Valor de la resistencia R2 del divisor de tension


// Variables de almacenamiento por canal 1.
int v_analog3 = 0;                //int value5 = 0;      // Definimos la variable value
byte d_input8 = 8;      // Nombre del pin 2  //A3
byte Posi_Nega8 = 0; //byte SelectorNP = 0; // Variable recibe dato de un comparador, para detectar si es negativo o positivo V.
float v_rms3 = 0; //float Vrms5 = 0;     // Variable para almacenar el calculo del valor RMS de fuente AC
float v_out3 = 0;//float vout5 = 0.0;   // Definimos la variable Vout
float v_in3 = 0;//float vin5 = 0.0;    // Definimos la variable Vin


// Variables de almacenamiento por canal 2
int v_analog2 = 0;      // Definimos la variable value
byte d_input9 = 9;      // Nombre del pin 2
byte Posi_Nega9 = 0; // Variable recibe dato de un comparador, para detectar si es negativo o positivo V.
float v_rms2 = 0;     // Variable para almacenar el calculo del valor RMS de fuente AC
float v_out2 = 0.0;   // Definimos la variable Vout
float v_in2 = 0.0;    // Definimos la variable Vin

// Variables de almacenamiento por canal 3
int v_analog1 = 0;      // Definimos la variable value
byte d_input10 = 10;      // Nombre del pin 2
byte Posi_Nega10 = 0; // Variable recibe dato de un comparador, para detectar si es negativo o positivo V.
float v_rms1 = 0;     // Variable para almacenar el calculo del valor RMS de fuente AC
float v_out1 = 0.0;   // Definimos la variable Vout
float v_in1 = 0.0;    // Definimos la variable Vin

// Variables de almacenamiento por canal 4
int v_analog0 = 0;      // Definimos la variable value
byte d_input11 = 11;      // Nombre del pin 2
byte Posi_Nega11 = 11; // Variable recibe dato de un comparador, para detectar si es negativo o positivo V.
float v_rms0 = 0;     // Variable para almacenar el calculo del valor RMS de fuente AC
float v_out0 = 0.0;   // Definimos la variable Vout
float v_in0 = 0.0;    // Definimos la variable Vin
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Esta invertido, canal 4 =3, canal 2 =2, canal 1 =3 y canal 0 = 1.





void setup()
{
  // Modo serial:
  Serial.begin(9600); // Este debe coincidir con los demas dispositovos que conectamos

  // Declaracion de puertos de entrada
  pinMode(a_input3, INPUT); // Activamos el input del pin analógico A3
  pinMode(a_input2, INPUT); // Activamos el input del pin analógico A2
  pinMode(a_input1, INPUT); // Activamos el input del pin analógico A1
  pinMode(a_input0, INPUT); // Activamos el input del pin analógico A0

  // Declaracion de puertos de entrada para trabajar en modo digital
  pinMode(7, INPUT);     // Pin 13 como entrada  DC antes, ahora es pin 7 entrada para el boton dc o ac.
  pinMode(d_input8, INPUT);  // Pin2 como entrada, ahora es 8
  pinMode(d_input9, INPUT);  // Pin1 como entrada, ahora es 9
  pinMode(d_input10, INPUT); // Pin8 como entrada, ahora es 10
  pinMode(d_input11, INPUT); // Pin0 como entrada, ahora es 11

  // Ajustes del display
  lcd.begin(84, 48); // Activamos LCD
  lcd.print("Voltimetro");
  lcd.setCursor(15, 1); // Posicionamos el cursor en el LCD
  lcd.print("Canales 4");
  // Establecer Pines como salidas para los LEDS que  activan cuando el voltaje excede 20V y -20V
  pinMode(l_led, OUTPUT);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  
  // ************** PROGRAMACION PARA CANAL 1 **********************
  // Selectores Negativo/positivo de la senal DC
  Posi_Nega8 = digitalRead(d_input8); // leemos el pin digital 8 reconocido como PIN_2
  bool estadoPulsador3 = digitalRead(2); // leemos el pin digital 13, voltaje para seleccionar entre AC/DC, ahora es el pin 2
  // Establecer lecturas de los pines analogicos y guardarlos en las variables value_n
  v_analog3 = analogRead(a_input3); // Leemos el pin analógico A0
  // Voltage de salida/entrada para cada canal
  v_out3 = (v_analog3* 5.0145) / 1024; // Cálculo para obtener el Vout
  v_in3 = v_out3 / (R2 / (R1 + R2));  // Cálculo para obtener Vin del divisor de tensión
  if (estadoPulsador3 == LOW)        // Condicional para entrar en el modo DC
  {
    if (Posi_Nega8 == LOW) // Si el voltaje de la fuente es positivo, va positivo.
    {                    // Condiciones de alarma de LED
      if (v_in3 > 24)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
      // Para mostrar en pantalla
      lcd.setCursor(12, 2); // Posicionamos el cursor en el LCD
      lcd.print("V1 = ");   // Mostramos el texto en el LCD
      lcd.print(v_in3); // Mostramos el valor del Vin en el LCD
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      Serial.print("V1 ");
      Serial.println(v_in3);
      delay(500);
    }
    else
    {
      if (Posi_Nega8 == HIGH) // Si el voltaje de la fuente es negativo
      {                    // Condiciones de alarma de LED
        if (v_in3 * -1 < -24)
        {
          digitalWrite(l_led,HIGH);
        }
        else
        {
          digitalWrite(l_led, LOW);
        }
        // Para mostrar en pantalla
        lcd.setCursor(12, 2);  // Posicionamos el cursor en el LCD
        lcd.print("V1 ");     // Mostramos el texto en el LCD
        lcd.print(v_in3 * -1); // Mostramos el valor del Vin en el LCD
        // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
        Serial.print("V1 ");
        Serial.println(v_in3 * -1);
        delay(500);
      }
    }
  }

  if (estadoPulsador3 == HIGH) // Condicional para entrar en el modo AC
  {
    // Buscamos el voltaje mas alto
    if (v_in3 > v_rms3)
    {
      v_rms3 = v_in3;
      if (v_rms3*0.7071 > 17)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
     
    }
     // Seteamos en la pantalla LCD
      lcd.setCursor(12, 2);            // Posicionamos el cursor en el LCD
      lcd.print("V1 RMS:");               // Mostramos el texto en el LCD
      lcd.print(v_rms3 *0.7071); // Al voltaje mas alto le colocamos la formula de Voltaje RMS -> Vp/sqrt(2)
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      //Serial.print("Vrms1 ");
      //Serial.println(5 * Vrms5 / sqrt(2));
      //delay(500);
  }



  // CANAL 2 

  Posi_Nega9 = digitalRead(d_input9); // Revisamos si la tension es positiva o negativa
  bool estadoPulsador2 = digitalRead(2); // leemos el pin digital 13, voltaje para seleccionar entre AC/DC
  // Establecer lecturas de los pines analogicos y guardarlos en las variables value_n
  v_analog2 = analogRead(a_input2); // Leemos el pin analógico A0
  // Voltage de salida/entrada para cada canal
  v_out2 = (v_analog2* 5.0145) / 1024; // Cálculo para obtener el Vout
  v_in2 = v_out2 / (R2 / (R1 + R2));  // Cálculo para obtener Vin del divisor de tensión
  if (estadoPulsador2 == LOW)        // Condicional para entrar en el modo DC
  {
    if (Posi_Nega9 == LOW) // Si el voltaje de la fuente es positivo
    {                    // Condiciones de alarma de LED
      if (v_in2 * 5 > 24)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
      // Para mostrar en pantalla
      lcd.setCursor(12, 3); // Posicionamos el cursor en el LCD
      lcd.print("V2= ");   // Mostramos el texto en el LCD
      lcd.print(v_in2 * 5); // Mostramos el valor del Vin en el LCD
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      Serial.print("V2 ");
      Serial.println(v_in2 * 5);
      delay(500);
    }
    else
    {
      if (Posi_Nega9 == HIGH) // Si el voltaje de la fuente es negativo
      {                    // Condiciones de alarma de LED
        if (v_in2 * -5 < -24)
        {
          digitalWrite(l_led, HIGH);
        }
        else
        {
          digitalWrite(l_led, LOW);
        }
        // Para mostrar en pantalla
        lcd.setCursor(12, 3);  // Posicionamos el cursor en el LCD
        lcd.print("V2 ");     // Mostramos el texto en el LCD
        lcd.print(v_in2 * -5); // Mostramos el valor del Vin en el LCD
        // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
        Serial.print("V2 ");
        Serial.println(v_in2 * -5);
        delay(500);
      }
    }
  }

  if (estadoPulsador2 == HIGH) // Condicional para entrar en el modo AC
  {
    // Buscamos el voltaje mas alto
    if (v_in2 > v_rms2)
    {
      v_rms2 = v_in2;
      if (v_rms2*5*0.7071 > 17)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
      // Seteamos en la pantalla LCD
      lcd.setCursor(12, 3);            // Posicionamos el cursor en el LCD
      lcd.print("V2 RMS:");               // Mostramos el texto en el LCD
      lcd.print(5 * v_rms2 *0.7071); // Al voltaje mas alto le colocamos la formula de Voltaje RMS -> Vp/sqrt(2)
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      //Serial.print("Vrms2: ");
      //Serial.println(Vrms4);
      //delay(500);
    }
  }
  // ************** PROGRAMACION PARA CANAL 3 **********************
  // Selectores Negativo/positivo de la senal DC
  Posi_Nega10 = digitalRead(d_input10); // leemos el pin digital 2 reconocido como PIN_2
  bool estadoPulsador1 = digitalRead(2); // leemos el pin digital 13, voltaje para seleccionar entre AC/DC
  // Establecer lecturas de los pines analogicos y guardarlos en las variables value_n
  v_analog1 = analogRead(a_input1); // Leemos el pin analógico A0
  // Voltage de salida/entrada para cada canal
  v_out1 = (v_analog1 * 5.0145) / 1024; // Cálculo para obtener el Vout
  v_in1 = v_out1 / (R2 / (R1 + R2));  // Cálculo para obtener Vin del divisor de tensión
  if (estadoPulsador1 == LOW)        // Condicional para entrar en el modo DC
  {
    if (Posi_Nega10 == LOW) // Si el voltaje de la fuente es positivo
    {                    // Condiciones de alarma de LED
      if (v_in1 * 5 > 24)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
      // Para mostrar en pantalla
      lcd.setCursor(12, 4); // Posicionamos el cursor en el LCD
      lcd.print("V3= ");   // Mostramos el texto en el LCD
      lcd.print(v_in1 * 5); // Mostramos el valor del Vin en el LCD
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      Serial.print("V3 ");
      Serial.println(v_in1* 5);
      delay(500);
    }
    else
    {
      if (Posi_Nega10 == HIGH) // Si el voltaje de la fuente es negativo
      {                    // Condiciones de alarma de LED
        if (v_in1 * -5 < -24)
        {
          digitalWrite(l_led, HIGH);
        }
        else
        {
          digitalWrite(l_led, LOW);
        }
        // Para mostrar en pantalla
        lcd.setCursor(12, 4);  // Posicionamos el cursor en el LCD
        lcd.print("V3 ");     // Mostramos el texto en el LCD
        lcd.print(v_in1 * -1); // Mostramos el valor del Vin en el LCD
        // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
        Serial.print("V3 ");
        Serial.println(v_in1 * -5);
        delay(500);
      }
    }
  }

  if (estadoPulsador1 == HIGH) // Condicional para entrar en el modo AC
  {
    // Buscamos el voltaje mas alto
    if (v_in1 > v_rms1)
    {
      v_rms1 = v_in1;
      if (v_rms1*5*0.7071 > 17)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
      // Seteamos en la pantalla LCD
      lcd.setCursor(12, 4);            // Posicionamos el cursor en el LCD
      lcd.print("V3 RMS:");               // Mostramos el texto en el LCD
      lcd.print(5 * v_rms1 *0.7071); // Al voltaje mas alto le colocamos la formula de Voltaje RMS -> Vp/sqrt(2)
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      //Serial.print("Vrms3: ");
      //Serial.println(Vrms2);
      //delay(500);
    }
  }
  // ************** PROGRAMACION PARA CANAL 4 **********************
   // Selectores Negativo/positivo de la senal DC
  Posi_Nega11 = digitalRead(d_input11); // leemos el pin digital 2 reconocido como PIN_2
  bool estadoPulsador0 = digitalRead(2); // leemos el pin digital 13, voltaje para seleccionar entre AC/DC
  // Establecer lecturas de los pines analogicos y guardarlos en las variables value_n
  v_analog0 = analogRead(a_input0); // Leemos el pin analógico A0
  // Voltage de salida/entrada para cada canal
  v_out0 = (v_analog0 * 5.0145) / 1024; // Cálculo para obtener el Vout
  v_in0 = v_out0 / (R2 / (R1 + R2));  // Cálculo para obtener Vin del divisor de tensión
  if (estadoPulsador0 == LOW)        // Condicional para entrar en el modo DC
  {
    if (Posi_Nega11 == LOW) // Si el voltaje de la fuente es positivo
    {                    // Condiciones de alarma de LED
      if (v_in0 * 5 > 24)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
      // Para mostrar en pantalla
      lcd.setCursor(12, 5); // Posicionamos el cursor en el LCD
      lcd.print("V4= ");   // Mostramos el texto en el LCD
      lcd.print(v_in0);
      //lcd.print(v_in0 * 5); // Mostramos el valor del Vin en el LCD
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      Serial.print("V4 ");
      Serial.print(v_in0 * 5);
      delay(500);
    }
    else
    {
      if (Posi_Nega11 == HIGH) // Si el voltaje de la fuente es negativo
      {                    // Condiciones de alarma de LED
        if (v_in0 * -5 < -24)
        {
          digitalWrite(l_led, HIGH);
        }
        else
        {
          digitalWrite(l_led, LOW);
        }
        // Para mostrar en pantalla
        lcd.setCursor(12, 5);  // Posicionamos el cursor en el LCD
        lcd.print("V4 ");     // Mostramos el texto en el LCD
        lcd.print(-1* v_in0); // Mostramos el valor del Vin en el LCD
        // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
        Serial.print("V4 ");
        Serial.println(v_in0 * -5);
        delay(500);
     }
    }
  }

  if (estadoPulsador0 == HIGH) // Condicional para entrar en el modo AC
  {
    // Buscamos el voltaje mas alto
    if (v_in0 > v_rms0)
    {
      v_rms0 = v_in0;
      if (v_rms0*5*0.7071 > 17)
      {
        digitalWrite(l_led, HIGH);
      }
      else
      {
        digitalWrite(l_led, LOW);
      }
      // Seteamos en la pantalla LCD
     
      // Pasamos al serial monitor que eventualmente lo pasaremos a un archivo csv
      //Serial.print("Vrms4: ");
      //Serial.println(Vrms0);
      delay(500);
    }
     lcd.setCursor(12, 5);            // Posicionamos el cursor en el LCD
      lcd.print("V4 RMS:");               // Mostramos el texto en el LCD
      lcd.print(5 * v_rms0*0.7071); // Al voltaje mas alto le colocamos la formula de Voltaje RMS -> Vp/sqrt(2)
  }
   if (estadoPulsador0 == HIGH)
   {
      Serial.print("Vrms1 ");
      Serial.println(5 * v_rms3 / sqrt(2));
      Serial.print("Vrms2 ");
      Serial.println(v_rms2*5*0.7071);
      Serial.print("Vrms3 ");
      Serial.println(v_rms1*5*0.7071);
      Serial.print("Vrms4 ");
      Serial.println(v_rms0*5*0.7071);
   }
  // ********************************PRUEBA****************************************
  // Esto de aqui abajo es meramente de prueba, para la comunicacion serial se deben meter en los condicionales del switch AC\DC (yo lo hago)
 // Serial.print("V: ");
  //Serial.println(vin5);
} // Final de void loop()