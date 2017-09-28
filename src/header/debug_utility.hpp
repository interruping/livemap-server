#ifndef SOLARCODE_DEBUG_UTILITY
#define SOLARCODE_DEBUG_UTILITY

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <time.h>

#define SC_DBGMSG(x) \
        { \
            std::cout << "[" << time_in_HH_MM_SS_MMM() << "]" \
            << " : " << "notification : " << x << " in " << __func__ << std::endl; \
        }    
            
namespace {
std::string time_in_HH_MM_SS_MMM()
{
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;
    
    char buffer[80] = {0,};
    strftime(buffer, sizeof(buffer), "%Y/%m/%d-%H:%M:%S", &bt);
    
    oss << buffer; // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}
}
#endif
