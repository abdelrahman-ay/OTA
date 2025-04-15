// Copyright (C) 2025 -AYS- Abdelrahman Yasser Samir. All rights reserved.
// OTA Library
// @date 15.04.2025
// This library is designed to update the firmware of ESP32/ESP8266 devices using GitHub releases.
#pragma once

#ifdef ESP32
#include <WiFi.h>        // Handles WiFi connection
#include <HTTPClient.h>  // HTTP requests
#include <ArduinoJson.h> // JSON parsing from GitHub API
#include <Update.h>      // OTA update functionality
#include "SPIFFS.h"      // Filesystem for temporary file storage
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>
#include <Updater.h>
#include <LittleFS.h>
#endif

#ifndef CONFIG_H                                     // define these definitions in config.h for your customized project
#define YOUR_USER = "Repository UserName";           // Your GitHub username
#define YOUR_REPO = "Repository Name";               // The name of your repository
#define GHPAT = "YOUR_GITHUB_PERSONAL_ACCESS_TOKEN"; // Personal Access Token for private repos
#define FIRMWARE_FILE = "firmware.bin";              // The frimware file to download from the release
#define localVersionFile = "/version.txt";           // Where we store our local version
#define rebootOnUpdate                               // reboot after update
#endif

class GitHubUpdater
{
public:
    GitHubUpdater(const String &repoOwner, const String &repoName, const String &token, const String &file);
    void init();
    void checkForUpdate();

private:
    String _repoOwner,
        _repoName,
        _token,
        _fileToDownload;
    String getLocalVersion();
    void saveLocalVersion(const String &version);
    bool getLatestRelease(String &version, String &url);
    bool downloadAndUpdate(const String &url);
};
