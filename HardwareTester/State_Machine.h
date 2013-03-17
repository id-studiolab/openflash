///////////////////////////////////////////////////////////////////////////////
// This is the state machine function,
// In order to jump to a certain state, use ctrl+f (Windows), (cmd+F on MacOS)
// and type the state name you are looking for.
///////////////////////////////////////////////////////////////////////////////

void do_flash()
{
  digitalWrite(FLASH_PIN, HIGH);
  delay(50);
  digitalWrite(FLASH_PIN, LOW);
  Serial.println ();
  Serial.println ("FLASH_TRAIN_SUCCESSFUL");
}

void state_machine ()
{
  switch (current_state)
  {
///////////////////////////////////////////////////////////////////////////////
  case NORMAL_STATE:
    {
      LED_display();

      if (select_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING TEST_SELECT_STATE");
        current_state = TEST_SELECT_STATE;  
      }

      if (change_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING TEST_CHANGE_STATE");
        current_state = TEST_CHANGE_STATE;
      }

      if (aux_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING TEST_AUX_STATE");
        current_state = TEST_AUX_STATE;
      }

      if (flash_sensor_triggered())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("SENSOR TRIGGERED");
        do_flash();
      }
      break;
    }

  case TEST_SELECT_STATE:
    {
      LED_display();
 
      if (timeout())
      {
        timer_reset();
        do_flash();
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;
      }
      break;
    }

  case TEST_CHANGE_STATE:
    {
      LED_display();

      if (timeout())
      {
        timer_reset();
        do_flash();
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;
      }
      break;
    }

  case TEST_AUX_STATE:
    {
      LED_display();

      if (timeout())
      {
        timer_reset();
        do_flash();
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;
      }
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////





