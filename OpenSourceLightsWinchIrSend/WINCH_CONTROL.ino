int motorPwm;

// CH3 Winch Mode
void WinchControl()
{

  Channel3 = Pos3;
  
  Channel3Pulse = pulseIn(Channel3_Pin, HIGH, ServoTimeout);

  if (DEBUG)
  {
    Serial.print(F("Ch3 : "));
    Serial.print(Channel3);
    Serial.print(F(" Ch4 Last : "));
    Serial.print(Channel4LastCommand);
    Serial.print(F(" Winch pulseVal: "));
    Serial.println(Channel3Pulse);
  }

  if (Channel4LastCommand == Pos2)
  {
    if (DEBUG)
    {
      PrintSpaceDash();
      Serial.println(F("Winch Mode Start! - CH3 MOVE MORE THAN CH3 DEAD BAND!"));
      PrintSpaceDash();
    }

    // If Switch Winch Mode, CH3 Change
    if (DEBUG)
    {
      Serial.println(F("Channel3LastPulse >=  Channel3Pulse - Channel3Deadband"));
      Serial.print(Channel3LastPulse);
      Serial.print(F(" >= "));
      Serial.println(Channel3Pulse - Channel3Deadband);

      Serial.println(F("Channel3LastPulse <=  Channel3Pulse + Channel3Deadband"));
      Serial.print(Channel3LastPulse);
      Serial.print(F(" <= "));
      Serial.println(Channel3Pulse + Channel3Deadband);
    }

    // CH3 No Changed
    if (((Channel3LastPulse >= (Channel3Pulse - Channel3Deadband)) && (Channel3LastPulse  <= (Channel3Pulse + Channel3Deadband))) || Channel3LastPulse == 0)
    {
      setWinchOff(HIGH);

      // Save CH4 Last Position
      Channel4LastCommand = Pos2;
    }
    // CH3 Changed
    else
    {
      // Save CH3 Last Position
      Channel3LastPulse = Channel3Pulse;

      Channel4LastCommand = Pos3;
    }
  }
  // CH3 Out of Range : Winch Off
  else if (Channel3Pulse < (Channel3PulseMin - Channel3Deadband) || Channel3Pulse > (Channel3PulseMax + Channel3Deadband))
  {
    setWinchOff(LOW);
  }
  // CH3 Center : Winch LOCK
  else if (Channel3Pulse > (Channel3PulseCenter - Channel3Deadband) && Channel3Pulse < (Channel3PulseCenter + Channel3Deadband))
  {
    setWinchOff(HIGH);
  }
  else
  {
    motorPwm = map(Channel3Pulse, Channel3PulseMin, Channel3PulseMax, -250, 250);

    if (DEBUG)
    {
      //Serial.print(F("motorPwm: "));
      //Serial.print(motorPwm);
      //Serial.print(F("       "));
    }
    
    if(motorPwm < 0){
      analogWrite(WinchEnablePin, motorPwm*(-1));
      digitalWrite(WinchPhasePin, LOW);
      digitalWrite(WinchSleepPin, HIGH);

      if (DEBUG)
      {
        //Serial.print(F("CAL PWM: "));
        //Serial.println(motorPwm*(-1));
      }
    }
  
    if(motorPwm > 0)
    {
      analogWrite(WinchEnablePin, motorPwm);
      digitalWrite(WinchPhasePin, HIGH);
      digitalWrite(WinchSleepPin, HIGH);

      if (DEBUG)
      {
        //Serial.print(F("CAL PWM: "));
        //Serial.println(motorPwm);
      }
    }
   
    if(motorPwm >= -30 && motorPwm <= 30){
      setWinchOff(LOW);
    }
  }
}

void setWinchOff(int SleepPinStat)
{
  digitalWrite(WinchEnablePin, LOW);
  digitalWrite(WinchPhasePin, LOW);
  digitalWrite(WinchSleepPin, SleepPinStat);
}

