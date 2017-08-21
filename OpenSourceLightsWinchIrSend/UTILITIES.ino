

void PrintSpaceDash()
{
    Serial.print(F(" - "));
}

void PrintSpace()
{
    Serial.print(F("  "));
}    

void PrintHorizontalLine()
{
    Serial.println(F("-----------------------------------"));
}

void PrintTrueFalse(boolean boolVal)
{
    if (boolVal == true) { Serial.println(F("TRUE")); } else { Serial.println(F("FALSE")); }
}

//does not check that input is valid
int htoi (char c)
{
   if (c<='9')
       return c-'0';
   if (c<='F')
       return c-'A'+10;
   if (c<='f')
       return c-'a'+10;
   return 0;
}
