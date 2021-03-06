//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef __OPENDNP3_GENERATED_EVENTANALOGOUTPUTSTATUSRESPONSE_H_
#define __OPENDNP3_GENERATED_EVENTANALOGOUTPUTSTATUSRESPONSE_H_

#include <cstdint>

namespace opendnp3 {

enum class EventAnalogOutputStatusResponse : int
{
  Group42Var1 = 0,
  Group42Var2 = 1,
  Group42Var3 = 2,
  Group42Var4 = 3,
  Group42Var5 = 4,
  Group42Var6 = 5,
  Group42Var7 = 6,
  Group42Var8 = 7
};

int EventAnalogOutputStatusResponseToType(EventAnalogOutputStatusResponse arg);
EventAnalogOutputStatusResponse EventAnalogOutputStatusResponseFromType(int arg);

}

#endif
