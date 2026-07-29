/*
=================================================
  IoT ENERGY MONITOR
  ESP32-S3 + PZEM-004T + MQTT HiveMQ Cloud

  Sensor :
  PZEM-004T Open CT

  MQTT Topic :
  rumah/energi/data
=================================================
*/


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>


// ================================
// WIFI CONFIGURATION
// ================================

const char* ssid = "Nihao";
const char* password = "yogayoga1919!";


// ================================
// MQTT HIVE MQ CONFIGURATION
// ================================

const char* mqtt_server = "9b902896ff004582a696e5037b584b4f.s1.eu.hivemq.cloud";

const int mqtt_port = 8883;

const char* mqtt_username = "yogailmi19";

const char* mqtt_password = "Yogayoga1919!@";


// MQTT Topic

const char* mqtt_topic = "rumah/energi/data";


// ================================
// PZEM CONFIGURATION
// ================================

HardwareSerial pzemSerial(2);


// RX ESP32 = GPIO18
// TX ESP32 = GPIO17

PZEM004Tv30 pzem(
  pzemSerial,
  18,
  17
);


// ================================
// MQTT OBJECT
// ================================

WiFiClientSecure secureClient;

PubSubClient mqttClient(secureClient);


// ================================
// CONNECT WIFI
// ================================

void konekWiFi(){


  if(WiFi.status() == WL_CONNECTED){
    return;
  }


  Serial.print("Menghubungkan WiFi");


  WiFi.begin(
    ssid,
    password
  );


  while(WiFi.status()!=WL_CONNECTED){

    delay(500);
    Serial.print(".");

  }


  Serial.println();

  Serial.println("WiFi berhasil terhubung");

  Serial.print("IP Address : ");

  Serial.println(
    WiFi.localIP()
  );


}



// ================================
// CONNECT MQTT
// ================================

void konekMQTT(){


  while(!mqttClient.connected()){


    Serial.println(
      "Menghubungkan MQTT..."
    );


    String clientID = "ESP32_ENERGY_";

    clientID += String(
      random(0xffff),
      HEX
    );


    if(
      mqttClient.connect(
        clientID.c_str(),
        mqtt_username,
        mqtt_password
      )
    ){


      Serial.println(
        "MQTT berhasil terhubung"
      );


    }
    else{


      Serial.print(
        "MQTT gagal kode : "
      );


      Serial.println(
        mqttClient.state()
      );


      delay(3000);


    }

  }

}



// ================================
// SETUP
// ================================

void setup(){


  Serial.begin(115200);


  delay(1000);


  Serial.println();

  Serial.println(
    "================================"
  );

  Serial.println(
    " MONITOR ENERGI IoT"
  );

  Serial.println(
    " ESP32-S3 + PZEM-004T"
  );

  Serial.println(
    "================================"
  );



  // UART PZEM

  pzemSerial.begin(
    9600,
    SERIAL_8N1,
    18,
    17
  );


  konekWiFi();



  // TLS HiveMQ

  secureClient.setInsecure();



  mqttClient.setServer(
    mqtt_server,
    mqtt_port
  );


}



// ================================
// LOOP
// ================================

void loop(){



  konekWiFi();


  if(!mqttClient.connected()){

    konekMQTT();

  }


  mqttClient.loop();



  // ================================
  // BACA PZEM
  // ================================


  float tegangan = pzem.voltage();

  float arus = pzem.current();

  float daya = pzem.power();

  float energi = pzem.energy();

  float frekuensi = pzem.frequency();

  float pf = pzem.pf();



  String status;



  // Sensor gagal membaca

  if(isnan(tegangan)){


    tegangan = 0;
    arus = 0;
    daya = 0;
    frekuensi = 0;
    pf = 0;


    status = "Sensor Error";


    Serial.println();

    Serial.println(
      "STATUS : SENSOR ERROR"
    );


  }



  // Tidak ada beban

  else if(arus < 0.01){


    arus = 0;
    daya = 0;


    status = "Tidak Ada Beban";


  }



  // Ada beban

  else{


    status = "Beban Aktif";


  }



  // ================================
  // SERIAL MONITOR
  // ================================


  Serial.println();

  Serial.println(
    "DATA LISTRIK"
  );

  Serial.println(
    "----------------------------"
  );


  Serial.print("Status       : ");
  Serial.println(status);


  Serial.print("Tegangan     : ");
  Serial.print(tegangan,2);
  Serial.println(" V");


  Serial.print("Arus         : ");
  Serial.print(arus,2);
  Serial.println(" A");


  Serial.print("Daya         : ");
  Serial.print(daya,2);
  Serial.println(" W");


  Serial.print("Energi       : ");
  Serial.print(energi,3);
  Serial.println(" kWh");


  Serial.print("Frekuensi    : ");
  Serial.print(frekuensi,2);
  Serial.println(" Hz");


  Serial.print("Faktor Daya  : ");
  Serial.println(pf,2);



  // ================================
  // JSON MQTT
  // ================================


  StaticJsonDocument<300> json;


  json["status"] = status;

  json["tegangan"] = tegangan;

  json["arus"] = arus;

  json["daya"] = daya;

  json["energi"] = energi;

  json["frekuensi"] = frekuensi;

  json["pf"] = pf;



  char buffer[300];


  serializeJson(
    json,
    buffer
  );



  // retained message TRUE

  mqttClient.publish(
    mqtt_topic,
    buffer,
    true
  );



  Serial.println();

  Serial.println(
    "Data MQTT terkirim:"
  );


  Serial.println(buffer);


  Serial.println(
    "============================"
  );



  delay(5000);


}
