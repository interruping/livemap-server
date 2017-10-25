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

#ifndef stdout_log_utility_h
#define stdout_log_utility_h

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <time.h>

#define SC_STDOUTLOG(x) \
{ \
std::cout.sync_with_stdio(true);\
std::cout << "[" << log_time_in_HH_MM_SS_MMM() << "]" \
<< " : " << "log : " << x << std::endl; \
}

namespace {
    
    
    std::string log_time_in_HH_MM_SS_MMM()
    {
        using namespace std::chrono;
        
        
        auto now = system_clock::now();
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        auto timer = system_clock::to_time_t(now);

        std::tm bt = *std::localtime(&timer);
        
        std::ostringstream oss;
        
        char buffer[80] = {0,};
        strftime(buffer, sizeof(buffer), "%Y/%m/%d-%H:%M:%S", &bt);
        
        oss << buffer;
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        
        return oss.str();
    }
}
#endif /* stdout_log_utility_h */
