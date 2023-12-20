#include <WiFi.h>
#include <Secrets.h>
#include <IR_Sensor.h>

#include <ThingSpeak.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo;

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

WiFiClient  client;

WiFiServer server(80);

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int Sensor_data();
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Dept. of EC ");
  lcd.setCursor(0, 1);
  lcd.print("#~IIST Indore~#");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-->> SMART CAR");
  lcd.setCursor(0, 1);
  lcd.print("PARKING-2022-->>");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);
  pinMode(IR6, INPUT);
  pinMode(IR7, INPUT);
  pinMode(IR8, INPUT);

  // Servo pin.
  myservo.attach(33);

  slot = Total;

  //myservo.write(pos);
  myservo.write(0);

  //flank bit for door open and close
  f = 0;
  g = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected $..");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP address:");
  lcd.setCursor(2, 1);
  lcd.print(WiFi.localIP());
  delay(2000);

  server.begin();
  ThingSpeak.begin(client); // Initialize ThingSpeak
  delay(100);

}

void loop() {

  int x = digitalRead(IR1);
  int y = digitalRead(IR2);
  int z = digitalRead(IR3);
  int a = digitalRead(IR4);
  int b = digitalRead(IR5);
  int c = digitalRead(IR6);
delay(10);
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.

  ThingSpeak.setField(1, x);
  ThingSpeak.setField(2, y);
  ThingSpeak.setField(3, z);
  ThingSpeak.setField(4, a);
  ThingSpeak.setField(5, b);
  ThingSpeak.setField(6, c);
delay(10);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
delay(10);
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");

            client.println();
            client.println("<html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link href=\"https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css\" rel=\"stylesheet\">");
            client.println("<style>html {font-family: Arial, Helvetica, sans-serif;text-align: center;}h1 {font-size: 1.8rem;color: rgb(0, 0, 0);text-shadow: 2px 2px 7px grey;box-shadow: 2px 2px 7px grey;padding: 10px;font-family:Verdana;width: fit-content;margin: 3rem 0;}.animate__fadeInDown{animation-duration: 800ms;} .animate__fadeInRight,.animate__fadeInLeft,.animate__bounceInUp {animation-duration: 1500ms;}.wrapper {width: fit-content;align-items: center;justify-content: center;display: flex;flex-wrap: wrap;}.slot {background-color: rgb(204, 250, 250);width: 9rem;height: 11rem;overflow: hidden;}.area {width: fit-content;}.s1 {margin: 6px;box-shadow: 5px 5px 10px grey;}.car-image {width: 7rem;height: 7rem;padding-top: 2rem;filter: opacity(0.1) drop-shadow(0 0 0 rgba(0, 0, 0));transition: all .5s;}.car-image:hover{transform: scale(1.25);} .btn {margin-top: 30px;background-color: aqua;border: 0.5px solid rgb(208, 196, 196);border-radius: 5px;height: 3rem;text-decoration: none;transition: all 0.5s; padding: 0.8rem ;font-size: 1.2rem; cursor: pointer;}.btn:hover{box-shadow:5px 5px 10px grey;transform: translate(15px,15px) scale(1.15);}.line1,.line2{width: 29rem;height: 5px; }.line1{background-color: rgb(0, 0, 0);}@media(max-width:500px) {.slot {max-width: 6rem;height: 8rem;}.car-image {width: 5rem;height: 5rem;}.line1,.line2{width: 20rem;height: 3px; }h1{font-size: 1.3rem;}}</style></head>");
            client.println("<body onload = \"JavaScript:AutoRefresh(5000);\">");
            client.println("<center>");
            client.println("<h1 class=\"animate__fadeInDown\"\">SMART CAR PARKING SYSTEM USING IOT</h1>");
            client.println("<div class=\"area\"\"><div class=\"wrapper animate__fadeInLeft\"\">");

            if (digitalRead(IR1) == 0)
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\" style=\"background-color: #ffcccc;\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt=""></div><p><b>Parked</b></p></div>");

            }
            else
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt="" style=\"filter: opacity(0.6) drop-shadow(0 0 rgb(255, 0, 0));\"\"></div><p><b> Slot 1</b></p></div>");

            }
            if (digitalRead(IR2) == 0)
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\" style=\"background-color: #ffcccc;\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt=""></div><p><b>Parked</b></p></div>");

            }
            else
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt="" style=\"filter: opacity(0.6) drop-shadow(0 0 rgb(255, 0, 0));\"\"></div><p><b> Slot 2</b></p></div>");

            }
            if (digitalRead(IR3) == 0)
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\" style=\"background-color: #ffcccc;\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt=""></div><p><b>Parked</b></p></div>");
            }
            else
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt="" style=\"filter: opacity(0.6) drop-shadow(0 0 rgb(255, 0, 0));\"\"></div><p><b> Slot 3</b></p></div>");
            }

            client.println("</div></div>");

            client.println("<div class=\"line2\"\"></div>");
            client.println("<div class=\"line1 animate__bounceInUp\"\"></div>");
            client.println("<div class=\"line2\"\"></div>");

            client.println("<div class=\"area\"\"><div class=\"wrapper animate__fadeInRight\"\">");
            if (digitalRead(IR4) == 0)
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\" style=\"background-color: #ffcccc;\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt=""></div><p><b>Parked</b></p></div>");
  
            }
            else
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt="" style=\"filter: opacity(0.6) drop-shadow(0 0 rgb(255, 0, 0));\"\"></div><p><b> Slot 4</b></p></div>");
        
            }
            if (digitalRead(IR5) == 0)
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\" style=\"background-color: #ffcccc;\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt=""></div><p><b>Parked</b></p></div>");
              s5 = 0;
            }
            else
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt="" style=\"filter: opacity(0.6) drop-shadow(0 0 rgb(255, 0, 0));\"\"></div><p><b> Slot 5</b></p></div>");
         
            }
            if (digitalRead(IR6) == 0)
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\" style=\"background-color: #ffcccc;\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt=""></div><p><b>Parked</b></p></div>");
          
            }
            else
            {
              client.println("<div class=\"s1\"\"><div class=\"slot\"\"><img src=\"https://github.com/JAGAHPEE/SMART-CAR-PARKING-SYSTEM-USING-IOT/blob/main/car-top-view.png?raw=true\"\" class=\"car-image\"\" alt="" style=\"filter: opacity(0.6) drop-shadow(0 0 rgb(255, 0, 0));\"\"></div><p><b> Slot 6</b></p></div>");
         
            }
            client.println("</div></div>");
            client.println("<button onClick=\"window.location.href=window.location.href\"\" class=\"btn\"\"><b><i>Refresh</i></b></button>");
            client.println();
            client.println("<br>");
            client.println("<br>");
            client.println("<br>");


            client.println("</center>");
            client.println("</body>");
            client.println("</html>");
            client.println();
            client.println();
            // The HTTP response ends with another blank line:
            client.println();
            delay(10);
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

      }
    }
    // close the connection:
    //    client.stop();
    Serial.println("Client Disconnected.");
  }
  delay(10);

  slot=Sensor_data();
