#include <PCD8544.h> //Incluyo libreria PCD8544
#include <math.h>
#include <stdio.h>


// Declaracion de Hardware y variables necesarias
static PCD8544 lcd;
//LEDS para activar alarmas
int l_led1 = 12; // LED canal 1
int l_led2 = 13; // LED canal 2
int l_led3 = 18; // LED canal 3
int l_led4 = 19; // LED canal 4

int a_input3 = 3; //Entrada analogica canal 1
int a_input2 = 2; //Entrada analogica canal 2
int a_input1 = 1; //Entrada analogica canal 3
int a_input0 = 0; //Entrada analogica canal 4

// Variables para el circuito
float R1 = 25000.0;  //  R1 (25K) Division de tension
float R2 = 5000.0; //  R2 (5k)    Division de tension


// Variables analogicas
int v_analog3 = 0; //Variable analogica canal 1 
int v_analog2 = 0; //Variable analogica canal 2 
int v_analog1 = 0; //Variable analogica canal 3  
int v_analog0 = 0; //Variable analogica canal 4

//Entradas digitales
byte d_input8 = 8;      // Canal 1
byte d_input9 = 9;      // Canal 2
byte d_input10 = 10;    // Canal 3
byte d_input11 = 11;    // Canal 4

//Entrada positiva o negativa

byte Posi_Nega8 = 0; //byte SelectorNP = 0; // Variable recibe dato de un comparador, para detectar si es negativo o positivo V.
byte Posi_Nega9 = 0; 
byte Posi_Nega10 = 0; // Variable recibe dato de un comparador, para detectar si es negativo o positivo V.
byte Posi_Nega11 = 0; // Variable recibe dato de un comparador, para detectar si es negativo o positivo V.

//Variables Vrms

float v_rms3 = 0; //Canal 1
float v_rms2 = 0; //Canal 2
float v_rms1 = 0; //Canal 3
float v_rms0 = 0; //Canal 4

//Variables Vout
float v_out3 = 0; //Canal 1
float v_out2 = 0; //Canal 2
float v_out1 = 0; //Canal 3
float v_out0 = 0; //Canal 4

//Varaibles Vin
float v_in3 = 0; //Canal 1
float v_in2 = 0; //Canal 2
float v_in1 = 0; //Canal 3
float v_in0 = 0; //Canal 4

//float v_out0 = 0.0;   // Definimos la variable Vout
//float v_in0 = 0.0;    // Definimos la variable Vin
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Esta invertido, canal 4 =3, canal 2 =2, canal 1 =3 y canal 0 = 1.





