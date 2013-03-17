///////////////////////////////////////////////////////////////////////////////
//            Sections:
//
//            1- Pin Settings
//            2- User Interface configurable parameters
//            3- Global variables
//            4- State definitions
//            5- Flash sensor functions
//            6- Switch functions
//            7- Timing functions
//            8- Display functions
//            9- Displaying current variable values function in Serial Monitor
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//      Section 1 - Pin Settings
///////////////////////////////////////////////////////////////////////////////

// The push-button pins
#define CHANGE_SWITCH_PIN 9
#define SELECT_SWITCH_PIN 10
#define AUX_SWITCH_PIN 11

// The display/shift register pins
#define OE 4
#define SDI 8
#define CLK 1
#define LE 0

// The flash sensor pin
#define FLASH_SENSOR_PIN 2

// The flash trigger pin
#define FLASH_PIN 7

// The flash pins
#define RED_LED 3     //PWM enabled
#define GREEN_LED 5   //PWM enabled
#define BLUE_LED 6    //PWM enabled

///////////////////////////////////////////////////////////////////////////////
//      Section 2 - User Interface configurable parameters
///////////////////////////////////////////////////////////////////////////////

// Do we count preflashes?
boolean preFlashStatus = false;     
//true or false (1 or 0)
int preFlashLimit = 1;              
//Between 1-9
int flashPower = 1;                 
//Between 1-9
int flashColor = 0;                 
//As default between 0-3. 0=White, 1=Red, 2=Green, 3=Blue [See Section-3 --> int flashColors]
//If new colors are to be implemented, adjustments are required in [Section-3 --> int flashColors]
//and [tab "State _Machine.h" --> case FLASH_COLOR_STATE: --> if(flashColor == X)]
boolean customStateStatus = true;
//true or false (1 or 0)

///////////////////////////////////////////////////////////////////////////////
//      Section 3 - Global variables
///////////////////////////////////////////////////////////////////////////////

// shutterOpenDelay is used to aim the occurrence of the flash in the shutter-open interval
// May be different from camera to camera and dependent on camera settings
int shutterOpenDelay = 70;

// For your convenience if you want to play with colours
int flashColors [4][3] = 
{
  {
    255, 255, 255                }
  ,  //White
  {
    255, 0, 0                }
  ,      //Red
  {
    0, 255, 0                }
  ,      //Green
  {
    0, 0, 255                }
  ,      //Blue
};


boolean previousChangeSwitchState = false;
boolean changeSwitchState;
boolean previousSelectSwitchState = false;
boolean selectSwitchState;
boolean previousAuxSwitchState = false;
boolean auxSwitchState;
boolean previousFlashSensorState = false;
boolean flashSensorState;

int currentPreFlashCount = 0;

boolean flashState = false;

long previous_millis = 0;
long duration = 3000;

///////////////////////////////////////////////////////////////////////////////
//      Section 4 - State definitions
///////////////////////////////////////////////////////////////////////////////


//     Define all possible states of the State Machine as enumerated integers. 
//     We see them as text, but they are actually numbers.

typedef enum
{
  NORMAL_STATE,
  OPTION_A_STATE,
  OPTION_B_STATE,
  OPTION_C_STATE,
  OPTION_D_STATE,
  OPTION_E_STATE,
  PREFLASH_STATE,
  PREFLASH_COUNT_STATE,
  FLASH_POWER_STATE,
  FLASH_COLOR_STATE,
  MANUAL_FLASH_STATE,
  FLASH_TRAIN_STATE,
  COUNT_PREFLASH_STATE,
  DO_FLASH_STATE,
  CUSTOM_STATE 
}
_current_state;

_current_state   current_state;

//    Define the words "ON" and "OFF" as "1" and "0" respectively
//    This makes it more readable for us when we are doing certain 
//    actions such as turning LED's "ON" and "OFF" or changing states.

#ifndef	ON
#define  ON	1
#endif

#ifndef	OFF
#define  OFF	0
#endif

///////////////////////////////////////////////////////////////////////////////
//      Section 5 - Flash sensor functions
///////////////////////////////////////////////////////////////////////////////

//    When called, test if Flash Sensor is triggered

boolean flash_sensor_triggered()
{
  boolean triggerState = false;
  flashSensorState = digitalRead(FLASH_SENSOR_PIN); 

  if (flashSensorState != previousFlashSensorState)
  {


    if (flashSensorState == HIGH)
    {
      delay(10);
      if (flashSensorState == HIGH) 
      {
        triggerState = true;
      }
    }

  }
  previousFlashSensorState = flashSensorState;
  return triggerState;
}

///////////////////////////////////////////////////////////////////////////////
//      Section 6 - Switch functions
///////////////////////////////////////////////////////////////////////////////

//     When called, test if the "change" button is pressed

