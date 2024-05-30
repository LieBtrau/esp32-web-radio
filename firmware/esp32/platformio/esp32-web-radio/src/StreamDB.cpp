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
#include "rom/crc.h"

bool StreamDB::load(const char *dbFilePath)
{
    ESP_LOGI(, "[DB] open(%s)", dbFilePath);

    File dbFile = SPIFFS.open(dbFilePath, "r");
    if (!dbFile)
    {
        ESP_LOGE(, "ERROR: file not found");
        return false;
    }

    // Get the root object in the document
    DeserializationError err = deserializeJson(_doc, dbFile);
    if (err)
    {
        ESP_LOGE(, "%s", err.c_str());
        return false;
    }
    dbFile.close();

    _streams = _doc["streams"];

    for (JsonVariant value : _streams)
    {
        ESP_LOGD(TAG, "%s, %s", value["name"].as<const char *>(), value["url"].as<const char *>());
    }

    return true;
}

bool StreamDB::save(const char *dbFilePath)
{
    ESP_LOGI(TAG, "[DB] save(%s)", dbFilePath);

    File dbFile = SPIFFS.open(dbFilePath, "w");
    if (!dbFile)
    {
        ESP_LOGE(, "ERROR: file not found");
        return false;
    }

    // Serialize JSON to file
    if (serializeJson(_doc, dbFile) == 0)
    {
        ESP_LOGE(, "Failed to write to file");
        return false;
    }
    dbFile.close();
    return true;
}

bool StreamDB::safeSave(const char *dbFile)
{
    String tempFile = String(dbFile) + ".tmp";
    String backupFile = String(dbFile) + ".bak";
    uint32_t crc_file = 0;

    if (!save(tempFile.c_str()))
    {
        ESP_LOGE(, "Error saving temp-file: %s", tempFile.c_str());
        return false;
    }

    uint32_t crc_json = getCrc();
    ESP_LOGI(, "JSON CRC: %08X", crc_json);
    if (!getCrc(tempFile.c_str(), crc_file))
    {
        ESP_LOGE(, "Error getting CRC");
        return false;
    }
    if (crc_json != crc_file)
    {
        ESP_LOGE(, "CRC mismatch");
        return false;
    }

    if (SPIFFS.exists(dbFile))
    {
        if (SPIFFS.exists(backupFile))
        {
            SPIFFS.remove(backupFile);
        }
        if (!SPIFFS.rename(dbFile, backupFile))
        {
            ESP_LOGE(, "Error renaming file %s to %s", dbFile, backupFile.c_str());
            return false;
        }
    }

    if (! SPIFFS.rename(tempFile, dbFile))
    {
        ESP_LOGE(, "Error renaming file %s to %s", tempFile.c_str(), dbFile);
        return false;
    }
    return true;
}

bool StreamDB::getName(int index, String &name) const
{
    if (index < _streams.size())
    {
        name = _streams[index]["name"].as<String>();
        return true;
    }
    return false;
}

bool StreamDB::getStream(const String &name, String &url) const
{
    for (JsonVariant value : _streams)
    {
        if (value["name"].as<String>().compareTo(name) == 0)
        {
            url = value["url"].as<String>();
            return true;
        }
    }
    return false;
}

bool StreamDB::getVolume(const String &name, uint8_t &volume) const
{
    for (JsonVariant value : _streams)
    {
        if (value["name"].as<String>().compareTo(name) == 0)
        {
            volume = value["volume"].as<uint8_t>();
            return true;
        }
    }
    return false;
}

bool StreamDB::setVolumeCurrentChannel(uint8_t volume)
{
    String name;
    if (getCurrentStream(name))
    {
        return setVolume(name, volume);
    }
    return false;
}

bool StreamDB::setVolume(const String &name, uint8_t volume)
{
    for (JsonVariant value : _streams)
    {
        if (value["name"].as<String>().compareTo(name) == 0)
        {
            value["volume"] = volume;
            return true;
        }
    }
    return false;
}

bool StreamDB::getCurrentStream(String &name) const
{
    name = _doc["CurrentStream"].as<String>();
    return name.length() > 0 && name.compareTo("null") != 0;
}

bool StreamDB::getFirstStream(String &name) const
{
    if (_streams.size() > 0)
    {
        name = _streams[0]["name"].as<String>();
        return true;
    }
    return false;
}

void StreamDB::setCurrentStream(const String &name)
{
    for (JsonVariant value : _streams)
    {
        if (value["name"].as<String>().compareTo(name) == 0 && name.compareTo("null") != 0)
        {
            _lastStream = _doc["CurrentStream"].as<String>();
            _doc["CurrentStream"] = name;
            return;
        }
    }
}

void StreamDB::restoreLastStream()
{
    _doc["CurrentStream"] = _lastStream;
}

bool StreamDB::getCrc(const char *dbFile, uint32_t &crc)
{
    crc = 0;
    File file = SPIFFS.open(dbFile, "r");
    if (!file)
    {
        ESP_LOGE(, "ERROR: file not found");
        return false;
    }
    while (file.available())
    {
        char c = file.read();
        crc = crc32_le(crc, (const uint8_t *)&c, 1);
    }
    file.close();
    return crc;
}

uint32_t StreamDB::getCrc()
{
    return crc32_le(0, (const uint8_t *)_doc.as<String>().c_str(), _doc.as<String>().length());
}