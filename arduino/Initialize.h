///////////////////////////////////////////////////////////////////////////////
// For the INPUTs and OUTPUTs or the Arduino to function as we want them to,  we need to tell it if we want a certain pin to be an input or an output. 
// Hint: Even the pins that are labeled as "ANALOG IN" on the Arduino can actually be used as outputs!
// Also, to be able to use the serial port for debugging and communication with the Arduino, we need to initialize it and tell it which speed it should run on.
// Finally, its sometimes good practice to take initial readings from sensors and to turn of pins that we don't want to use right away.
///////////////////////////////////////////////////////////////////////////////

void initialize()
{
  //    Initialize the serial port
  Serial.begin (9600);

  //    Initialize the pins of the 3 colors of the flash as outputs and assign them the PWM values
  pinMode       (RED_LED, OUTPUT);
  pinMode       (GREEN_LED, OUTPUT);
  pinMode       (BLUE_LED, OUTPUT);
  analogWrite   (RED_LED, flashColors [flashColor][0]);     //write the PWM value to the dedicated pin. Take this value from the array called "flashColors"
  analogWrite   (GREEN_LED, flashColors [flashColor][1]);   
  analogWrite   (BLUE_LED, flashColors [flashColor][2]);    

  //    Initialize the flashing pin as OUTPUT
  pinMode       (FLASH_PIN, OUTPUT);

  //    Initialize all 2 buttons and the flash sensor as INPUT  
  pinMode       (FLASH_SENSOR_PIN, INPUT);
  pinMode       (CHANGE_BUTTON_PIN, INPUT);
  pinMode       (SELECT_BUTTON_PIN, INPUT);

  //    Initialize the input variables 
  boolean changeButtonState = digitalRead(CHANGE_BUTTON_PIN);  //get the initial value of the buttons, so that we know if they change state later on.
  boolean selectButtonState = digitalRead(SELECT_BUTTON_PIN);
  boolean flashSensorState = digitalRead(FLASH_SENSOR_PIN);

  //    Initialize the pins that are required for the shift register that is used to drive the display  
  pinMode       (SDI,OUTPUT);
  pinMode       (CLK,OUTPUT);
  pinMode       (LE,OUTPUT);
  pinMode       (OE,OUTPUT);
  digitalWrite  (CLK,LOW);
  digitalWrite  (LE,LOW);
  digitalWrite  (SDI,LOW);
  digitalWrite  (OE,HIGH);
  delay(100);

}

///////////////////////////////////////////////////////////////////////////////
// We want the reaction time of the OpenFlash to be as fast as possible. 
// Therefore, we make sure that the flash LED unit is always ready to fire the right color of flash.
// We do this by setting the PWM values to 3 OUTPUT pins.
// After this, when we actually want to flash to turn on, we only need to tell the unit to turn ON.
//
// The moral of the story is, when we change the flashColor parameter in the User Interface, 
// the Arduino needs to change the PWM values of the dedicated pins. 
// Therefore, we re-initialize this part of the regular initialization function.
///////////////////////////////////////////////////////////////////////////////

void re_initialize_flashpins()

{
  analogWrite   (RED_LED, flashColors [flashColor][0]);
  analogWrite   (GREEN_LED, flashColors [flashColor][1]);
  analogWrite   (BLUE_LED, flashColors [flashColor][2]);
}

