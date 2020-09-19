/*
    ConnectExpressVPN.cpp
    Joel Goodman 2020
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <memory>


int main()
{
    const char* fname                = ".vpnstate";
    const char* vpnConnectCommand    = "expressvpn connect smart";
    const char* vpnDisconnectCommand = "expressvpn disconnect";
 
    /* Credit for the code below that gets command output belongs to gregpaton08 on SO (https://bit.ly/3iMTppU).
     * I've been using this solution for years now and it always works perfectly. Thanks, Greg! */
    std::array<char,1023> buffer;
    const char* statusCommand = "expressvpn status";
    std::string result;
    std::unique_ptr<std::FILE,decltype(&pclose)> pipe(popen(statusCommand, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        result += buffer.data();
    const bool connected = (result.substr(0,3) == "Not") ? false : true;

    if(connected)
    {
        std::system("notify-send -i face-cool \"VPN Connector\" \"Disconnecting ExpressVPN...\"");
        std::system(vpnDisconnectCommand);
    }
    else
    {
        std::system("notify-send -i face-cool \"VPN Connector\" \"Connecting ExpressVPN...\"");
        std::system(vpnConnectCommand);
    }

    return EXIT_SUCCESS;
}