void setup()
{
 
   pinMode(2, INPUT);  
  // Declaracion de puertos de entrada
  pinMode(a_input3, INPUT); // Activamos el input del pin analógico A3
  pinMode(a_input2, INPUT); // Activamos el input del pin analógico A2
  pinMode(a_input1, INPUT); // Activamos el input del pin analógico A1
  pinMode(a_input0, INPUT); // Activamos el input del pin analógico A0

  // Declaracion de puertos de entrada para trabajar en modo digital    
  pinMode(d_input8, INPUT);  // Pin8 como entrada
  pinMode(d_input9, INPUT);  // Pin9 como entrada
  pinMode(d_input10, INPUT); // Pin10 como entrada
  pinMode(d_input11, INPUT); // Pin11 como entrada

  // Ajustes del display
  lcd.begin(84, 48); // LCD ON
  lcd.print("Voltimetro");
  lcd.setCursor(15, 1); // LCD
  lcd.print("Canales 4");
  
  //Pines LED como salidas
  pinMode(l_led1, OUTPUT);
  pinMode(l_led2, OUTPUT);
  pinMode(l_led3, OUTPUT);
  pinMode(l_led4, OUTPUT);

  Serial.begin(9600); 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  
//Canal 1
  Posi_Nega8 = digitalRead(d_input8); 
  bool estadoPulsador3 = digitalRead(2); 
 
  v_analog3 = analogRead(a_input3);
  
  v_out3 = (v_analog3* 5.0145) / 1024; 
  v_in3 = v_out3 / (R2 / (R1 + R2));  
  if (estadoPulsador3 == LOW)        
  {
    if (Posi_Nega8 == LOW) 
    {                    
      if (v_in3 > 24)
      {
        digitalWrite(l_led1, HIGH);
      }
      else
      {
        digitalWrite(l_led1, LOW);
      }
      
      lcd.setCursor(12, 2); 
      lcd.print("V1 = ");   
      lcd.print(v_in3); 
      
      Serial.print("V1 ");
      Serial.println(v_in3);
      delay(500);
    }
    else
    {
      if (Posi_Nega8 == HIGH) 
      {                    
        if (v_in3 * -1 < -24)
        {
          digitalWrite(l_led1,HIGH);
        }
        else
        {
          digitalWrite(l_led1, LOW);
        }
        
        lcd.setCursor(12, 2);  
        lcd.print("V1 ");     
        lcd.print(v_in3 * -1); 
        
        Serial.print("V1 ");
        Serial.println(v_in3 * -1);
        delay(500);
      }
    }
  }

  if (estadoPulsador3 == HIGH) 
  {
   
    if (v_in3 > v_rms3)
    {
      v_rms3 = v_in3;
      if (v_rms3*0.7071 > 17)
      {
        digitalWrite(l_led1, HIGH);
      }
      else
      {
        digitalWrite(l_led1, LOW);
      }
     
    }
    
      lcd.setCursor(12, 2);          
      lcd.print("V1 RMS:");            
      lcd.print(v_rms3 *0.7071); 
     
  }



  // CANAL 2 

  Posi_Nega9 = digitalRead(d_input9); 
  bool estadoPulsador2 = digitalRead(2); 
  
  v_analog2 = analogRead(a_input2); 
  
  v_out2 = (v_analog2* 5.0145) / 1024; 
  v_in2 = v_out2 / (R2 / (R1 + R2));  
  if (estadoPulsador2 == LOW)       
  {
    if (Posi_Nega9 == LOW) 
    {                   
      if (v_in2 > 24)
      {
        digitalWrite(l_led2, HIGH);
      }
      else
      {
        digitalWrite(l_led2, LOW);
      }
      
      lcd.setCursor(12, 3); 
      lcd.print("V2= ");   
      lcd.print(v_in2); 
      
      Serial.print("V2 ");
      Serial.println(v_in2);
      delay(500);
    }
    else
    {
      if (Posi_Nega9 == HIGH) 
      {                  
        if (v_in2*-1 < -24)
        {
          digitalWrite(l_led2, HIGH);
        }
        else
        {
          digitalWrite(l_led2, LOW);
        }
       
        lcd.setCursor(12, 3);  
        lcd.print("V2 ");     
        lcd.print(v_in2 * -1); 
        
        Serial.print("V2 ");
        Serial.println(v_in2 * -1);
        delay(500);
      }
    }
  }

  if (estadoPulsador2 == HIGH) 
  {
    //Voltaje mas alto
    if (v_in2 > v_rms2)
    {
      v_rms2 = v_in2;
      if (v_rms2*0.7071 > 17)
      {
        digitalWrite(l_led2, HIGH);
      }
      else
      {
        digitalWrite(l_led2, LOW);
      }
      
      lcd.setCursor(12, 3);         
      lcd.print("V2 RMS:");              
      lcd.print(v_rms2 *0.7071); 
     
      //Serial.print("Vrms2: ");
      //Serial.println(Vrms4);
      //delay(500);
    }
  }
  
  
  
  // Canal 3
  //Positov o negativo
  Posi_Nega10 = digitalRead(d_input10); 
  bool estadoPulsador1 = digitalRead(2); 
 
  v_analog1 = analogRead(a_input1); 
  
  v_out1 = (v_analog1 * 5.0145) / 1024; 
  v_in1 = v_out1 / (R2 / (R1 + R2));  
  if (estadoPulsador1 == LOW)        
  {
    if (Posi_Nega10 == LOW) 
    {                   
      if (v_in1  > 24)
      {
        digitalWrite(l_led3, HIGH);
      }
      else
      {
        digitalWrite(l_led3, LOW);
      }
      
      lcd.setCursor(12, 4); 
      lcd.print("V3= ");   
      lcd.print(v_in1); 
     
      Serial.print("V3 ");
      Serial.println(v_in1);
      delay(500);
    }
    else
    {
      if (Posi_Nega10 == HIGH) 
      {                    
        if (v_in1 * -1 < -24)
        {
          digitalWrite(l_led3, HIGH);
        }
        else
        {
          digitalWrite(l_led3, LOW);
        }
       
        lcd.setCursor(12, 4);  
        lcd.print("V3 ");    
        lcd.print(v_in1 * -1); 
        
        Serial.print("V3 ");
        Serial.println(v_in1 * -1);
        delay(500);
      }
    }
  }

  if (estadoPulsador1 == HIGH) 
  {
    // Para enocntrar el voltaje mas alto de la senal sinusoidal
    if (v_in1 > v_rms1)
    {
      v_rms1 = v_in1;
      if (v_rms1*0.7071 > 17)
      {
        digitalWrite(l_led3, HIGH);
      }
      else
      {
        digitalWrite(l_led3, LOW);
      }
      
      lcd.setCursor(12, 4);           
      lcd.print("V3 RMS:");              
      lcd.print(v_rms1 *0.7071); 
     //Serial prueba
      //Serial.print("Vrms3: ");
      //Serial.println(Vrms2);
      //delay(500);
    }
  }
  
  
  
  // Canal 4
  Posi_Nega11 = digitalRead(d_input11); 
  bool estadoPulsador0 = digitalRead(2); 
  
  v_analog0 = analogRead(a_input0); 
 
  v_out0 = (v_analog0 * 5.0145) / 1024; 
  v_in0 = v_out0 / (R2 / (R1 + R2));  
  if (estadoPulsador0 == LOW)        
  {
    if (Posi_Nega11 == LOW) 
    {                   
      if (v_in0  > 24)
      {
        digitalWrite(l_led4, HIGH);
      }
      else
      {
        digitalWrite(l_led4, LOW);
      }
     
      lcd.setCursor(12, 5); 
      lcd.print("V4= ");   
      lcd.print(v_in0);
    
      Serial.print("V4 ");
      Serial.print(v_in0 * 5);
      delay(500);
    }
    else
    {
      if (Posi_Nega11 == HIGH) 
      {                   
        if (v_in0 * -1 < -24)
        {
          digitalWrite(l_led4, HIGH);
        }
        else
        {
          digitalWrite(l_led4, LOW);
        }
       
        lcd.setCursor(12, 5);  
        lcd.print("V4 ");     
        lcd.print(-1* v_in0); 
        
        Serial.print("V4 ");
        Serial.println(v_in0 * -1);
        delay(500);
     }
    }
  }

  if (estadoPulsador0 == HIGH) 
  {
    
    if (v_in0 > v_rms0)
    {
      v_rms0 = v_in0;
      if (v_rms0*0.7071 > 17)
      {
        digitalWrite(l_led4, HIGH);
      }
      else
      {
        digitalWrite(l_led4, LOW);
      }
     
      delay(500);
    }
     lcd.setCursor(12, 5);           
      lcd.print("V4 rms:");               
      lcd.print(v_rms0*0.7071); 
  }
   if (estadoPulsador0 == HIGH)
   {
      Serial.print("Vrms1 ");
      Serial.println(v_rms3 / sqrt(2));
      Serial.print("Vrms2 ");
      Serial.println(v_rms2*0.7071);
      Serial.print("Vrms3 ");
      Serial.println(v_rms1*0.7071);
      Serial.print("Vrms4 ");
      Serial.println(v_rms0*0.7071);
   }

} 