boolean change_switch_pressed()
{
  boolean buttonState = false;
  changeSwitchState = digitalRead(CHANGE_SWITCH_PIN) ; 

  if (changeSwitchState != previousChangeSwitchState)
  {

    if (changeSwitchState == LOW)
    {
      delay(10);
      if (changeSwitchState == LOW)
      {
        buttonState = true;
      }
    }
  }
  previousChangeSwitchState = changeSwitchState;
  return buttonState;

}

//     When called, test if the "select" button is pressed

boolean select_switch_pressed()
{

  boolean buttonState = false;
  selectSwitchState = digitalRead(SELECT_SWITCH_PIN);

  if (selectSwitchState != previousSelectSwitchState)
  {

    if (selectSwitchState == LOW)
    {
      delay(10);
      {
        buttonState = true;
      }
    }
  }
  previousSelectSwitchState = selectSwitchState;
  return buttonState;

}

//     When called, test if the "aux" button is pressed

boolean aux_switch_pressed()
{

  boolean buttonState = false;
  auxSwitchState = digitalRead(AUX_SWITCH_PIN);

  if (auxSwitchState != previousAuxSwitchState)
  {

    if (auxSwitchState == LOW)
    {
      delay(10);
      {
        buttonState = true;
      }
    }
  }
  previousAuxSwitchState = auxSwitchState;
  return buttonState;

}

///////////////////////////////////////////////////////////////////////////////
//     Section 7 - Timing functions
///////////////////////////////////////////////////////////////////////////////

//     When called, check if timeout is reached

boolean timeout()
{

  if(millis() - previous_millis > duration)
  {
    return true;
  }

}

//     When called, reset the timer

void timer_reset()
{
  previous_millis = millis();
}

///////////////////////////////////////////////////////////////////////////////
//     Section 8 - Display functions
///////////////////////////////////////////////////////////////////////////////

//     "character[]" is an array consisting of 19 different values
//     It is used to choose the right bit that corresponds to the required digit or character.
//     Each value can be called from anywhere in the program by referring to its address.
//     For example, character[10] contains the value "0b01101111", which corresponds to digit "9" on the LED_display

uint8_t character [20] = 
{
  //pgfedcba
  B11111100,    // digit 0  
  B01100000,    // digit 1   
  B11011010,    // digit 2  
  B11110010,    // digit 3  
  B01100110,    // digit 4  
  B10110110,    // digit 5  
  B00111110,    // digit 6  
  B11100000,    // digit 7  
  B11111110,    // digit 8  
  B11100110,    // digit 9  
  B11101110,    // letter A  
  B00111110,    // letter B  
  B10011100,    // letter C  
  B01111010,    // letter D  
  B10011110,    // letter E  
  B10001110,    // letter F
  B00000000,    // blank
  B00000001,    // decimal point
  B00000010,     // middle-line
  B10010010,     // top-middle-bottom-lines
};

//     When called, send the required bits to the LED_display

void display(int bits) 
{
  digitalWrite(OE,HIGH);
  shiftOut(SDI, CLK, LSBFIRST, bits);
  digitalWrite(LE,HIGH);
  digitalWrite(LE,LOW);
  digitalWrite(OE,LOW);
}

//     Select what to display on the LED_display according to the current state
//     Here, the above mentioned character[] array is used and each time, a different address is called
//     Attention!!!
//     For OPTION_STATES, the operation is quite straight-forward. Becase, each state already has an assigned character.
//     However, the values of each of these states are variables and can be configured through the User Interface.
//     That's why, the adress to these values are called through a variable. At a first glance, this can be confusing.

void LED_display()
{

  switch (current_state)
  {

  case NORMAL_STATE:
    {
      display (character[18]);
      break;  
    }
 
  case MANUAL_FLASH_STATE:
    {
      display (character[19]);
      break;  
    }
  case OPTION_A_STATE:
    {
      display (character[10]);
      break;  
    }

  case OPTION_B_STATE:
    {
      display (character[11]);
      break;  
    }

  case OPTION_C_STATE:
    {
      display (character[12]);
      break;  
    }

  case OPTION_D_STATE:
    {
      display (character[13]);
      break;  
    }

  case OPTION_E_STATE:
    {
      display (character[14]);
      break;  
    }

  case PREFLASH_STATE:
    {
      display (character[preFlashStatus]);
      break;  
    }

  case PREFLASH_COUNT_STATE:
    {
      display (character[preFlashLimit]);
      break;  
    }

  case FLASH_POWER_STATE:
    {
      display (character[flashPower]);
      break;  
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//     Section 9 - Displaying current variable values function
///////////////////////////////////////////////////////////////////////////////

void print_current_status()
{
    //    Print out the current values of each option
  Serial.println ();
  Serial.print("Pre flash status= ");
  Serial.print(preFlashStatus);
  Serial.println ();
  Serial.print("Pre flash count limit is= ");
  Serial.print(preFlashLimit);
  Serial.println ();
  Serial.print("Flash power is= ");
  Serial.print(flashPower);
  Serial.println ();  
}













