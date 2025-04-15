#ifndef CONFIG_H // define these definitions in config.h for your customized project
#define CONFIG_H

#define YOUR_USER = "Repository UserName";           // Your GitHub username
#define YOUR_REPO = "Repository Name";               // The name of your repository
#define GHPAT = "YOUR_GITHUB_PERSONAL_ACCESS_TOKEN"; // Personal Access Token for private repos
#define FIRMWARE_FILE = "firmware.bin";              // The frimware file to download from the release
#define localVersionFile = "/version.txt";           // Where we store our local version
#define rebootOnUpdate                               // reboot after update comment to disable
#endif