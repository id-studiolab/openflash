///////////////////////////////////////////////////////////////////////////////
//            Sections:
//
//            1- Pin Settings
//            2- User Interface configurable parameters
//            3- Global variables
//            4- State definitions
//            5- Flash sensor functions
//            6- Button functions
//            7- Timing functions
//            8- Display functions
//            9- Displaying current variable values function in Serial Monitor
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//      Section 1 - Pin Settings
///////////////////////////////////////////////////////////////////////////////

// The push-button pins
#define CHANGE_BUTTON_PIN 8
#define SELECT_BUTTON_PIN 7 

// The display/shift register pins
#define OE 6
#define SDI 3
#define CLK 4
#define LE 5

// The flash sensor pin
#define FLASH_SENSOR_PIN 2

// The flash trigger pin
#define FLASH_PIN 12

// The flash pins
#define RED_LED 9      //PWM enabled
#define GREEN_LED 10   //PWM enabled
#define BLUE_LED 11    //PWM enabled

///////////////////////////////////////////////////////////////////////////////
//      Section 2 - User Interface configurable parameters
///////////////////////////////////////////////////////////////////////////////

boolean preFlashStatus = false;     
//true or false (1 or 0)
int preFlashLimit = 1;              
//Between 1-9
int flashPower = 1;                 
//Between 1-9
int flashColor = 1;                 
//As default between 1-4. 1=White, 2=Red, 3=Green, 4=Blue [See Section-3 --> int flashColors]
//If new colors are to be implemented, adjustments are required in [Section-3 --> int flashColors]
//and [tab "State _Machine.h" --> case FLASH_COLOR_STATE: --> if(flashColor == X)]
boolean customStateStatus = true;
//true or false (1 or 0)

///////////////////////////////////////////////////////////////////////////////
//      Section 3 - Global variables
///////////////////////////////////////////////////////////////////////////////

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


boolean previousChangeButtonState = false;
boolean changeButtonState;
boolean previousSelectButtonState = false;
boolean selectButtonState;
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
  IDLE_STATE,
  MANUAL_FLASH_STATE,
  OPTION_A_STATE,
  OPTION_B_STATE,
  OPTION_C_STATE,
  OPTION_D_STATE,
  OPTION_E_STATE,
  PREFLASH_STATE,
  PREFLASH_COUNT_STATE,
  FLASH_POWER_STATE,
  FLASH_COLOR_STATE,
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
//      Section 6 - Button functions
///////////////////////////////////////////////////////////////////////////////

//     When called, test if the "change" button is pressed

boolean change_button_pressed()
{
  boolean buttonState = false;
  changeButtonState = digitalRead(CHANGE_BUTTON_PIN) ; 

  if (changeButtonState != previousChangeButtonState)
  {

    if (changeButtonState == LOW)
    {
      delay(10);
      if (changeButtonState == LOW)
      {
        buttonState = true;
      }
    }

  }
  previousChangeButtonState = changeButtonState;
  return buttonState;

}

//     When called, test if the "select" button is pressed

boolean select_button_pressed()
{

  boolean buttonState = false;
  selectButtonState = digitalRead(SELECT_BUTTON_PIN);

  if (selectButtonState != previousSelectButtonState)
  {

    if (selectButtonState == LOW)
    {
      delay(10);
      {
        buttonState = true;
      }
    }


  }
  previousSelectButtonState = selectButtonState;
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

  case IDLE_STATE:
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

  case FLASH_COLOR_STATE:
    {
      display (character[flashColor]);
      break;  
    }

  case CUSTOM_STATE:
    {
      display (character[customStateStatus]);
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
  Serial.print("Flash color is= ");
  Serial.print(flashColor);
  Serial.println ();
  Serial.print("Custom function is= ");
  Serial.print(customStateStatus);
  Serial.println ();
  
}













