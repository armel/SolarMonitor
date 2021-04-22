// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Multitasking task for retreive propag data and greyline
void hamdata(void *pvParameters)
{
  File f;
  HTTPClient http;
  uint32_t limit = 1 * 30 * 1000; // Retry 30 secondes
  uint16_t len, httpCode;
  static uint8_t counter = 1;

  for (;;)
  {
    Serial.println(counter);

    if ((WiFi.status() == WL_CONNECTED)) // Check the current connection status
    {
      //Serial.println("HamQTH");
      clientHamQTH.setInsecure();
      http.begin(clientHamQTH, endpointHamQTH);       // Specify the URL
      http.addHeader("Content-Type", "text/plain");   // Specify content-type header
      http.setTimeout(1000);                          // Set Time Out
      httpCode = http.GET();                          // Make the request
      if (httpCode == 200)                            // Check for the returning code
      {
        HamQTHData = http.getString(); // Get data  
      }
      http.end(); // Free the resources
    }

    if(counter == 1) {
      if ((WiFi.status() == WL_CONNECTED)) // Check the current connection status
      {
        //Serial.println("Greyline");
        clientGreyline.setInsecure();
        http.begin(clientGreyline, endpointGreyline);   // Specify the URL
        http.setTimeout(1000);                          // Set Time Out
        httpCode = http.GET();                          // Make the request
        if (httpCode == 200)                            // Check for the returning code
        {
          greylineData = http.getString(); // Get data

          greylineData.replace("<img src=\"", ">>>");
          greylineData.replace("\" alt=\"Grey Line Map\"", "<<<");

          int16_t parenthesisBegin = greylineData.indexOf(">>>");
          int16_t parenthesisLast = greylineData.indexOf("<<<");

          if (parenthesisBegin > 0)
          {
            greylineData = greylineData.substring(parenthesisBegin + 4, parenthesisLast);
          }

          http.begin(clientGreyline, greylineData);     // Specify the URL
          httpCode = http.GET();                        // Make the request
          if (httpCode == 200)                          // Check for the returning code
          {
            if (httpCode == 200) {
              greylineRefresh = 1;
              // Open file
              f = SPIFFS.open("/greyline.jpg", "w+");

              // Get size
              len = http.getSize();
              // Create buffer for read
              uint8_t buff[1024] = { 0 };

              // Get TCP stream
              WiFiClient *stream = &clientGreyline;

              // Read all data from server
              while (http.connected() && (len > 0 || len == -1)) {
                int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff)));          
                // Write it to file
                f.write(buff, c);
                if (len > 0) {
                  len -= c;
                }
              }
              // Close file
              f.close();
            }
          }
        }
        http.end(); // Free the resources
      }

      vTaskDelay(pdMS_TO_TICKS(1000));

      if ((WiFi.status() == WL_CONNECTED)) // Check the current connection status
      {
        //Serial.println("HamQSL");
        http.begin(clientHamQSL, endpointHamQSL);       // Specify the URL
        http.addHeader("Content-Type", "text/plain");   // Specify content-type header
        http.setTimeout(1000);                          // Set Time Out
        httpCode = http.GET();                          // Make the request
        if (httpCode == 200)                            // Check for the returning code
        {
          HamQSLData = http.getString(); // Get data
        }
        http.end(); // Free the resources
      }
    }
    
    counter = (counter++ < 10) ? counter : 1;

    // Pause
    vTaskDelay(pdMS_TO_TICKS(limit));
  }
}

// Manage buttons
void button(void *pvParameters)
{
  int8_t right;
  int8_t left;
  int8_t change;

  for (;;)
  {
    getButton();

    /*
    if (btnA || btnB || btnC)
    {
      Serial.println(".");
      Serial.print(btnA);
      Serial.print("-");
      Serial.print(btnB);
      Serial.print("-");
      Serial.println(btnC);
    }
    else
    {
      Serial.print(".");
    }
    */

    if (M5.Lcd.getRotation() == 1) {
      right = 1;
      left = -1;
    } else {
      right = -1;
      left = 1;
    }

    if ((btnA || btnB || btnC))
    {
      screensaver = millis(); // Screensaver update !!!
      if(screensaverMode == 1)
      {
        //Serial.println("Wake up");
        btnA = 0;
        btnB = 0;
        btnC = 0;
      }
      else
      {
        change = alternance;
        if(btnA)
        {
          change += left;
        }
        else if(btnC)
        {
          change += right;
        }
        else if(btnB)
        {
          screenRefresh = 1;
        }

        change = (change < 0) ? 11 : change;
        change = (change > 11) ? 0 : change;

        if (change != alternance)
        {
          alternance = change;
          screenRefresh = 1;
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}