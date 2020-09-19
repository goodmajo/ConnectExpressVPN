/*
    ConnectExpressVPN.cpp
    Joel Goodman 2020
*/


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <memory>
#include <vector>


int main()
{
    constexpr const char* vpnConnectCommand    = "expressvpn connect smart";
    constexpr const char* vpnDisconnectCommand = "expressvpn disconnect";
    constexpr const char* notConnectedSubStr   = "Not";	/* This is a substring we check for to tell whether or not vpn is connected. */     
    constexpr const char* statusCommand        = "expressvpn status";
    constexpr size_t subStrLen	               = strlen(notConnectedSubStr);

    /* Credit for the code below that gets command output belongs to gregpaton08 on SO (https://bit.ly/3iMTppU).
     * I've been using this solution for years now and it always works perfectly. Thanks, Greg! */
    std::vector<char> buffer;
    buffer.reserve(subStrLen);

    std::string result;
    result.reserve(subStrLen);

    std::unique_ptr<std::FILE,decltype(&pclose)> pipe(popen(statusCommand, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), subStrLen, pipe.get()) != nullptr)
        result += buffer.data();

    const bool connected = (result.substr(0,subStrLen) == notConnectedSubStr) ? false : true;

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


