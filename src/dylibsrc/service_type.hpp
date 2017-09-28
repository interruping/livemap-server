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

//
//  service_type.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 26..
//

#ifndef service_type_h
#define service_type_h

enum service_type {
    DEFAULT = 0,
    USERNODEUPDATE = 1,
    REQUESTUSERINFO = 2,
    SETUSERINFO = 3,
    NEARNODEINFO = 4,
    USERVIEWPOINTUPDATE = 5,
    UTF8MESSAGESEND = 6
    
};


#endif /* service_type_h */
