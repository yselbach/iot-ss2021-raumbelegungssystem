# iot-ss2021-raumbelegungssystem

Repository für das Modul »Internet of Things« im Sommersemester 2021 im Studiengang Medieninformatik an der TH Köln.

Node.js, Docker, git und VS Code mit PlatformIO vorausgesetzt

1. Repository clonen (mit der Konsole git clone)
2. in den Ordner esp32-iot-raumbelegungssystem/ wechseln und in VS Code öffnen
3. Libarys (Grove - LCD RGB Backlight by Seeed Studio & PubSubClient) installieren
4. in iot-raumbelegungssystem/src/ eine credentials.h mit folgendem Inhalt erstellen:
```
 const char *ssid = "Network Name";
 const char *pass = "Network Password";
 const char *mqttserver = "MQTT Broker";
 ```
"Network Name" , "Network Password" und "MQTT Broker" durch eigene Daten ersetzen.

5. Docker starten und  Node-RED mit Docker installieren.
```docker run -it -p 1880:1880 -v node_red_data:/data --name mynodered nodered/node-red```

6. Node-Red starten & im Browser localhost:1880 aufrufen. In NodeRed dann noch den Flow https://github.com/yselbach/iot-ss2021-raumbelegungssystem/blob/main/raumbelegungssystem_dashboard.json importieren.
Um auf das Dashboard zu kommen localhost:1880/ui im Browser aufrufen