//  slot = s1 + s2 + s3 + s4 + s5 + s6;
  if (digitalRead(IR7) == 0)
  {
    if (slot > 0 && slot <= 6)
    {
      f = 1;
//      slot = slot - 1;
//      Car Leaving..
    lcd.clear();
//  lcd.setCursor(0, 0);
    lcd.print("Car Enters");

    }
    delay(100);
  }
  else if (digitalRead(IR8) == 0)
  {
    if (slot >= 0 && slot < 6)
    {
      g = 1;
    lcd.clear();
    lcd.print("Car Leaving");
//      slot = slot + 1;
// Car Entering...

    }
    delay(100);
  }
  if (f == 1 || g == 1)
  { 
    lcd.clear();
    lcd.print("Gate Opening");
    for (pos = 0; pos <= 100; pos += 10)
    {
      myservo.write(pos);
      Serial.println(pos);
      delay(10);
    }
    delay(50);
    if (f == 1)
    {
      while ((digitalRead(IR8) == 0) || (digitalRead(IR7) == 0))
      {
        lcd.clear();
        lcd.print("Enter/Exit Fast");
        delay(500);
      }
    }
    else if (g == 1)
    {
      while (digitalRead(IR7) == 0 || digitalRead(IR8) == 0)
      {
        lcd.clear();
        lcd.print("Enter/Exit Fast");
        delay(500);
      }
    }
    lcd.clear();
    lcd.print("Gate Closeing");
    for (pos = 100; pos >= 0; pos -= 10)
    {
      myservo.write(pos);
      Serial.println(pos);
      delay(10);
    }
    delay(50);
    f = 0;
    g = 0;
  }
  delay(1000);
  slot=Sensor_data();
  if (slot == 6)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Parking Space");
    delay(1000);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slot Available");
    lcd.setCursor(0, 1);
    lcd.print(Total - slot);
    delay(1000);
  
   }
   int s=Total - slot;
Serial.println(s);
delay(500);

}
int Sensor_data()
{
    s1=digitalRead(IR1)==0?1:0;
    s2=digitalRead(IR2)==0?1:0;
    s3=digitalRead(IR3)==0?1:0;
    s4=digitalRead(IR4)==0?1:0;
    s5=digitalRead(IR5)==0?1:0;
    s6=digitalRead(IR6)==0?1:0;
    s7=digitalRead(IR7)==0?1:0;
    s8=digitalRead(IR8)==0?1:0;
    unsigned int q = s1 + s2 + s3 + s4 + s5 + s6;
    return q; 
}
