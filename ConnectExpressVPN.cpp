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

    bool connected;
    std::ifstream stateFileIn;
    stateFileIn.open(fname);
    
    if(!stateFileIn.is_open())
    {
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
    	connected = (result.substr(0,3) == "Not") ? false : true;
    }
    else
    {
    	stateFileIn >> connected;
    	stateFileIn.close();
    }

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

    std::ofstream stateFileOut;
    stateFileOut.open(fname, std::ios::trunc);
    const char* newState = connected ? "0" : "1";
    stateFileOut << newState;
    stateFileOut.close();

    return EXIT_SUCCESS;
}

