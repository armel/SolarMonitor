// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Clear screen
void clear()
{
  M5.Lcd.fillRect(0, 0, 320, 44, M5.Lcd.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
  M5.Lcd.drawFastHLine(  0, 0, 320, TFT_WHITE);
  M5.Lcd.drawFastHLine(  0, 44, 320, TFT_WHITE);
  M5.Lcd.drawFastHLine(  0, 100, 320, TFT_WHITE);
}

// Build scroll
void buildScroll()
{
  int16_t h = 20;
  int16_t w = M5.Lcd.width();

  // We could just use fillSprite(color) but lets be a bit more creative...
  while (h--)
    img.drawFastHLine(0, h, w, TFT_BLACK);

  // Now print text on top of the graphics
  img.setTextSize(1);          // Font size scaling is x1
  img.setTextFont(2);          // Font 2 selected
  //img.setFreeFont(&tahoma6pt7b);
  
  img.setTextColor(TFT_WHITE); // White text, no background colour
  img.setTextWrap(false);      // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  img.setCursor(pos, 2); // Print text at pos
  img.print(message);

  img.setCursor(pos - w, 2); // Print text at pos - sprite width
  img.print(message);
}

// Scroll
void scroll(uint8_t pause)
{
  // Sprite for scroll
  buildScroll();
  img.pushSprite(0, 78);

  pos -= 1;
  if (pos == 0)
  {
    pos = M5.Lcd.width();
  }

  delay(pause);
}

// Manage temporisation
void temporisation()
{
  for(uint16_t i = 0; i <= 500; i += 1)
  {
    if(screenRefresh == 1)
    {
      break;
    }
    scroll(10);
  }
}

// Detect rotation
void getAcceleration()
{
  float accX = 0.0F;
  float accY = 0.0F;
  float accZ = 0.0F;

  if(BOARD == GREY || BOARD == CORE2) {
    M5.IMU.getAccelData(&accX,&accY,&accZ);

    if(int(accY * 1000) < -500 && M5.Lcd.getRotation() != 3) {
      M5.Lcd.setRotation(3);
    }
    else if(int(accY * 1000) > 500 && M5.Lcd.getRotation() != 1) {
      M5.Lcd.setRotation(1);
    }
  }
}

// Draw title
void title(String title)
{
  // Title
  M5.Lcd.setTextColor(TFT_WHITE, M5.Lcd.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
  M5.Lcd.setFreeFont(&dot15pt7b);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(320);
  M5.Lcd.drawString(title, 160, 16);

  M5.Lcd.setFreeFont(0);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(320);

  // Update date and time
  tmpString = xmlData;
  tmpString.replace("<updated>", "(");
  tmpString.replace("</updated>", ")");
  parenthesisBegin = tmpString.indexOf("(");
  parenthesisLast = tmpString.indexOf(")");
  if (parenthesisBegin > 0)
  {
    tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
  }

  tmpString.trim();

  M5.Lcd.drawString(tmpString, 160, 36);
}

// Draw Propag Data
void propagData()
{
  // Title
  solarData[alternance].toUpperCase();

  // Current value
  tmpString = xmlData;
  tmpString.replace("<" + solarKey[alternance] + ">", "(");
  tmpString.replace("</" + solarKey[alternance] + ">", ")");
  parenthesisBegin = tmpString.indexOf("(");
  parenthesisLast = tmpString.indexOf(")");
  if (parenthesisBegin > 0)
  {
    tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
  }

  tmpString.trim();

  title(solarData[alternance] + " " + tmpString);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setFreeFont(&rounded_led_board10pt7b);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(320);
}

// Draw Propag Message
void propagMessage()
{
  // Current propagation 50 MHz
  tmpString = xmlData;
  tmpString.replace("location=\"europe_6m\">", "(");
  tmpString.replace("</phenomenon>", ")");
  parenthesisBegin = tmpString.indexOf("(");
  parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
  if (parenthesisBegin > 0)
  {
    tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
  }

  tmpString.trim();

  message = "E-Skip / Europe 6m / " + tmpString;
}

// Draw Propag Condition
void propagCondition()
{
  for(uint8_t i = 0; i < 56; i += 1)
  {
    M5.Lcd.drawFastHLine(  0, 44 + i, 320, M5.Lcd.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    M5.Lcd.drawFastHLine(  0, 44 + i + 1, 320, TFT_WHITE);
    if (i < 28)
    {
      scroll(10);
    }
    else
    {
      delay(10);
    }
  }

  title("BANDS CONDITIONS");

  // Current propagation
  M5.Lcd.setFreeFont(&DroidSansMono6pt7b);
  M5.Lcd.setTextPadding(160);
  M5.Lcd.setTextDatum(CL_DATUM);

  // Day
  for(uint8_t i = 0; i <= 3; i += 1)
  {
    tmpString = xmlData;
    tmpString.replace(propagKey[i], "(");
    tmpString.replace("</band>", ")");
    parenthesisBegin = tmpString.indexOf("(");
    parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
    if (parenthesisBegin > 0)
    {
      tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
    }
    tmpString.trim();
    tmpString.toUpperCase();

    M5.Lcd.drawString(propagKey[i].substring(0, 7) + " DAY " + tmpString, 20, 48 + (14 * i));
  }

  M5.Lcd.setTextDatum(CR_DATUM);

  // Night
  for(uint8_t i = 4; i <= 7; i += 1)
  {
    tmpString = xmlData;
    tmpString.replace(propagKey[i], "(");
    tmpString.replace("</band>", ")");
    parenthesisBegin = tmpString.indexOf("(");
    parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
    if (parenthesisBegin > 0)
    {
      tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
    }
    tmpString.trim();
    tmpString.toUpperCase();

    M5.Lcd.drawString(propagKey[i].substring(0, 7) + " NIGHT " + tmpString, 300, 48 + (14 * (i - 4)));
  }

  for(uint16_t i = 0; i <= 500; i += 1)
  {
    if(screenRefresh == 1)
    {
      break;
    }
    delay(10);
  }

  for(uint8_t i = 0; i < 56; i += 1)
  {
    M5.Lcd.drawFastHLine(  0, 99 - i, 320, TFT_BLACK);
    M5.Lcd.drawFastHLine(  0, 99 - i - 1, 320, TFT_WHITE);
    if (i > 28)
    {
      scroll(10);
    }
    else
    {
      delay(10);
    }
  }
}