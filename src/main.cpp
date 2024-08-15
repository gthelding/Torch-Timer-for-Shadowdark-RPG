// Includes
#include <header.h>            //  Include the header file for the project to keep this cleaner
#include <secrets.h>           // Include the secrets file for WiFi credentials

bool isTorchLit = false; // Global boolean to track if the torch is lit, initial value is false
int duration = 0; // Global variable to store the duration of the torch being lit

// Fire Up Web Server
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// put function declarations here:  
void Torch_Function(int duration); // Function to control the torch
String getRootPage(); // Function to generate the root page

// put your setup code here, to run once:
void setup() {
  Serial.begin(115200); //Serial Monitor
  u8g2.begin(); //Initialize OLED
  delay(2000); // Sanity Delay

  WiFi.mode(WIFI_STA); // Set WiFi mode to Station
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi (SSID and Password from secrets.h - you will need to create this file)
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

// Print WiFi status to Serial Monitor  
  Serial.println("\nConnected");
  Serial.println((String)"[+] RSSI : " + WiFi.RSSI() + " dB");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

//Print WiFi status to OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_courB10_tf);
  u8g2.drawStr(0, 10, "Torch is Out!");
  u8g2.drawStr(0, 27, "IP:");
  u8g2.drawStr(0, 42, WiFi.localIP().toString().c_str());
//  u8g2.sendBuffer();
  std::string rssi;
  rssi.append("RSSI: ").append(std::to_string(WiFi.RSSI())).append(" dB");
  u8g2.drawStr(0, 57, rssi.c_str()); 
  u8g2.sendBuffer();
//NTP Setup
timeClient.begin();
timeClient.update();

//Flame Effect Setup
FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
FastLED.setBrightness(g_Brightness);
set_max_power_indicator_LED(LED_BUILTIN);                               // Light the builtin LED if we power throttle
FastLED.setMaxPowerInMilliWatts(g_PowerLimit);                          // Set the power limit, above which brightness will be throttled

// Web Server Setup
// Serve the root page
 server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", getRootPage());
    Serial.println("Root page requested");
  });

// Respond to requests to light the torch
  server.on("/light-torch", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasArg("duration")) {
      duration = request->arg("duration").toInt();
    }
    isTorchLit = true; // Set flag to true to indicate the torch is lit
    request->send(200, "text/plain", "Torch lit for " + String(duration / 60) + " minutes");
    Serial.println("Light torch requested with duration: " + String(duration));
  });

// Respond to requests to extinquish the torch
  server.on("/extinguish-torch", HTTP_GET, [](AsyncWebServerRequest * request) {
    isTorchLit = false; // Set flag to false to indicate the torch is not lit (Torch function while loop will exit) 
    request->send(200, "text/plain", "Torch extinquished");
    Serial.println("Extinquish torch requested");
    FastLED.clear(true); // Clear the LED strip
     u8g2.clearBuffer();
     u8g2.drawStr(0, 10, "Torch is Out!");
     u8g2.sendBuffer();
  });

  server.begin();
}

// put your main code here, to run repeatedly:
void loop() {
// If the torch is lit, call the Torch Function with the provided duration
if(isTorchLit == true){
  Torch_Function(duration);
}

isTorchLit = false; // Reset the torch lit flag to avoid infinite loop 

  ws.cleanupClients();
  delay(100);
}


//Function Definitions

