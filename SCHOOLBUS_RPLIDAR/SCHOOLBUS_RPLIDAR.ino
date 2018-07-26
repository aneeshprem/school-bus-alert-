/*
 /*******************************************************************************
 * File Name          : <CAPSTONE PROJECT>
 *                      
 * Description        : <SCHOOL BUS ALERT SYSTEM>
 *                      A system that can measure the distance of the vehicle 
 *                      behind or infront of a school bus using a Lidar.If vehicle 
 *                      crosses the safe distance, the system will alert the children 
 *                      about the danger with the help of buzzer. It also give indication 
 *                      to vehicles by using a strobe light.
 * Author:              <CRYSTEENA ELIZABATH JEROME>
 *                      <ANEESH PREM>
 * Date:                <2018-07-26>
 ******************************************************************************
 */
/* Downloaded sketch code from Arduino library
 * PIN CONNECTION
 * 1. Connect the RPLIDAR's serial port (RX/TX/GND) to your Arduino board (Pin 0 and Pin1)
 * 2. Connect the RPLIDAR's motor ctrl pin to the Arduino board pin 3 
 * 3. Connect LED(instead of strobe light) to pin 12 of Arduino board
 * 4. Connect buzzer to pin 13 of Arduino board
 * 5. Connect the RPLIDAR's motor ctrl pin to the Arduino board pin 3 
 */
 
 
// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>

// You need to create an driver instance 
RPLidar lidar;

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal 
  #define led 12   // pin to connect LED
  #define buzzer 13 // Pin to connect the buzzer
  
// FUNCTION      : printDistance()
// DESCRIPTION   : Prints the distance value and checks the 
//               : different switch coonditions needed for providing alert
// PARAMETERS    : None
// RETURNS       : nothing 
                
 void printDistance(void);   
                
void setup() {
  // bind the RPLIDAR driver to the arduino hardware serial
  lidar.begin(Serial);
  
  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {
  if (IS_OK(lidar.waitPoint())) {
         printDistance();     //calls the function 
    //perform data processing here... 

    
  } else {
   
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       // detected...
       lidar.startScan();
       
       // start motor rotating at max allowed speed
       analogWrite(RPLIDAR_MOTOR, 255);
       delay(1000);
    }
  
}
}

//  FUNCTION      : printDistance
//  DESCRIPTION   : It calculates the distance of the detected obstacle according 
//                  to the conditions in switch staement.Focussed angle is between 
//                  290 and 310 degrees. The distance range is between 4000mm and 
//                  0mm. 
//                  If distance fro the obstacle is between 4000mm and 1500mm, LED   
//                  turns ON indicating a warning to vehicles.
//                  If obstacle enters the range of 1500mm to 1000mm, then buzzer  
//                  will turn ON for alerting children.
//                  If the obstacle stops the buzzer will turn OFF otherwise it will 
//                  be ON until the vehicle stops
//   PARAMETERS    : None
//   RETURNS       : nothing

void printDistance(void)
{
     float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
     
    float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
     int Condition = 0;
     static int count = 0; // checking condition if vehicle is still moving
      if((angle >= 0)&&(angle<290)) // If angle is between 0 and 290 degrees
         {
           Condition = 5;
         }
           
   else if((angle >= 290)&&(angle<=310)) // angle between 290 and 310 degrees
       {
          if((distance >= 1500)&&(distance<=4000)) //distance range 4000mm to 1500mm
            {
              Condition = 1; 
            }
           
          else if((distance >= 1000) && (distance <=1500)) //distance range between 1500 and 1000
            {
              count++;
              if(count < 500)
                {
                  Condition = 2;
                }
              else if(count >=500)
                {
                  Condition = 3;
                }
              }
       
                
              }
  
        
         
    switch(Condition)
        {
        case 1: 
              {
                digitalWrite(led,HIGH);
                digitalWrite(buzzer,LOW);
                Serial.print(distance);
                Serial.print("mm\t\t");
                Serial.print(angle);
                Serial.println("deg");
                break;
              }
         case 2: 
               {
                digitalWrite(led,HIGH);
                digitalWrite(buzzer,HIGH);
                Serial.print(distance);
                Serial.print("mm\t\t");
                Serial.print(angle);
                Serial.println("deg");
                 break; 
                }
          case 3:
                {
                  digitalWrite(led,LOW);
                  digitalWrite(buzzer,LOW); 
                  Condition =4;
                }
          case 4:
                {
                  Serial.println("car stopped");
                }
          case 5:
                {
                  digitalWrite(led,LOW);
                  digitalWrite(buzzer,LOW);
                }
  }
}
