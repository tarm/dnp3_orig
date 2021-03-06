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
#ifndef __I_APP_LAYER_H_
#define __I_APP_LAYER_H_

#include "opendnp3/app/APDURequest.h"
#include "opendnp3/app/APDUResponse.h"

namespace opendnp3
{

/** Interface for the async app layer.

	For both transaction models defined below, the client is responsible
	for setting the header field of the  APDU, WITH THE EXCEPTION OF THE
	SEQUENCE #.
*/
class IAppLayer
{
public:

	/** Start a response transaction with optional confirmation.

		The incrementing of the sequence number is controlled
		by the FIR bit. If FIR is not set, the sequence is incremented.

		A callback to one of the following is guaranteed:

		OnFailure() - The transaction failed, either because the send failed or
		the confirmation timed out.

		OnLowerLayerDown() - The transaction failed because the lower layer went down.

		OnSendSuccess() - The data was transmitted succesfully and valid confirmation
		was received within the timeout (if requested)
	*/
	virtual void SendResponse(APDUResponse&) = 0;

	/** Start an unsolicited transaction with optional confirmation and retries. This
		sequence is almost identical to a response transaction.

		FIR and FIN must both be set for unsolcited responses since they cannot
		be multifragmented.

		A callback to one of the following is gauranteed:

		OnFailure() - The transaction failed, either because the send failed or
		the confirmation timed out.

		OnLowerLayerDown() - The transaction failed because the lower layer went down.

		OnSendSucces() - The data was transmitted succesfully and valid confirmation
		was received within the timeout (if requested)
	*/
	virtual void SendUnsolicited(APDUResponse&) = 0;

	/** Start a send transaction with optional confirmation and retries,
		that should result in a resposne.

		Callbacks are as follows:

		OnFailure() - The transaction failed, either because the send failed or
		the confirmation timed out.

		OnLowerLayerDown() - The transaction failed because the lower layer went down.

		OnPartialResponse(APDU) - A non-FIN response was received. The client should
		process the response and continue waiting for:

				- More partial responses
				- A failure
				- A final response

		OnFinalResponse(APDU) - A FIN response was received. The transaction is complete.
	*/
	virtual void SendRequest(APDURequest&) = 0;

	/**
		Cancel a running response transaction. The outstation must still wait for an OnFailure()
		from the application layer before proceeding with another response.

		This is necessary to implement the behavior decribed in in DNP3Spec-V2-Part1-ApplicationLayer:

		4.2.1 - ReadRules - Rule 1
	*/
	virtual void CancelResponse() = 0;
};

} //end ns

#endif
