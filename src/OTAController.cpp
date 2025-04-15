// Copyright (C) 2025 AYS Abdelrahman Yasser Samir
// OTA Library
// @date 15.04.2025
// This library is designed to update the firmware of ESP32/ESP8266 devices using GitHub releases.
#include "OTAController.h"

GitHubUpdater::GitHubUpdater(const String &repoOwner, const String &repoName, const String &token, const String &file)
    : _repoOwner(repoOwner),
      _repoName(repoName),
      _token(token),
      _fileToDownload(file)
{
}

void GitHubUpdater::init()
{
    if (!WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected");
        Serial.println("Connect to WiFi and try again.");
        return;
    }
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS mount failed");
        return;
    }
}

String GitHubUpdater::getLocalVersion()
{
    if (!SPIFFS.exists("/version.txt"))
        return "0.0.0";
    File file = SPIFFS.open("/version.txt", FILE_READ);
    String ver = file.readStringUntil('\n');
    file.close();
    return ver;
}

void GitHubUpdater::saveLocalVersion(const String &version)
{
    File file = SPIFFS.open("/version.txt", FILE_WRITE);
    file.print(version);
    file.close();
}

bool GitHubUpdater::getLatestRelease(String &version, String &downloadUrl)
{
    String apiReleaseUrl = "https://api.github.com/repos/" + _repoOwner + "/" + _repoName + "/releases/latest";

    HTTPClient http;
    http.begin(apiReleaseUrl);
    http.addHeader("User-Agent", "ESPClient");
#ifdef GithubPAT
    http.addHeader("Authorization", "Bearer " + String(_token)); // Needed for private repos
#endif
    http.setTimeout(10000); // 10 seconds timeout

    int code = http.GET();
    if (code != 200)
    {
        Serial.printf("GitHub API failed: %d\n", code);
        http.end();
        return false;
    }

    DynamicJsonDocument doc(64 * 1024);
    deserializeJson(doc, http.getString());

    version = doc["tag_name"].as<String>();
    JsonArray assets = doc["assets"];
    for (JsonObject asset : assets)
    {
        if (asset["name"] == _fileToDownload)
        {
            downloadUrl = asset["browser_download_url"].as<String>();
            http.end();
            return true;
        }
    }

    Serial.println("Asset not found");
    http.end();
    return false;
}

bool GitHubUpdater::downloadAndUpdate(const String &url)
{
    HTTPClient http;
    http.begin(url);
    http.addHeader("User-Agent", "ESPClient");
#ifdef GithubPAT
    http.addHeader("Authorization", "Bearer " + String(_token)); // Needed for private repos
#endif
    http.setTimeout(10000); // 10 seconds timeout

    int code = http.GET();
    if (code != 200)
    {
        Serial.printf("Failed to download: %d\n", code);
        http.end();
        return false;
    }

    int contentLength = http.getSize();
    WiFiClient *stream = http.getStreamPtr();

    if (!Update.begin(contentLength))
    {
        Serial.println("Update initialization failed");
        return false;
    }

    size_t written = Update.writeStream(*stream);
    if (written != contentLength)
    {
        Serial.println("Incomplete write");
        return false;
    }

    if (!Update.end())
    {
        Serial.println("Update finalization failed");
        return false;
    }

    if (Update.isFinished())
    {
        Serial.println("Firmware update complete.");
        delay(500);
#ifdef rebootOnUpdate
        Serial.println("Rebooting...");
        ESP.restart();
#endif
        return true;
    }
    else
    {
        Serial.println("Update not finished correctly");
        return false;
    }
    return false;
}

void GitHubUpdater::checkForUpdate()
{
    String latestVer, downloadUrl;
    if (!getLatestRelease(latestVer, downloadUrl))
        return;

    String localVer = getLocalVersion();
    Serial.printf("Local: %s | Remote: %s\n", localVer.c_str(), latestVer.c_str());

    if (localVer != latestVer)
    {
        Serial.println("New update available!");
        if (downloadAndUpdate(downloadUrl))
        {
            saveLocalVersion(latestVer);
        }
    }
    else
    {
        Serial.println("No update needed.");
    }
}
