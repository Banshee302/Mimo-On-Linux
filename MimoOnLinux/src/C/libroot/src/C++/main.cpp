#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "headers/libroot.h"

// Base64 encoding/decoding
static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64_encode(const std::string &in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

static std::string base64_decode(const std::string &in) {
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

static std::string getMaskedInput() {
    std::string input;
    char ch;
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n') {
        input += ch;
        std::cout << '*';
        std::cout.flush();
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
    return input;
}

static const std::string keyPath = "Keys/root.key";

static bool keyExists() {
    std::ifstream file(keyPath);
    return file.good();
}

static void saveEncodedPassword(const std::string& pass) {
    std::ofstream file(keyPath);
    file << base64_encode(pass);
    file.close();
}

static std::string loadDecodedPassword() {
    std::ifstream file(keyPath);
    std::string encoded;
    std::getline(file, encoded);
    file.close();
    return base64_decode(encoded);
}

static bool verifyPassword(const std::string& stored) {
    std::cout << "[libroot] Enter Password: ";
    std::string input = getMaskedInput();
    return input == stored;
}

// Public API exposed to C
extern "C" {

void init_libroot() {
    if (!keyExists()) {
        std::cout << "It seems like this is your first time using libroot.\n";
        std::cout << "[libroot] Enter Password: ";
        std::string stored = getMaskedInput();
        saveEncodedPassword(stored);
        std::cout << "[libroot] Password set successfully.\n";
    } else {
        std::cout << "[libroot] Password already set. Use reset logic if needed.\n";
    }
}

int verify_libroot_password() {
    if (!keyExists()) {
        std::cerr << "[libroot] ERROR: No password set. Use 'mkpasswd' to initialize.\n";
        return 0;
    }
    std::string stored = loadDecodedPassword();
    if (!verifyPassword(stored)) {
        std::cerr << "[libroot] ERROR: PASSWORD INCORRECT\n";
        return 0;
    }
    saveEncodedPassword(stored); // Re-encode after success
    return 1;
}

int run_as_root(const char* appName) {
    if (!verify_libroot_password()) return 1;

    const char* home = getenv("HOME");
    if (!home) {
        std::cerr << "[libroot] ERROR: Could not determine home directory.\n";
        return 1;
    }

    std::string appPath = std::string(home) + "/MimoOnLinux/src/C/Apps/" + std::string(appName);
    if (access(appPath.c_str(), X_OK) != 0) {
        std::cerr << "[libroot] ERROR: Application not found or not executable: " << appPath << "\n";
        return 1;
    }

    std::cout << "[libroot] Executing root application: " << appName << "\n";
    return system(appPath.c_str());
}

}

