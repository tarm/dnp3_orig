/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#include "DataObserverAdapter.hpp"

#include <iostream>


using namespace opendnp3;

DataObserverAdapter::DataObserverAdapter(JavaVM* apJVM, jobject aProxy) :
	mpJVM(apJVM),
	mProxy(aProxy)
{
	JNIEnv* pEnv = this->GetEnv();
	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != nullptr);

	mStartId = pEnv->GetMethodID(clazz, "start", "()V");
	assert(mStartId != nullptr);
	mEndId = pEnv->GetMethodID(clazz, "end", "()V");
	assert(mEndId != nullptr);

	mUpdateBinaryInput = pEnv->GetMethodID(clazz, "updateBI", "(ZBJJ)V");
	assert(mUpdateBinaryInput != nullptr);

	mUpdateAnalogInput = pEnv->GetMethodID(clazz, "updateAI", "(DBJJ)V");
	assert(mUpdateAnalogInput != nullptr);

	mUpdateCounter = pEnv->GetMethodID(clazz, "updateC", "(JBJJ)V");
	assert(mUpdateCounter != nullptr);

	mUpdateBinaryOutputStatus = pEnv->GetMethodID(clazz, "updateBOS", "(ZBJJ)V");
	assert(mUpdateBinaryOutputStatus != nullptr);

	mUpdateAnalogOutputStatus = pEnv->GetMethodID(clazz, "updateAOS", "(DBJJ)V");
	assert(mUpdateAnalogOutputStatus != nullptr);
}

JNIEnv* DataObserverAdapter::GetEnv()
{
	JNIEnv* pEnv = nullptr;
	mpJVM->GetEnv((void**) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != nullptr);
	return pEnv;
}

void DataObserverAdapter::_Start()
{
	GetEnv()->CallVoidMethod(mProxy, mStartId);
}

void DataObserverAdapter::_Update(const Binary& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();

	jboolean value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();
	jlong index = aIndex;

	pEnv->CallVoidMethod(mProxy, mUpdateBinaryInput, value, quality, timestamp, index);
}

void DataObserverAdapter::_Update(const Analog& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();

	jdouble value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();
	jlong index = aIndex;

	pEnv->CallVoidMethod(mProxy, mUpdateAnalogInput, value, quality, timestamp, index);
}

void DataObserverAdapter::_Update(const Counter& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();

	jlong value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();
	jlong index = aIndex;

	pEnv->CallVoidMethod(mProxy, mUpdateCounter, value, quality, timestamp, index);
}

void DataObserverAdapter::_Update(const AnalogOutputStatus& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();

	jdouble value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();
	jlong index = aIndex;

	pEnv->CallVoidMethod(mProxy, mUpdateAnalogOutputStatus, value, quality, timestamp, index);
}

void DataObserverAdapter::_Update(const BinaryOutputStatus& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();

	jboolean value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();
	jlong index = aIndex;

	pEnv->CallVoidMethod(mProxy, mUpdateBinaryOutputStatus, value, quality, timestamp, index);
}

void DataObserverAdapter::_End()
{
	GetEnv()->CallVoidMethod(mProxy, mEndId);
}
