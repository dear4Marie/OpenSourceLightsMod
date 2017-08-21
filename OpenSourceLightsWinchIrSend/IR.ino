// ------------------------------------------------------------------------------------------------------------------------------------------------>  
// irSendData - IR Send Drive Mode & Light States
// ------------------------------------------------------------------------------------------------------------------------------------------------>  
void irSendData(int DriveMode)
{
    int SaveSetting[NumLights];
    int j;

    // Loop through each light, assign the setting appropriate to its state
    for (j=0; j<NumLights; j++)
    {
        // We will use the temporary variable SaveSetting to assign the setting for this light. 
        // A light could have multiple settings apply at one time, be we can only set it to one thing.
        // Therefore each setting is ranked by importance. If multiple settings apply to a light, 
        // the setting applied LAST will be the one used (each check can overwrite the prior one). 
        // You can re-order the checks below, the least important should come first, and most important last.


        // Least important - does this light have a setting related to Channel 3? 
        // --------------------------------------------------------------------------------------------------->>
        // SaveSetting[j] = LightSettings[j][Channel3];


        // Next - does this light have a setting related to Drive Mode? (Forward, reverse, stop)
        // --------------------------------------------------------------------------------------------------->>
        switch (DriveMode) {
            case FWD:
                irSendBufferLong = IRStateFwd;
                break;
            case REV:
                irSendBufferLong = IRStateRev;
                break;
            case STOP:
                // We have two stop states: 
                // StateStop occurs when the vehicle stops
                // StateStopDelay occurs when the vehicle has been stopped for LongStopTime_mS and will supersede StateStop when it occurs (if not NA)
                irSendBufferLong = IRStateStop;
                if (LightSettings[j][StateStopDelay] != NA && StoppedLongTime == true) { irSendBufferLong += IRStateStopDelay; }
                break;
        }

        // Next - does this light come on during turns? 
        // --------------------------------------------------------------------------------------------------->>        
        if (TurnCommand > 0)    // Right Turn
        {
            irSendBufferLong += IRStateRT;
        }
        if (TurnSignalOverride > 0) // Artificial Right Turn
        {
            irSendBufferLong += IRStateRT;
        }

        if (TurnCommand < 0 || TurnSignalOverride < 0)    // Left Turn
        {
            irSendBufferLong += IRStateLT;
        }
        if (TurnSignalOverride < 0) // Artificial Left Turn
        {
            irSendBufferLong += IRStateLT;
        }

        switch (Channel3)
        {
            case Pos1:
                irSendBufferLong += IRStateCh3Pos1;
                break;
            case Pos2:
                irSendBufferLong += IRStateCh3Pos2;
                break;
            case Pos3:
                irSendBufferLong += IRStateCh3Pos3;
                break;
            case Pos4:
                irSendBufferLong += IRStateCh3Pos4;
                break;
            case Pos5:
                irSendBufferLong += IRStateCh3Pos5;
                break;
        }

        switch (Channel4)
        {
            case Pos1:
                irSendBufferLong += IRStateCh4Pos1;
                break;
            case Pos2:
                irSendBufferLong += IRStateCh4Pos2;
                break;
            case Pos3:
                irSendBufferLong += IRStateCh4Pos3;
                break;
        }

        // Next - does this light come on during braking?
        // --------------------------------------------------------------------------------------------------->>        
        if (Braking)
        {
            irSendBufferLong = IRStateBrake;
        }

        // Next - does this light come on during deceleration (probably Backfiring?)
        // --------------------------------------------------------------------------------------------------->>        
        if (canBackfire)
        {
            irSendBufferLong += IRStateBackFire;
        }

        // Next - 
        // --------------------------------------------------------------------------------------------------->>        
        if (Overtaking)
        {
            irSendBufferLong += IRStateOverTaking;
        }

        irSendBufferLong += (ThrottleCommand + 100);

        //irsend.sendSony(irSendBufferLong, 32);
        irsend.sendNEC(irSendBufferLong, 32);
        //mySender.send(SONY,irSendBufferLong, 32);

        if (DEBUG)
        {
            time = millis();
            Serial.print("Time: ");
            Serial.print(time);
            Serial.print(F("   "));
            Serial.print(irSendBufferLong, HEX);
            Serial.print("   ");
            Serial.print(irSendBufferLong);
            Serial.print("   ");
            Serial.println(ThrottleCommand, DEC);
        }

        delay(30);
    }

    // Now we have done looping through all the lights, and setting them all. 

    // There is an option in UserConfig.h to have the board LEDs reflect car movement, this 
    // can sometimes be useful for debugging. 

    // If moving forward, green led is on, if moving backwards, red led is on. 
    // If stopped, both are off.
    // If braking, both are on. 
    // If right turn, green LED blinks quickly
    // If left turn, red LED blinks quickly
    if (LED_DEBUG)
    {
        if (DriveMode == FWD)  { digitalWrite(RedLED , LOW ) ; digitalWrite(GreenLED, HIGH) ; }
        if (DriveMode == REV)  { digitalWrite(RedLED , HIGH) ; digitalWrite(GreenLED, LOW ) ; }
        if (DriveMode == STOP) { digitalWrite(RedLED , LOW ) ; digitalWrite(GreenLED, LOW ) ; }    
        if (Braking == true)   { digitalWrite(RedLED , HIGH) ; digitalWrite(GreenLED, HIGH) ; }
        if (TurnCommand > 0)   { GreenBlink(); }    // Right turn
        if (TurnCommand < 0)   { RedBlink();   }    // Left turn
    }

}
