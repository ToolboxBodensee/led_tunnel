#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

struct OTA_CONFIG {
    const int version;
    const char* check_url;
    const char* binary_url;
    const bool debug;
};

void FirmwareUpdate(OTA_CONFIG config)
{
    //show current version in debug
    if (config.debug)
    {
        char buffer [20];
        sprintf (buffer, "version: %d", config.version);
        Serial.println(buffer);
      }

      //no wifi - can't do
    if (WiFi.status() != WL_CONNECTED)
    {
        if (config.debug)
            Serial.println("no wifi connection - no updates");
        return;
    }
    else
    {
  // Überprüfen der Firmwareversion des programmms aud dem Server
  HTTPClient http;
  int FirmwareNeu = 0;
  http.begin(config.check_url);     // Webseite aufrufen
  int httpCode = http.GET();            // Antwort des Servers einlesen
  if (httpCode == HTTP_CODE_OK)         // Wenn Antwort OK
  {
    String payload = http.getString();  // Webseite einlesen
    FirmwareNeu = payload.toInt();      // Zahl aus Sting bilden
  }
  http.end();

  if (FirmwareNeu > config.version)        // Firmwareversion mit aktueller vergleichen
  {
      if (config.debug)
      {
        Serial.println("Neue Firmware verfuegbar");
        Serial.println("Starte Download");
    }
    ESPhttpUpdate.rebootOnUpdate(false);// reboot abschalten, wir wollen erst Meldungen ausgeben
    t_httpUpdate_return ret = ESPhttpUpdate.update(config.binary_url);
    switch (ret)
    {
      case HTTP_UPDATE_FAILED:
      if (config.debug)
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
      case HTTP_UPDATE_OK:
      if (config.debug)
      {
        Serial.println("Update erfolgreich");
        Serial.println("Reset");
        Serial.flush();
    }
        delay(1);
        ESP.reset();
        delay(100);
        break;
    }
  }
}
}
