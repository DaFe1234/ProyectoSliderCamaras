/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "jonathantech";
const char* password = "elvictoriosovive";

bool ledState = 0;
const int ledPin = 2;

DynamicJsonDocument doc(1024);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>Camera Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>Camera Web Server</h1>
  </div>
  <body>
  <div id="ColumnaRoll">
    <h> Roll </h>
   <input id="slider-pwm-1" data-id="1" type="range" min="-360" max="360" value="0" onchange="sendPWM(this.dataset.id, this.value);" data-upgraded=",MaterialSlider">
  <input id="slider-text-pwm-1" data-id="1" style="width:35px;" onchange="sendPWM(this.dataset.id, this.value);" value="0">
  </div>
  <div id="ColumnaPitch">
    <h> Pitch </h>
   <input id="slider-pwm-2" data-id="2" type="range" min="-360" max="360" value="0" onchange="sendPWM(this.dataset.id, this.value);" data-upgraded=",MaterialSlider">
    <input id="slider-text-pwm-2" data-id="2" style="width:35px;" onchange="sendPWM(this.dataset.id, this.value);" value="0">
  </div>
  <div id="ColumnaDistancia">
    <h> Distancia </h>
   <input id="slider-pwm-3" data-id="3" type="range" min="-200" max="200" value="0" onchange="sendPWM(this.dataset.id, this.value);" data-upgraded=",MaterialSlider">
    <input id="slider-text-pwm-3" data-id="3" style="width:35px;" onchange="sendPWM(this.dataset.id, this.value);" value="0">
  </div>
  <!--<div>
    <button> M1 </button>
    <button> M2 </button>
    <button> M3 </button>
    <button> M4 </button>
  </div>
  <div>
    <button> Save </button>
  </div>-->
  <div>
    <h> Desing by Daniel Romero </h>
  </div>
</body>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket(){
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    websocket.send('Received from Client');
	  console.log('Connected');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event){
    console.log('Received from server: ', event.data);
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
  function sendPWM(id, pwm){
    updateSliderText(id, pwm);
    let data = {
      command:"setPWM",
      id:id,
      pwm:pwm
    }
    let json = JSON.stringify(data);
    console.log(json);
    websocket.send(json);
  }
  function updateSliderText(id, value) {
    document.getElementById('slider-pwm-' + id).value = value; 
    document.getElementById('slider-text-pwm-'+ id).value = value; 
  }
</script>
</body>
</html>
)rawliteral";

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  
  String datoEntrada = (char*)data;
  //Serial.println(datoEntrada);
  deserializeJson(doc, datoEntrada);
  JsonObject obj = doc.as<JsonObject>();

  int id = obj[String("id")];
  int pwm = obj[String("pwm")];

  setPWM(String(id),pwm);
  /*AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
  }*/
}

void notifyClients() {
  ws.textAll("hola mundo");
}


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setPWM(String id, int pwm)
{
   Serial.print("Set PWM ");
   Serial.print(id);
   Serial.print(": ");
   Serial.println(pwm);
   if(id == "1"){
    float valor = 0.0;
    valor = pwm*2.9228/360;
    String comando = "G21G91X"+ String(valor, 2) +"F1";
    Serial.println(comando);
   }
   if(id == "2"){
    float valor = 0.0;
    valor = pwm*8.48/360;
    String comando = "G21G91Y"+ String(valor, 2) +"F1";
    Serial.println(comando);
   }
   if(id == "3"){
    float valor = 0.0;
    valor = pwm *10;
    valor = 0.0225364809 * valor;
    String comando = "G21G91Z"+ String(valor, 2) +"F1";
    Serial.println(comando);
   }
}

void doAction(String actionId)
{
   Serial.print("Doing action: ");
   Serial.println(actionId);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}
