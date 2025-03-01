/************************************************************************************
  If not stated otherwise in this file or this component's LICENSE file the
  following copyright and licenses apply:
  
  Copyright 2018 RDK Management
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
  
  http://www.apache.org/licenses/LICENSE-2.0
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 **************************************************************************/

#ifndef WIFI_SSP_MAIN_H
#define WIFI_SSP_MAIN_H

#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    pthread_t   tid;
    int         argc;
    char        **argv;
} wifi_ssp_t;

#ifdef __cplusplus
}
#endif

#endif //WIFI_SSP_MAIN_H
