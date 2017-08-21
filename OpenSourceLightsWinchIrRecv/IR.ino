void GetIRRecvData(decode_results *results)
{
    if (irrecv.decode(results))      // Grab an IR code
    {
        if (((long)results->value > 0) && ((long)results->value < 1100000000))
        {
            sprintf(irRecvData, "%08lX", (long)results->value);

            ThrottleCommand = (htoi(irRecvData[6])*16 + htoi(irRecvData[7])) - 100;

            if (DEBUG == true)
            {
                time = millis();
                Serial.println(F("   "));
                Serial.print("Time: ");
                Serial.print(time);
                Serial.print(F("   "));
                Serial.print(irRecvData);
                Serial.print(F("   "));
                Serial.print((long)results->value);
                Serial.print(F("   "));
                Serial.println(ThrottleCommand);
            }
            
        }

        irrecv.resume();              // Prepare for the next value
    }
}
  

unsigned long ircode (decode_results *results)
{
  unsigned long irBuffer;
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    Serial.print(results->address, HEX);
    Serial.print(":");
  }

  // Print Code
  //Serial.print(results->value, HEX);
  sprintf(irBuffer, "08%X", results->value);
  return irBuffer;
}

//+=============================================================================
// Display encoding type
//
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
    case DENON:        Serial.print("Denon");         break ;
  }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpInfo (decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}
