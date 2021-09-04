AsyncWebServer server(80);

void InitServer()
{
	server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

	server.onNotFound([](AsyncWebServerRequest *request) {
		request->send(400, "text/plain", "Not found");
	});

	server.begin();
  Serial.println("HTTP server started");
  Serial.println("[GC:G0 G54 G17 G21 G90 G94 M5 M9 T0 F0 S0]");
}