void Torch_Function(int duration) {
  Serial.println("Torch Function called");
  // Countdown Timer
  const unsigned long COUNTDOWN_TIME = duration; // Set the countdown time in seconds based on duration from web form
  unsigned long startTime;
  unsigned long currentTime;
  unsigned long elapsedTime = 0;
  unsigned long lastExecutionTime = 0;
  startTime = millis(); // Record the starting time
// Begin Flame Effect Code
bool bLED = 0;

//Prototupe fire effect for single string - modify for individual deployment
ClassicFireEffect fire(NUM_LEDS, 10, 100, 4, 4, true, true);    // Inwards toward Middle
int originalBrightness = g_Brightness;
while (isTorchLit == true && elapsedTime <= COUNTDOWN_TIME) {
  currentTime = millis(); // Get the current time
  elapsedTime = (currentTime - startTime) / 1000; // Calculate elapsed time in seconds
    unsigned long remainingTime = COUNTDOWN_TIME - elapsedTime;
    FastLED.clear();
    fire.DrawFire();
    FastLED.show(g_Brightness);                          //  Show and delay
    delay(33); //original value is 33ms
    // Display remaining time in Minutes:Seconds format
    unsigned int minutes = remainingTime / 60;
    unsigned int seconds = remainingTime % 60;
    std::string timeRemaining;
    if (currentTime - lastExecutionTime >= 1000) {
      lastExecutionTime = currentTime;
      //Serial.println("Time remaining: " + String(minutes) + ":" + String(seconds));
      u8g2.clearBuffer();
      u8g2.drawStr(0, 10, "Torch Lit!");
      u8g2.drawStr(0, 27, "Time Remaining:");
      timeRemaining = std::to_string(minutes) + ":" + std::to_string(seconds);
      u8g2.drawStr(0, 42, timeRemaining.c_str());
      u8g2.sendBuffer();
      if (minutes == 0 && seconds == 0) {
         Serial.println("Time's up!");
         u8g2.clearBuffer();
         u8g2.drawStr(0, 10, "Torch is Out!");
         u8g2.sendBuffer();
         }
    }
    } 
  FastLED.clear(true);
  }

  //Generate Web Page 
  String getRootPage() {
  String page = R"rawliteral(
  <html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Torch Timer</title>
    <style>
        body {
            font-family: Arial, sans-serif;
        }
        
        .timer {
            font-size: 36px;
            font-weight: bold;
            margin-bottom: 20px;
        }
        
        .duration {
            margin-top: 20px;
        }
        
        input[type="radio"] {
            margin-right: 10px;
        }
    </style>
</head>
<body>
    <h1>Torch Timer</h1>
    <div style="background-image: url('https://helding.net/wp-content/uploads/2024/08/Third-Party-Shadowdark-Logo-Black.png'); background-size: contain; background-repeat: no-repeat; width: 200px; height: 100px;"></div>
    <!-- Buttons -->
    <div style="display: flex; justify-content: flex-start;">
    <button id="lightTorch" style="margin-right: 20px; width: 140px; height: 40px; font-size: 14px; color: #FF7F00; font-weight: bold">Light Torch</button>
    <button id="extinguishTorch" style="width: 160px; height: 40px; font-size: 14px; font-weight: bold">Extinguish Torch</button>
    </div>
    
    <!-- Duration Radio Buttons -->
    <h2>Duration:</h2>
    <div class="duration">
        <input type="radio" id="oneHour" name="duration" value="3600">
        <label for="oneHour">One Hour (Standard)</label><br>

        <input type="radio" id="slyHour" name="duration" value="60">
        <label for="slyHour">Sly Hour (One Hour minus 1d12 Minutes)</label><br>
        
        <input type="radio" id="halfHour" name="duration" value="1800">
        <label for="halfHour">Half Hour (Blitz Mode)</label><br>
        
        <input type="radio" id="customDuration" name="duration" value="">
        <label for="customDuration">Custom Time (minutes):</label>
        <input type="number" id="customTime" min="1" max="59">
    </div>
    
    <!-- Timer -->
    <h2>Timer:</h2>
    <p class="timer">00:00</p>
    <script>
        let duration = 0;
        
        // Get the timer element
        const timerElement = document.querySelector('.timer');
        
        // Function to update the timer
        function updateTimer() {
            const minutes = Math.floor((duration / 60) % 60);
            const seconds = Math.floor(duration % 60);
            
            timerElement.textContent = `${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
        }
        
        // Function to count down
        function countDown() {
            duration -= 1;
            updateTimer();
            if (duration > 0) {
                setTimeout(countDown, 1000);
            } else {
                timerElement.textContent = '00:00';
                duration = 0;
            }
        }
        
         // Function to get random duration for "Sly Flourish" radio button
        function getRandomDuration() {
            return (60 - Math.floor(Math.random() * 12 + 1))*60;
        }
        
        // Add event listeners to buttons
       document.getElementById('lightTorch').addEventListener('click', () => {
            if (document.querySelector('input[name="duration"]:checked').id === 'slyHour') {
                duration = getRandomDuration();
            } else {
                duration = parseInt(document.querySelector('input[name="duration"]:checked').value) || document.getElementById('customTime').valueAsNumber * 60;
            }
            countDown();
            fetch('/light-torch?duration=' + duration)
       			.then(response => response.json())
        		.then(data => console.log(data))
        		.catch(error => console.error('Error:', error));
        }); 

        document.getElementById('extinguishTorch').addEventListener('click', () => {
            duration = 0;
            updateTimer();
            fetch('/extinguish-torch')
        		.then(response => response.text())
        		.then(data => console.log(data))
        		.catch(error => console.error('Error:', error));
        });
    </script>
</body>
</html>
  )rawliteral";
  return page;
}