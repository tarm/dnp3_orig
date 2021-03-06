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
#include "MockPhysicalLayerAsync.h"

#include "BufferHelpers.h"
#include <openpal/IExecutor.h>

#include <memory.h>
#include <functional>

using namespace openpal;

namespace opendnp3
{

MockPhysicalLayerAsync::MockPhysicalLayerAsync(openpal::LogRoot& root, openpal::IExecutor* apExecutor) :
	PhysicalLayerAsyncBase(root),
	mpWriteBuff(nullptr),
	mNumToRead(0),
	mNumToWrite(0),
	mNumWrites(0),
	mNumOpen(0),
	mNumOpenSuccess(0),
	mNumOpenFailure(0),
	mNumClose(0),
	mNumOpeningClose(0),

	mIsAutoOpenSuccess(true),
	mpExecutor(apExecutor)
{

}

void MockPhysicalLayerAsync::DoOpeningClose()
{
	++mNumOpeningClose;
}

void MockPhysicalLayerAsync::DoOpen()
{
	++mNumOpen;
}

void MockPhysicalLayerAsync::DoClose()
{
	++mNumClose;
}

void MockPhysicalLayerAsync::SignalOpenSuccess()
{
	std::error_code ec;
	this->OnOpenCallback(ec);
}

void MockPhysicalLayerAsync::SignalOpenFailure()
{
	std::error_code ec(1, std::generic_category());
	this->OnOpenCallback(ec);
}

void MockPhysicalLayerAsync::SignalSendSuccess()
{
	uint32_t num = mNumToWrite;
	mNumToWrite = 0;
	std::error_code ec;
	this->OnWriteCallback(ec, num);
}

void MockPhysicalLayerAsync::SignalSendFailure()
{
	mNumToWrite = 0;
	std::error_code ec(1, std::generic_category());
	this->OnWriteCallback(ec, 0);
}

void MockPhysicalLayerAsync::SignalReadFailure()
{
	mNumToRead = 0;
	std::error_code ec(1, std::generic_category());
	this->OnReadCallback(ec, mpWriteBuff, 0);
}

void MockPhysicalLayerAsync::TriggerRead(const std::string& arData)
{
	HexSequence hs(arData);
	assert(hs.Size() <= this->mNumToRead);
	memcpy(mpWriteBuff, hs, hs.Size());
	mNumToRead = 0;
	std::error_code ec;
	this->OnReadCallback(ec, mpWriteBuff, hs.Size());
}

void MockPhysicalLayerAsync::TriggerClose()
{
	std::error_code ec(1, std::generic_category());
	this->OnReadCallback(ec, mpWriteBuff, 0);
}


}
