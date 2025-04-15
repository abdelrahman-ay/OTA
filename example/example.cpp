// Copyright (C) 2025 AYS Abdelrahman Yasser Samir
// OTA Library example
// @date 15.04.2025
// This library is designed to update the firmware of ESP32/ESP8266 devices using GitHub releases.
#include "OTAController.h"

String githubUser = YOUR_USER;
String githubRepo = YOUR_REPO;
String githubPat = GHPAT;
String fileName = FIRMWARE_FILE;
GitHubUpdater updater(githubUser, githubRepo, githubPat, fileName);

void setup()
{
    Serial.begin(115200);
    updater.init();
    updater.checkForUpdate();
}

void loop() {}
