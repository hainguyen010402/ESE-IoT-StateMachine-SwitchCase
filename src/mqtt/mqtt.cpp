#include "mqtt.h"

#include "../config/config.h"

#include "../config/state.h"

#include "../sensor_handler/sensor_handler.h"

#include "../gpio/gpio.h"

#include "../freeRTOS_tasks/freeRTOS_tasks.h"

#include "../SD_card/SD_card.h"

#include "../get_time/get_time.h"



//PubSubClient client;
PubSubClient mqttClient;

// mqtt server
String mqtt_server;

char TOPIC_TELEMETRY_INFO[TOPIC_MAX_LEN];
char TOPIC_TELEMETRY_MEASUREMENT[TOPIC_MAX_LEN];
char TOPIC_TELEMETRY_RELAYSTATUS[TOPIC_MAX_LEN];
char TOPIC_CMD_RELAYSET[TOPIC_MAX_LEN];
const char* TOPIC_LWT = "device/ESP32_001/LWT";


JsonDocument doc;



void protocol_cmdGPIOWrite(GpioConfig &config, bool state)
{
    Serial.println("CMD: GPIO WRITE");
    digitalWrite(config.gpio, !state);


    

    /*

    JsonDocument response;

    response["id"] = request["id"];
    response["result"] = "ok";

    JsonObject data = response["data"].to<JsonObject>();
    data["state"] = gpio_get(config.gpio);

    mqtt_publish(TOPIC_STT1, response);

    */
}

// hàm build mqtt topics
void mqtt_buildTopics(const char* device_id)
{
    snprintf(TOPIC_TELEMETRY_INFO,
             sizeof(TOPIC_TELEMETRY_INFO),
             "device/%s/info", device_id);

    snprintf(TOPIC_TELEMETRY_MEASUREMENT,
             sizeof(TOPIC_TELEMETRY_MEASUREMENT),
             "device/%s/measurement", device_id);

    snprintf(TOPIC_TELEMETRY_RELAYSTATUS,
             sizeof(TOPIC_TELEMETRY_RELAYSTATUS),
             "device/%s/relay/status", device_id);

    snprintf(TOPIC_CMD_RELAYSET,
             sizeof(TOPIC_CMD_RELAYSET),
             "device/%s/relay/set", device_id);

    //snprintf(TOPIC_LWT,
    //         sizeof(TOPIC_LWT),
    //         "device/%s/LWT", device_id);
}




