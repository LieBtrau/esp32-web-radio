#include "StreamDB.h"

// File System
#include "SPIFFS.h"

static const char *TAG = "streams";


bool StreamDB::open(const char *dbFile)
{
    ESP_LOGI(TAG, "[DB] open(%s)", dbFile);

    File db = SPIFFS.open(dbFile, "r");
    if (!db)
    {
        ESP_LOGE(TAG, "ERROR: file not found");
        return false;
    }

    // Get the root object in the document
    DeserializationError err = deserializeJson(_doc, db);
    if (err)
    {
        ESP_LOGE(TAG, "%s", err.c_str());
        return false;
    }
    db.close();

    _streams = _doc["streams"];

    for (JsonVariant value : _streams)
    {
        ESP_LOGD(TAG, "%s, %s", value["name"].as<const char *>(), value["url"].as<const char *>());
    }

    return true;
}

bool StreamDB::getName(int index, String& name)
{
    if (index < _streams.size())
    {
        name = _streams[index]["name"].as<String>();
        return true;
    }
    return false;
}

bool StreamDB::getStream(const char* name, String& url)
{
    for (JsonVariant value : _streams)
    {
        if (strcmp(value["name"].as<String>().c_str(), name) == 0)
        {
            url = value["url"].as<String>();
            return true;
        }
    }
    return false;
}
