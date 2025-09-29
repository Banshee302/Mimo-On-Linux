#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "libroot.h"


int main() {
    std::string input;
    std::string username = getenv("USER") ? getenv("USER") : "User";

    std::cout << "[UserService] Welcome, " << username << ". Type 'stop UserService' to exit.\n";

    while (true) {
        std::cout << "[" << username << "@Mimo] # ";
        std::getline(std::cin, input);

        if (input == "stop UserService") {
            std::cout << "[UserService] Session ended.\n";
            break;
        } else if (input == "version") {
            std::cout << "Mimo Container Version CLI v1.0\n";
        } else if (input == "arch") {
            std::cout << "Current architecture: ARM\n";
        } else if (input == "security") {
            std::cout << "libroot v1.0, check Docs for more info.\n";
        } else if (input == "libroot") {
            std::cout << "[libroot] ERROR: No application name provided.\n";
        } else if (input.rfind("libroot ", 0) == 0) {
            std::string arg = input.substr(8);
            if (arg == "mkpasswd") {
                init_libroot();
            } else {
                int result = run_as_root(arg.c_str());
                if (result != 0) {
                    std::cout << "[libroot] Application failed with code " << result << "\n";
                }
            }
        } else {
            std::cout << "Unknown command: " << input << "\n";
        }
    }

    return 0;
}

