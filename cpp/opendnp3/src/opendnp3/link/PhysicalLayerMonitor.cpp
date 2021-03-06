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
#include "PhysicalLayerMonitor.h"

#include "PhysicalLayerMonitorStates.h"

#include "opendnp3/LogLevels.h"

#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/LogMacros.h>
#include <openpal/Bind.h>

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

PhysicalLayerMonitor::PhysicalLayerMonitor(
	openpal::LogRoot& root,
    IPhysicalLayerAsync* pPhys_,
    TimeDuration minOpenRetry_,
    TimeDuration maxOpenRetry_,
    IOpenDelayStrategy* pOpenStrategy_) :
	logger(root.GetLogger()),
	pPhys(pPhys_),
	isOnline(false),
	mpOpenTimer(nullptr),
	mpState(MonitorStateInit::Inst()),
	mFinalShutdown(false),
	minOpenRetry(minOpenRetry_),
	maxOpenRetry(maxOpenRetry_),
	currentRetry(minOpenRetry_),
	pOpenStrategy(pOpenStrategy_)
{
	assert(pPhys != nullptr);
	pPhys->SetHandler(this);
}

ChannelState PhysicalLayerMonitor::GetState()
{
	return mpState->GetState();
}

void PhysicalLayerMonitor::ChangeState(IMonitorState* apState)
{
	FORMAT_LOG_BLOCK(logger, flags::DBG, "%s -> %s", mpState->Name(), apState->Name());
	IMonitorState* pLast = mpState;
	mpState = apState;

	if(pLast->GetState() != apState->GetState())
	{
		this->OnStateChange(mpState->GetState());
	}

	if (mpState->GetState() == ChannelState::SHUTDOWN)
	{
		this->OnShutdown();
	}
}


/// ------- IHandlerAsync -------

void PhysicalLayerMonitor::OnOpenFailure()
{
	if (mpState->OnOpenFailure(this))
	{
		this->OnPhysicalLayerOpenFailureCallback();
		this->currentRetry = pOpenStrategy->GetNextDelay(currentRetry, maxOpenRetry);
	}
}

void PhysicalLayerMonitor::OnLowerLayerUp()
{
	if (mpState->OnLayerOpen(this))
	{
		isOnline = true;
		this->currentRetry = minOpenRetry;
		this->OnPhysicalLayerOpenSuccessCallback();
	}
}

void PhysicalLayerMonitor::OnLowerLayerDown()
{
	if (mpState->OnLayerClose(this))
	{
		isOnline = false;
		this->OnPhysicalLayerCloseCallback();
	}
}

/* ------- User facing events that occurs ------- */

void PhysicalLayerMonitor::Start()
{	
	mpState->OnStartRequest(this);
}

void PhysicalLayerMonitor::StartOne()
{
	mpState->OnStartOneRequest(this);
}

void PhysicalLayerMonitor::Close()
{
	mpState->OnCloseRequest(this);
}

void PhysicalLayerMonitor::Suspend()
{	
	mpState->OnSuspendRequest(this);
}

void PhysicalLayerMonitor::Shutdown()
{
	mpState->OnShutdownRequest(this);
}

/* ------- External events that occurs ------- */

void PhysicalLayerMonitor::OnOpenTimerExpiration()
{
	if (mpState->OnOpenTimeout(this))
	{
		assert(mpOpenTimer != nullptr);
		mpOpenTimer = nullptr;
	}
}

/* ------- Actions for the states ------- */

void PhysicalLayerMonitor::StartOpenTimer()
{
	assert(mpOpenTimer == nullptr);
	auto lambda = [this]() { this->OnOpenTimerExpiration(); };
	mpOpenTimer = pPhys->GetExecutor()->Start(currentRetry, Bind(lambda));
}

void PhysicalLayerMonitor::CancelOpenTimer()
{
	assert(mpOpenTimer != nullptr);
	mpOpenTimer->Cancel();
	mpOpenTimer = nullptr;
}

}
