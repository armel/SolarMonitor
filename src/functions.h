// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Parse data
String getValue(String data, char separator, uint16_t index)
{
  int16_t strIndex[] = {0, -1};
  int16_t found = 0;
  int16_t maxIndex = data.length() - 1;

  for (uint16_t i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Clear screen
void clear()
{
  M5.Lcd.fillRect(0, 0, 320, 44, M5.Lcd.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
  M5.Lcd.drawFastHLine(  0, 0, 320, TFT_WHITE);
  M5.Lcd.drawFastHLine(  0, 44, 320, TFT_WHITE);
  M5.Lcd.drawFastHLine(  0, 100, 320, TFT_WHITE);
}

// Manage message cycle
String binarise()
{
  switch(messageCurrent)
  {
    case 0: return "00"; break;
    case 1: return "01"; break;
    case 2: return "10"; break;
    case 3: return "11"; break;
  }

  return "00";
}

// Build scroll A
void buildScrollA()
{
  int16_t h = 20;
  int16_t w;

  imgA.setFreeFont(&FreeSans9pt7b); 
  w = imgA.textWidth(messageA) + 80;
  // We could just use fillSprite(color) but lets be a bit more creative...
  while (h--)
    imgA.drawFastHLine(0, h, w, TFT_BLACK);

  // Now print text on top of the graphics
  imgA.setTextColor(TFT_WHITE); // White text, no background colour
  imgA.setTextWrap(false);      // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  imgA.drawString(messageA, posA, 2);
  imgA.drawString(messageA, posA - w, 2);
}

// Scroll A
void scrollA(uint8_t pause)
{
  // Sprite for scroll
  buildScrollA();
  imgA.pushSprite(0, 52);

  posA -= 1;
  if (posA == 0)
  {
    //posA = M5.Lcd.width();
    imgA.setFreeFont(&FreeSans9pt7b); 
    posA = imgA.textWidth(messageA) + 80;
  }

  delay(pause);
}

// Build scroll B
void buildScrollB()
{
  int16_t h = 20;
  int16_t w;

  imgB.setTextSize(1);          // Font size scaling is x1
  imgB.setTextFont(2);          // Font 2 selected
  w = imgB.textWidth(messageB) + 80;
  // We could just use fillSprite(color) but lets be a bit more creative...
  while (h--)
    imgB.drawFastHLine(0, h, w, TFT_BLACK);

  // Now print text on top of the graphics
  imgB.setTextColor(M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b)); // Gray text, no background colour
  imgB.setTextWrap(false);      // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  imgB.drawString(messageB, posB, 2);
  imgB.drawString(messageB, posB - w, 2);
}

// Scroll B
void scrollB(uint8_t pause)
{
  // Sprite for scroll
  buildScrollB();
  imgB.pushSprite(0, 74);

  posB -= 1;
  if (posB == 0)
  {
    //posB = M5.Lcd.width();
    imgB.setTextSize(1);          // Font size scaling is x1
    imgB.setTextFont(2);          // Font 2 selected
    posB = imgB.textWidth(messageB) + 80;
  }

  delay(pause);
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

  if(alternance % 2 == 0)
  {
    // Update date and time
    tmpString = hamQSLData;
    tmpString.replace("<updated>", "(");
    tmpString.replace("</updated>", ")");
    parenthesisBegin = tmpString.indexOf("(");
    parenthesisLast = tmpString.indexOf(")");
    if (parenthesisBegin > 0)
    {
      tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
    }

    tmpString.trim();
  }
  else
  {
    tmpString = String(NAME) + " Version " + String(VERSION);
  }

  M5.Lcd.drawString(tmpString, 160, 36);

// On right, view reload data
  tmpString = reloadState;
  tmpString.trim();

  if(tmpString != "")
  {
    /*
    M5.Lcd.fillRect(2, 38, 13, 5, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
    M5.Lcd.fillRect(3, 38, 11, 4, M5.Lcd.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    M5.Lcd.drawFastVLine(8, 31, 10, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
    M5.Lcd.drawLine(8, 40, 5, 37, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
    M5.Lcd.drawLine(8, 40, 11, 37, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
    */

    M5.Lcd.drawFastHLine(2, 35, 10, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
    M5.Lcd.drawLine(12, 35, 8, 31, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
    M5.Lcd.drawLine(12, 35, 8, 39, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));

    //M5.Lcd.drawFastHLine(0, 30, 320, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));

    M5.Lcd.setTextColor(M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b), M5.Lcd.color565(TFT_BACK.r, TFT_BACK.g, TFT_BACK.b));
    M5.Lcd.setTextPadding(60);
    M5.Lcd.setTextDatum(ML_DATUM);
    M5.Lcd.drawString(tmpString, 18, 36);
  }

  // On left, view battery level
  uint8_t val = map(getBatteryLevel(1), 0, 100, 0, 16);
  M5.Lcd.drawRect(294, 30, 20, 12, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
  M5.Lcd.drawRect(313, 33, 4, 6, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
  M5.Lcd.fillRect(296, 32, val, 8, M5.Lcd.color565(TFT_GRAY.r, TFT_GRAY.g, TFT_GRAY.b));
}

// Draw Propag Data
void propagData()
{
  // Title
  solarData[alternance].toUpperCase();

  // Current value
  tmpString = hamQSLData;
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
}

// Draw Propag Message
void propagMessage()
{
  if(binarise().charAt(1) == '0')
  {
    messageB = "VHF Conditions -- ";

    for (uint8_t i = 0; i < 4; i++)
    {
      // Current propagation 50 MHz
      tmpString = hamQSLData;
      tmpString.replace(skipKey[i], "(");
      tmpString.replace("</phenomenon>", ")");
      parenthesisBegin = tmpString.indexOf("(");
      parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
      if (parenthesisBegin > 0)
      {
        tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
      }

      tmpString.trim();

      messageB += skipData[i] + " : " + tmpString;
      messageB += " -- ";
    }

    messageB = messageB.substring(0, messageB.length() - 4);
  }
  else if(binarise().charAt(1) == '1')
  {
    messageB = "HF Conditions -- ";

    // Day
    for(uint8_t i = 0; i <= 3; i += 1)
    {
      tmpString = hamQSLData;
      tmpString.replace(propagKey[i], "(");
      tmpString.replace("</band>", ")");
      parenthesisBegin = tmpString.indexOf("(");
      parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
      if (parenthesisBegin > 0)
      {
        tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
      }
      tmpString.trim();
      //tmpString.toUpperCase();

      messageB += propagKey[i].substring(0, 7) + " Day " + tmpString + " -- ";
    }

    // Night
    for(uint8_t i = 4; i <= 7; i += 1)
    {
      tmpString = hamQSLData;
      tmpString.replace(propagKey[i], "(");
      tmpString.replace("</band>", ")");
      parenthesisBegin = tmpString.indexOf("(");
      parenthesisLast = tmpString.indexOf(")", parenthesisBegin);
      if (parenthesisBegin > 0)
      {
        tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
      }
      tmpString.trim();
      //tmpString.toUpperCase();

      messageB += propagKey[i].substring(0, 7) + " Night " + tmpString + " -- ";
    }
    messageB = messageB.substring(0, messageB.length() - 4);
  }
}

// Draw Cluster Message
void clusterAndSatMessage()
{
  boolean exclude = 0;
  uint8_t counter = 0;
  uint32_t tmp = 0;

  if(binarise().charAt(0) == '0')
  {
    size_t n = sizeof(frequencyExclude)/sizeof(frequencyExclude[0]);

    messageA = "";
    hamQTHData.replace("\n", "|");

    for (uint8_t i = 0; i < 50; i++)
    {
      cluster[i] = getValue(hamQTHData, '|', i);
      frequency[i] = getValue(cluster[i], '^', 1);
      tmp = frequency[i].toInt();
      
      exclude = 0;

      for (uint8_t j = 0; j < n; j++)
      {
        if(abs(tmp - frequencyExclude[j]) <= 2 || tmp > 470000)
        {
          exclude = 1;
          break;
        }
      }

      if(exclude == 0)
      {    
        call[i] = getValue(cluster[i], '^', 0);
        band[i] = getValue(cluster[i], '^', 8);
        country[i] = getValue(cluster[i], '^', 9);

        messageA += call[i] + " " + band[i] + " " + frequency[i] + " " + country[i] + " -- ";
        counter += 1;
      }

      if(counter == 10) 
      {
        break;
      }
    }
    if(messageA != "")
    {
      messageA = "DX Cluster -- " + messageA;
      messageA = messageA.substring(0, messageA.length() - 4);
    }
    else
    {
      messageA = "DX Cluster -- Data acquisition on the way, please wait...";
    }
  }
  else if(binarise().charAt(0) == '1')
  {
    messageA = "";
    if(satData.length() > 32)
    {
      messageA = satData.substring(15, satData.length() - 3);
    }
    if(messageA != "")
    {
      messageA = "Satellites Passes -- " + messageA;
    }
    else
    {
      messageA = "Satellites Passes -- Data acquisition on the way, please wait...";
    }
  }
}

// Draw Greyline
void greyline()
{
  if(greylineRefresh == 1)
  {
    // Draw greyline
    decoded = JpegDec.decodeFsFile("/greyline.jpg");
    if (decoded) {
      M5.Lcd.drawJpgFile(SPIFFS, "/greyline.jpg", 0, 101, 320, 139, 0, 11, JPEG_DIV_2);
    }
    greylineRefresh = 0;
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
      M5.Lcd.clear();
      clear();
      greylineRefresh = 1;
    }
    else if(int(accY * 1000) > 500 && M5.Lcd.getRotation() != 1) {
      M5.Lcd.setRotation(1);
      M5.Lcd.clear();
      clear();
      greylineRefresh = 1;
    }
  }
}

// Manage temporisation
void temporisation()
{
  for(uint16_t i = 0; i <= 200; i += 1)
  {
    if(screenRefresh == 1)
    {
      break;
    }
    scrollA(5);
    scrollB(5);
  }
}