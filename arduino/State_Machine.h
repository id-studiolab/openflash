///////////////////////////////////////////////////////////////////////////////
// This is the State Machine function,
// In order to jump to a certain state, use ctrl+f, (CMD+F) and type the state name you are looking for.
// Most code in this section is 
///////////////////////////////////////////////////////////////////////////////

void state_machine ()
{

  switch (current_state)
  {

  case IDLE_STATE:

    {
      LED_display();

      if (select_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_A_STATE");
        current_state = OPTION_A_STATE;  
      }

      if (change_button_pressed())
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
        if (customStateStatus == true)
        {
          Serial.println ();
          Serial.println ("SENSOR TRIGGERED");
          flash_train();
          current_state = IDLE_STATE;
        }

        if (customStateStatus == false)
        {
          Serial.println ();
          Serial.println ("SENSOR TRIGGERED");
          flash_train();
          current_state = IDLE_STATE;
        }        

      }

      break;

    }

  case MANUAL_FLASH_STATE:

    {
      LED_display();
      if (select_button_pressed())
      {
        timer_reset();
        manual_flash_train();
        current_state = IDLE_STATE;
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING IDLE_STATE");
        current_state = IDLE_STATE;
        print_current_status();
      }

      break;
    }

  case OPTION_A_STATE:

    {
      LED_display();

      if (change_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_B_STATE");

        current_state = OPTION_B_STATE;
      }

      if (select_button_pressed())
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
        Serial.println ("ENTERING IDLE_STATE");
        current_state = IDLE_STATE;
        print_current_status();
      }

      break;

    }

  case PREFLASH_STATE:

    {
      LED_display();

      if (change_button_pressed())
      {
        timer_reset();
        preFlashStatus = 1 - preFlashStatus;
        Serial.println ();
        Serial.println (preFlashStatus);

      }

      if (select_button_pressed())
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

      if (change_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_C_STATE");
        current_state = OPTION_C_STATE;        
      }

      if (select_button_pressed())
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
        Serial.println ("ENTERING IDLE_STATE");
        current_state = IDLE_STATE;
        print_current_status();
      }

      break;

    }

  case PREFLASH_COUNT_STATE:

    {
      LED_display();

      if (change_button_pressed())
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

      if (select_button_pressed())
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

      if (change_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_D_STATE");
        current_state = OPTION_D_STATE;        
      }

      if (select_button_pressed())
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
        Serial.println ("ENTERING IDLE_STATE");
        current_state = IDLE_STATE;        
        print_current_status();
      }

      break;

    }

  case FLASH_POWER_STATE:

    {
      LED_display();

      if (change_button_pressed())
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

      if (select_button_pressed())
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

  case OPTION_D_STATE:

    {
      LED_display();

      if (change_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_E_STATE");  
        current_state = OPTION_E_STATE;
      }

      if (select_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING FLASH_COLOR_STATE");
        current_state = FLASH_COLOR_STATE;
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING IDLE_STATE");
        current_state = IDLE_STATE;
        print_current_status();        
      }

      break;

    }

  case FLASH_COLOR_STATE:

    {
      LED_display();

      if (change_button_pressed())
      {
        timer_reset();
        flashColor ++;

        if(flashColor == 5)  // Change this number if you made changes to the "int flashColors"
        {
          flashColor = 1;
        }

        re_initialize_flashpins();
        Serial.println ();
        Serial.println (flashColor);
      }

      if (select_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_D_STATE");
        current_state = OPTION_D_STATE;        
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_D_STATE");
        current_state = OPTION_D_STATE;        
      }

      break;

    }

  case OPTION_E_STATE:

    {
      LED_display();

      if (change_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_A_STATE");
        current_state = OPTION_A_STATE;        
      }

      if (select_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING CUSTOM_STATE");
        current_state = CUSTOM_STATE;        
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING IDLE_STATE");
        current_state = IDLE_STATE;        
        print_current_status();
      }

      break;

    }

  case CUSTOM_STATE:

    {
      LED_display();

      if (change_button_pressed())
      {
        timer_reset();        
        customStateStatus = 1 - customStateStatus;
        Serial.println ();
        Serial.println (customStateStatus);  
      }

      if (select_button_pressed())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_E_STATE");
        current_state = OPTION_E_STATE;        
      }

      if (timeout())
      {
        timer_reset();
        Serial.println ();
        Serial.println ("ENTERING OPTION_E_STATE");
        current_state = OPTION_E_STATE;        
      }

      break;

    }
  }
}

///////////////////////////////////////////////////////////////////////////////





