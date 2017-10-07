// Copyright 2017 GeunYoung Lim <interruping4dev@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
            << " : " << "debug msg : " << x << " < function : " << __func__ << " > " << std::endl; \
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
