/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include "IINField.h"

#include <openpal/ToHex.h>

#include <sstream>

using namespace openpal;
using namespace std;

namespace opendnp3
{

bool IINField::operator==(const IINField& aRHS) const
{
	return (LSB == aRHS.LSB) && (MSB == aRHS.MSB);
}

#define MACRO_IIN_TO_STRING(field) if(Get##field()) oss << " "#field;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string IINField::ToString() const
{
	ostringstream oss;
	oss << " IIN: (LSB: " << ByteToHex(LSB);

	MACRO_IIN_TO_STRING(AllStations)
	MACRO_IIN_TO_STRING(Class1Events)
	MACRO_IIN_TO_STRING(Class2Events)
	MACRO_IIN_TO_STRING(Class3Events)
	MACRO_IIN_TO_STRING(NeedTime)
	MACRO_IIN_TO_STRING(LocalControl)
	MACRO_IIN_TO_STRING(DeviceTrouble)
	MACRO_IIN_TO_STRING(DeviceRestart)

	oss << ") (MSB: " << ByteToHex(MSB);

	MACRO_IIN_TO_STRING(FuncNotSupported)
	MACRO_IIN_TO_STRING(ObjectUnknown)
	MACRO_IIN_TO_STRING(ParameterError)
	MACRO_IIN_TO_STRING(EventBufferOverflow)
	MACRO_IIN_TO_STRING(AlreadyExecuting)
	MACRO_IIN_TO_STRING(ConfigurationCorrupt)
	MACRO_IIN_TO_STRING(Reserved1)
	MACRO_IIN_TO_STRING(Reserved2)
	oss << ")";

	return oss.str();
}
#endif

}