// ---------------- Helper: ghi CSV có mutex ----------------
static void writeCsvLocked(const char* path, const char* line, const char* header)
{
    if (xSemaphoreTake(spi_mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        sdSPI_write_csv(path, line, header);
        xSemaphoreGive(spi_mutex);
    }
    else
    {
        Serial.printf("[SD] mutex timeout: %s\n", path);
    }
}
// ---------------- Helper: serialize + publish 1 topic ----------------
static bool publishObject(const char* topic, JsonVariantConst obj)
{

        char buf[256];
        size_t len = serializeJson(obj, buf, sizeof(buf));

        if (len >= sizeof(buf))   // serializeJson cắt âm thầm nếu buffer nhỏ
        {
            Serial.printf("[MQTT FAILED] payload too large (%u) : %s\n", (unsigned)len, topic);
            return false;
        }


    
        // code
        bool ok = mqttClient.connected() && mqttClient.publish(topic, buf);
        

        Serial.print(ok ? "[MQTT SUCCEED] " : "[MQTT FAILED] ");
        Serial.print("Publish ");
        Serial.print(topic);
        Serial.print(" : ");
        Serial.println(buf);

        return ok;

}
// ---------------- Hàm gộp: build JSON -> (ghi SD) -> MQTT publish ----------------
bool buildJsonPayload_then_mqttPublish(JsonDocument& doc, uint8_t mask)
{
    doc.clear();

    bool ok = true;
    char csv[256];
    String ts = get_time_yyyymmdd_hhmmss();   // lấy 1 lần, 3 object cùng timestamp

    // ---------- INFO ----------
    if (mask & TELE_INFO)
    {
        JsonObject info = doc["info"].to<JsonObject>();
        info["device_id"] = DEVICE_ID;
        info["name"]      = NAME;
        info["location"]  = LOCATION;
        info["online"]    = MQTT_STATUS_FLAG;

        snprintf(csv, sizeof(csv), "%s,%s,%s,%s,%d",
                 ts.c_str(), DEVICE_ID, NAME, LOCATION, MQTT_STATUS_FLAG);
        writeCsvLocked("/data/info.csv", csv,
                       "Time,Device ID,Name,Location,Online Status");

        ok &= publishObject(TOPIC_TELEMETRY_INFO, info);
    }

    // ---------- MEASUREMENT ----------
    if (mask & TELE_MEASUREMENT)
    {
        JsonObject m = doc["measurement"].to<JsonObject>();
        m["temperature"] = temp;
        m["humidity"]    = hum;

        snprintf(csv, sizeof(csv), "%s,%.2f,%.2f", ts.c_str(), temp, hum);
        writeCsvLocked("/data/measurement.csv", csv,
                       "Time,Temperature,Humidity");

        ok &= publishObject(TOPIC_TELEMETRY_MEASUREMENT, m);
    }

    // ---------- RELAY ----------
    if (mask & TELE_RELAY)
    {
        JsonObject r = doc["relay"].to<JsonObject>();
        r["relay_1"] = !esp32_gpio_config[0].state;
        r["relay_2"] = !esp32_gpio_config[1].state;

        snprintf(csv, sizeof(csv), "%s,%d,%d", ts.c_str(),
                 !esp32_gpio_config[0].state,
                 !esp32_gpio_config[1].state);
        writeCsvLocked("/data/relay.csv", csv,
                       "Time,Relay 1 Status,Relay 2 Status");

        ok &= publishObject(TOPIC_TELEMETRY_RELAYSTATUS, r);
    }

    // ---------- WATCHDOG ----------
    if (mask & TELE_WATCHDOG)
    {
        JsonObject info = doc["info"].to<JsonObject>();
        info["device_id"] = DEVICE_ID;
        info["name"]      = NAME;
        info["location"]  = LOCATION;
        info["online"]    = "WATCHDOG TRIGGERED";

        snprintf(csv, sizeof(csv), "%s,%s,%s,%s,%s",
                 ts.c_str(), DEVICE_ID, NAME, LOCATION, "WATCHDOG TRIGGERED");
        writeCsvLocked("/data/info.csv", csv,
                       "Time,Device ID,Name,Location,Online Status");

        ok &= publishObject(TOPIC_TELEMETRY_INFO, info);
    }

    return ok;
}




void JsonPayload_buildTelemetry(JsonDocument& doc)
{
    doc.clear();
    

    /*

     //---------------- Device ----------------
    JsonObject device = doc["device"].to<JsonObject>();

    device["id"] = DEVICE_ID1;
    device[""] = FW_VERSION;
    device["uptime"] = millis();

     //---------------- Sensor ----------------
    JsonObject sensor = doc["sensor"].to<JsonObject>();

    sensor["temp1"] = temp1;
    sensor["temp2"] = temp2;

    //---------------- GPIO ----------------
    JsonObject gpio = doc["gpio"].to<JsonObject>();

    gpio["gpio1"] = gpio_get(esp32_gpio_config[0].gpio);
    gpio["gpio2"] = gpio_get(esp32_gpio_config[1].gpio);

    */

    // ---------------- INFO ----------------
    JsonObject info = doc["info"].to<JsonObject>();

    info["device_id"] = DEVICE_ID;
    info["name"]       = NAME;
    info["location"]   = LOCATION;
    info["online"]     = MQTT_STATUS_FLAG;

    char info_buffer[256];

    snprintf(
        info_buffer,
        sizeof(info_buffer),
        "%s,%s,%s,%s,%d",
        get_time_yyyymmdd_hhmmss().c_str(),
        DEVICE_ID,
        NAME,
        LOCATION,
        MQTT_STATUS_FLAG
    );


    

    

    
    if (xSemaphoreTake(spi_mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        sdSPI_write_csv("/data/info.csv", info_buffer, "Time,Device ID,Name,Location,Online Status");
        xSemaphoreGive(spi_mutex);
    }

    

    

    // ---------------- MEASUREMENT ----------------
    JsonObject measurement = doc["measurement"].to<JsonObject>();

    measurement["temperature"] = temp;
    measurement["humidity"]    = hum;

    char measurement_buffer[256];

    snprintf(
        measurement_buffer,
        sizeof(measurement_buffer),
        "%s,%f,%f",
        get_time_yyyymmdd_hhmmss().c_str(),
        temp,
        hum
    );

    

    
    if (xSemaphoreTake(spi_mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        sdSPI_write_csv("/data/measurement.csv", measurement_buffer, "Time,Temperature,Humidity");
        xSemaphoreGive(spi_mutex);
    }

    // ---------------- RELAY ----------------
    JsonObject relay = doc["relay"].to<JsonObject>();

    relay["relay_1"] = !esp32_gpio_config[0].state;
    relay["relay_2"] = !esp32_gpio_config[1].state;

    char relay_buffer[256];

    snprintf(
        relay_buffer,
        sizeof(relay_buffer),
        "%s,%d,%d",
        get_time_yyyymmdd_hhmmss().c_str(),
        !esp32_gpio_config[0].state,
        !esp32_gpio_config[1].state
    );

    

    
    if (xSemaphoreTake(spi_mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        sdSPI_write_csv("/data/relay.csv", relay_buffer, "Time,Relay 1 Status,Relay 2 Status");
        xSemaphoreGive(spi_mutex);
    }

}

bool mqttPublish(JsonDocument& doc)
{

        char buffer_topicInfo[256];

        serializeJson(doc["info"], buffer_topicInfo);

    
        // code

        bool result_topicInfo = mqttClient.publish(TOPIC_TELEMETRY_INFO, buffer_topicInfo);

        if (result_topicInfo == 0 || mqttClient.connected() == 0)
        {
        Serial.print("[MQTT FAILED] 1");
        }

        if (result_topicInfo == 1 && mqttClient.connected() == 1)
        {
        Serial.print("[MQTT SUCCEED] ");
        }
        

        Serial.print("Publish ");
        Serial.print(TOPIC_TELEMETRY_INFO);
        Serial.print(" : ");
        Serial.println(buffer_topicInfo);









        char buffer_topicMeasurement[256];

        serializeJson(doc["measurement"], buffer_topicMeasurement);

        bool result_topicMeasurement = mqttClient.publish(TOPIC_TELEMETRY_MEASUREMENT, buffer_topicMeasurement);

        if (result_topicMeasurement == 0 || mqttClient.connected() == 0)
        {
        Serial.print("[MQTT FAILED] 2");
        }

        if (result_topicMeasurement == 1 && mqttClient.connected() == 1)
        {
        Serial.print("[MQTT SUCCEED] ");
        }

        Serial.print("Publish ");
        Serial.print(TOPIC_TELEMETRY_MEASUREMENT);
        Serial.print(" : ");
        Serial.println(buffer_topicMeasurement);










        char buffer_topicRelay[256];

        serializeJson(doc["relay"], buffer_topicRelay);

        bool result_topicRelay = mqttClient.publish(TOPIC_TELEMETRY_RELAYSTATUS, buffer_topicRelay);

        if (result_topicRelay == 0 || mqttClient.connected() == 0)
        {
        Serial.print("[MQTT FAILED] 3");
        }

        if (result_topicRelay == 1 && mqttClient.connected() == 1)
        {
        Serial.print("[MQTT SUCCEED] ");
        }

        Serial.print("Publish ");
        Serial.print(TOPIC_TELEMETRY_RELAYSTATUS);
        Serial.print(" : ");
        Serial.println(buffer_topicRelay);


        bool result = result_topicInfo && result_topicMeasurement && result_topicRelay;



        return result;

}





void protocol_processCommand(JsonDocument& doc)
{
    String request_cmd = doc["request_cmd"];

    bool state = doc["status"];

    Serial.println("protocol_processCommand");

    if(request_cmd == CMD_GPIO1_WRITE)
    {
        protocol_cmdGPIOWrite(esp32_gpio_config[0], state);

        Serial.println("request_cmd == CMD_GPIO1_WRITE");

        vTaskDelay(pdMS_TO_TICKS(200));

        doc.clear();

        // data phản hồi lại cmd
        JsonObject response_message = doc["response_message"].to<JsonObject>();

        response_message["relay_1"] = !esp32_gpio_config[0].state;
        response_message["relay_2"] = !esp32_gpio_config[1].state;
        response_message["request_cmd"] = request_cmd;

        publishObject(TOPIC_TELEMETRY_RELAYSTATUS, response_message);



    }
        

    else if(request_cmd == CMD_GPIO2_WRITE)
    {
        protocol_cmdGPIOWrite(esp32_gpio_config[1], state);
        Serial.println("request_cmd == CMD_GPIO2_WRITE");

        vTaskDelay(pdMS_TO_TICKS(200));

        doc.clear();

        // data phản hồi lại cmd
        JsonObject response_message = doc["response_message"].to<JsonObject>();
        
        response_message["relay_1"] = !esp32_gpio_config[0].state;
        response_message["relay_2"] = !esp32_gpio_config[1].state;
        response_message["request_cmd"] = request_cmd;

        publishObject(TOPIC_TELEMETRY_RELAYSTATUS, response_message);
    }    
}


void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    JsonDocument doc;

    DeserializationError err = deserializeJson(doc, payload, length);

    if (err)
    {
        Serial.print("JSON Error: ");
        Serial.println(err.c_str());
        return;
    }


    if (strcmp(topic, TOPIC_CMD_RELAYSET) == 0)
    {
        protocol_processCommand(doc);
        Serial.println("strcmp(topic, TOPIC_CMD_RELAYSET) == 0");
    }
}

void mqtt_connect()
{
    Serial.println("MQTT CONNECTING...");

    if (mqttClient.connect(DEVICE_ID, MQTT_ID, MQTT_PW, TOPIC_LWT, 1, true, "offline"))
    {
    
        MQTT_IP_CONFIGURATED_FIRST_TIME = false;
        MQTT_IP_CONFIGURATED = true;
        MQTT_IP_ENTER_SCREEN_FLAG = false;
        MQTT_CONNECTING_FLAG = false;
        MQTT_STATUS_FLAG = true;
        
        Serial.println("MQTT Connected");

        mqtt_buildTopics(DEVICE_ID);

        if (!mqttClient.subscribe(TOPIC_TELEMETRY_INFO))
        {
            Serial.println("Subcribe to topic failed! Retrying...");
            MQTT_SUBSCRIBE_SUCCESS = false;
            return;
        }

        MQTT_SUBSCRIBE_SUCCESS = true;

        MQTT_FIRST_TIME_CONNECTED = true;

        PROCESSING_STATE = TELEMETRY;

        Serial.print("Subscribe: ");
        Serial.println(TOPIC_TELEMETRY_INFO);
        mqttClient.subscribe(TOPIC_TELEMETRY_MEASUREMENT);
        Serial.print("Subscribe: ");
        Serial.println(TOPIC_TELEMETRY_MEASUREMENT);
        mqttClient.subscribe(TOPIC_TELEMETRY_RELAYSTATUS);
        Serial.print("Subscribe: ");
        Serial.println(TOPIC_TELEMETRY_RELAYSTATUS);
        mqttClient.subscribe(TOPIC_CMD_RELAYSET);
        Serial.print("Subscribe: ");
        Serial.println(TOPIC_CMD_RELAYSET);
    }

    else
    {
        if (MQTT_IP_CONFIGURATED_FIRST_TIME)
        {
            MQTT_SUBSCRIBE_SUCCESS = false;
            MQTT_IP_CONFIGURATED = false;
            MQTT_IP_ENTER_SCREEN_FLAG = true;
            MQTT_CONNECTING_FLAG = false;
            MQTT_STATUS_FLAG = false;
            
            Serial.print("MQTT Failed: ");
            Serial.println(mqttClient.state());

            PROCESSING_STATE = MQTT;
        }
        else
        {
            Serial.print("MQTT Reconnect Failed, retrying...");
        }
    }

}
