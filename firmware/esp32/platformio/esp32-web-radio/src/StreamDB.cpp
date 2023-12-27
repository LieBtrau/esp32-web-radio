/**
 * @file StreamDB.cpp
 * @author Christoph Tack (you@domain.com)
 * @brief Manages user configurable streams
 *  Each stream has its own volume setting because some streams are louder than others.
 * @version 0.1
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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

bool StreamDB::save(const char *dbFile)
{
    ESP_LOGI(TAG, "[DB] save(%s)", dbFile);

    File db = SPIFFS.open(dbFile, "w");
    if (!db)
    {
        ESP_LOGE(TAG, "ERROR: file not found");
        return false;
    }

    // Serialize JSON to file
    if (serializeJson(_doc, db) == 0)
    {
        ESP_LOGE(TAG, "Failed to write to file");
        return false;
    }
    db.close();

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

bool StreamDB::getVolume(const char* name, uint8_t& volume)
{
    for (JsonVariant value : _streams)
    {
        if (strcmp(value["name"].as<String>().c_str(), name) == 0)
        {
            volume = value["volume"].as<uint8_t>();
            return true;
        }
    }
    return false;
}

bool StreamDB::setVolume(const char* name, uint8_t volume)
{
    for (JsonVariant value : _streams)
    {
        if (strcmp(value["name"].as<String>().c_str(), name) == 0)
        {
            value["volume"] = volume;
            return true;
        }
    }
    return false;
}

