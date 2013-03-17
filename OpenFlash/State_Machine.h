///////////////////////////////////////////////////////////////////////////////
// This is the state machine function,
// In order to jump to a certain state, use ctrl+f (Windows), (cmd+F on MacOS)
// and type the state name you are looking for.
///////////////////////////////////////////////////////////////////////////////

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
        Serial.println ("ENTERING OPTION_A_STATE");
        current_state = OPTION_A_STATE;  
      }

      if (change_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING MANUAL FLASH STATE");
        Serial.println ("MANUAL FLASH ARMED");
        current_state = MANUAL_FLASH_STATE;
      }

      if (flash_sensor_triggered())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("SENSOR TRIGGERED");
        Serial.println ("ENTERING FLASH STATE");
        current_state = FLASH_TRAIN_STATE;
      }
      break;
    }

///////////////////////////////////////////////////////////////////////////////
// States related menu navigation and option setting
///////////////////////////////////////////////////////////////////////////////
  case OPTION_A_STATE:

    {
      LED_display();

      if (change_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_B_STATE");

        current_state = OPTION_B_STATE;
      }

      if (select_switch_pressed())
      {
        timer_reset(); 
        Serial.println ();
        Serial.println ("ENTERING PREFLASH_STATE");
        current_state = PREFLASH_STATE;
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;
        print_current_status();
      }

      break;

    }

  case PREFLASH_STATE:

    {
      LED_display();

      if (change_switch_pressed())
      {
        timer_reset();
        preFlashStatus = 1 - preFlashStatus;
        Serial.println ();
        Serial.println (preFlashStatus);

      }

      if (select_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_A_STATE");
        current_state = OPTION_A_STATE;        
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_A_STATE");
        current_state = OPTION_A_STATE;        
      }

      break;

    }

  case OPTION_B_STATE:
    {
      LED_display();

      if (change_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_C_STATE");
        current_state = OPTION_C_STATE;        
      }

      if (select_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING PREFLASH_COUNT_STATE");
        current_state = PREFLASH_COUNT_STATE;        
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;
        print_current_status();
      }

      break;

    }

  case PREFLASH_COUNT_STATE:

    {
      LED_display();

      if (change_switch_pressed())
      {
        timer_reset();
        preFlashLimit ++;

        if(preFlashLimit == 10)
        {
          preFlashLimit = 1;
        }

        Serial.println ();
        Serial.println (preFlashLimit);
      }

      if (select_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_B_STATE");
        current_state = OPTION_B_STATE;        
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_B_STATE");
        current_state = OPTION_B_STATE;
      }

      break;

    }

  case OPTION_C_STATE:
    {
      LED_display();

      if (change_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_A_STATE");
        current_state = OPTION_A_STATE;        
      }

      if (select_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING FLASH_POWER_STATE");
        current_state = FLASH_POWER_STATE;        
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;        
        print_current_status();
      }

      break;

    }

  case FLASH_POWER_STATE:

    {
      LED_display();

      if (change_switch_pressed())
      {
        timer_reset();
        flashPower ++;

        if(flashPower == 10)
        {
          flashPower = 1;
        }

        Serial.println ();
        Serial.println (flashPower);
      }

      if (select_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_C_STATE");
        current_state = OPTION_C_STATE;
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_C_STATE");
        current_state = OPTION_C_STATE;
      }

      break;

    }

///////////////////////////////////////////////////////////////////////////////
// States related to causing a manual flash
///////////////////////////////////////////////////////////////////////////////
  case MANUAL_FLASH_STATE:

    {
      LED_display();
      
      if (select_switch_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING DO_FLASH_STATE");
        current_state = DO_FLASH_STATE;
        print_current_status();
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;
        print_current_status();
      }

      break;
    }
///////////////////////////////////////////////////////////////////////////////
// States related detecting a manual flash
///////////////////////////////////////////////////////////////////////////////

  case FLASH_TRAIN_STATE:
    {
      LED_display();
      
      if (preFlashStatus == ON)
      {
        Serial.println ();
        Serial.println ("ENTERING COUNT_PREFLASH_STATE");
        current_state = COUNT_PREFLASH_STATE;
        print_current_status();
      }
      else // preFlashStatus == OFF
      {
        Serial.println ();
        Serial.println ("ENTERING DO_FLASH_STATE");
        current_state = DO_FLASH_STATE;
        print_current_status();
      }
      
	  break;
    }
  case COUNT_PREFLASH_STATE:
    {
      LED_display();

      currentPreFlashCount++;    // don't forget to count the first flash that brought us here
      Serial.println ();
      Serial.println ("STARTED COUNTING FLASHES");
      Serial.println ();
      Serial.print("current count=");
      Serial.print(currentPreFlashCount);
      
      if (currentPreFlashCount < (preFlashLimit + 1))
      {
        Serial.println ();
        Serial.println ("ENTERING NORMAL_STATE");
        current_state = NORMAL_STATE;
        print_current_status();
      }
      else
      {
        currentPreFlashCount = 0;
        Serial.println ();
        Serial.println ("ENTERING DO_FLASH_STATE");
        current_state = DO_FLASH_STATE;
        print_current_status();
      }
      break;
    }
  case DO_FLASH_STATE:
    {
      LED_display();
      
      delay(shutterOpenDelay);
      digitalWrite(FLASH_PIN, HIGH);
      delay(map(flashPower, 1, 9, 10, 50));
      digitalWrite(FLASH_PIN, LOW);
      Serial.println ();
      Serial.println ("FLASH_TRAIN_SUCCESSFUL");
      Serial.println ();
      Serial.println ("ENTERING NORMAL_STATE");
      current_state = NORMAL_STATE;
      print_current_status();

      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////





