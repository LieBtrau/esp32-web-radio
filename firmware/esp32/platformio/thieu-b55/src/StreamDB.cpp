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

bool StreamDB::getStream(const int index, String& name, String& url)
{
    if (index >= _streams.size())
    {
        return false;
    }
    name = _streams[index]["name"].as<const char *>();
    url = _streams[index]["url"].as<const char *>();
    return true;
}
