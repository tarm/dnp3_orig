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
#include "MockAppLayer.h"

#include <openpal/LogMacros.h>
#include <openpal/ToHex.h>

#include <opendnp3/LogLevels.h>

#include "HexConversions.h"
#include "Exception.h"

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

MockAppLayer::MockAppLayer(const openpal::Logger& logger_) :
	logger(logger_),
	mNumCancel(0),
	mpUser(nullptr),
	mAutoSendCallback(true),
	mIsSuccess(true)
{

}

void MockAppLayer::SetUser(IAppUser* apUser)
{
	mpUser = apUser;
}

void MockAppLayer::EnableAutoSendCallback(bool aIsSuccess)
{
	mAutoSendCallback = true;
	mIsSuccess = aIsSuccess;
}

void MockAppLayer::DisableAutoSendCallback()
{
	mAutoSendCallback = false;
}

void MockAppLayer::DoSendUnsol()
{
	if(mAutoSendCallback)
	{
		assert(mpUser != nullptr);
		if(mIsSuccess) mpUser->OnUnsolSendSuccess();
		else mpUser->OnUnsolFailure();
	}
}

void MockAppLayer::DoSendSol()
{
	if(mAutoSendCallback)
	{
		assert(mpUser != nullptr);
		if(mIsSuccess) mpUser->OnSolSendSuccess();
		else mpUser->OnSolFailure();
	}
}

void MockAppLayer::SendResponse(APDUResponse& apdu)
{	
	mFragments.push_back(toHex(apdu.ToReadOnly()));
	this->DoSendSol();

}

void MockAppLayer::SendUnsolicited(APDUResponse& apdu)
{
	mFragments.push_back(toHex(apdu.ToReadOnly()));
	this->DoSendUnsol();
}

void MockAppLayer::SendRequest(APDURequest& apdu)
{	
	mFragments.push_back(toHex(apdu.ToReadOnly()));
}

bool MockAppLayer::NothingToRead()
{
	return mFragments.size() == 0;
}

std::string MockAppLayer::Read()
{
	if(mFragments.size() == 0) throw InvalidStateException(LOCATION, "no more fragments");
	auto frag = mFragments.front();
	mFragments.pop_front();
	return frag;
}

void MockAppLayer::CancelResponse()
{
	++mNumCancel;
}

}

