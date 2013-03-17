/*

 $$$$$$\                                      $$$$$$$$\ $$\                     $$\             
 $$  __$$\                                     $$  _____|$$ |                    $$ |            
 $$ /  $$ | $$$$$$\   $$$$$$\  $$$$$$$\        $$ |      $$ | $$$$$$\   $$$$$$$\ $$$$$$$\        
 $$ |  $$ |$$  __$$\ $$  __$$\ $$  __$$\       $$$$$\    $$ | \____$$\ $$  _____|$$  __$$\       
 $$ |  $$ |$$ /  $$ |$$$$$$$$ |$$ |  $$ |      $$  __|   $$ | $$$$$$$ |\$$$$$$\  $$ |  $$ |      
 $$ |  $$ |$$ |  $$ |$$   ____|$$ |  $$ |      $$ |      $$ |$$  __$$ | \____$$\ $$ |  $$ |      
 $$$$$$  |$$$$$$$  |\$$$$$$$\ $$ |  $$ |      $$ |      $$ |\$$$$$$$ |$$$$$$$  |$$ |  $$ |      
 \______/ $$  ____/  \_______|\__|  \__|      \__|      \__| \_______|\_______/ \__|  \__|      
          $$ |                                                                                  
          $$ |                                                                                  
          \__|                                                                                  
 
 This is OpenFlash Hardware Tester
 
 Set your OpenFlash hardware up using the provided OpenFlash PCB kit, an Arduino and a breadboard.
 Modify and upload this sketch to the Arduino in order to change your OpenFlash's firmware. 

 Code written by Argun Cencen and Aadjan van der Helm, extracts from examples by Rob Luxen, Martin Verwaal and Walter Aprile.
 For questions please contact Aadjan van der Helm, a.j.c.vanderhelm@tudelft.nl
 
 OpenFlash is developed in co-operation with Applied Labs and Id.Studiolab for the course Interaction and Electronics.
 
 */

///////////////////////////////////////////////////////////////////////////////
// We need to declare at the main window which header files we are using 
///////////////////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include "Advanced.h"
#include "Initialize.h"
#include "State_Machine.h"

///////////////////////////////////////////////////////////////////////////////
// Each Arduino sketch requires a void setup()
// The function inside here is executed every time the Arduino is powered up/resetted
///////////////////////////////////////////////////////////////////////////////


void setup()
{
  initialize();     //See tab "Initialize.h" for details
}

///////////////////////////////////////////////////////////////////////////////
// Each Arduino sketch requires a void loop()
// The function inside here is repeated over and over again while the Arduino is powered ON.
///////////////////////////////////////////////////////////////////////////////

void loop()
{
  state_machine();     // Run the main state machine - see tab State_Machine.h for details
}
