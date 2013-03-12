///////////////////////////////////////////////////////////////////////////////
// If Flash state is ON, look if the preFlashStatus is also ON. If that is so, 
// start counting each following flash, until we reach the pre-defined preFlashLimit.
// When this limit is reached, step out of the counting loop(while) and continue with the rest of the flash sequence.
// That is, turn the digitalWrite(FLASH_PIN HIGH);
// If Flash state is OFF, turn off the Flash LED.
///////////////////////////////////////////////////////////////////////////////

void flash (int flashState)
{
  if (flashState == ON)
  {
    if (preFlashStatus == ON)
    {
      currentPreFlashCount ++;    // don't forget to count the first flash that brought us here
      Serial.println ();
      Serial.println ("STARTED COUNTING FLASHES");
      Serial.println ();
      Serial.print("current count=");
      Serial.print(currentPreFlashCount); 
      while (currentPreFlashCount < (preFlashLimit + 1))
      {
        if (flash_sensor_triggered())
        {
          currentPreFlashCount ++;
          Serial.print(currentPreFlashCount);
        }
      }
    }

    digitalWrite(FLASH_PIN, HIGH);
    currentPreFlashCount = 0;
  }

  if (flashState == OFF)
  {

    digitalWrite(FLASH_PIN, LOW);

  }
}

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

void power ()
{
  delay (map(flashPower, 1, 9, 10, 50));
}  

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////

void flash_train()
{ 
  flash(ON);
  power();
  flash(OFF);
  Serial.println ();
  Serial.println ("FLASH_TRAIN_SUCCESSFUL");
  Serial.println ("ENTERING IDLE_STATE");


}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////

void manual_flash_train()
{ 
  digitalWrite(FLASH_PIN, HIGH);
  power();
  digitalWrite(FLASH_PIN, LOW);
  Serial.println ();
  Serial.println ("MANUAL_FLASH_TRAIN_SUCCESSFUL");
  Serial.println ("ENTERING IDLE_STATE");  
}


