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
package com.automatak.dnp3;

/**
* Opendnp3 API enum used for differentiating cases when a command sequence fails without a response from the outstation
*/
public enum CommandResult
{
  /**
  * A response was received from the outstation, check the CommandStatus enumeration
  */
  RESPONSE_OK(0),
  /**
  * The operation timed out without a response
  */
  TIMEOUT(1),
  /**
  * There is no communication with the outstation, and the command was not attempted
  */
  NO_COMMS(2);

  private final int id;

  private CommandResult(int id)
  {
    this.id = id;
  }

  public int toType()
  {
    return id;
  }

  public static CommandResult fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return RESPONSE_OK;
      case(1):
        return TIMEOUT;
      case(2):
        return NO_COMMS;
    }
    return NO_COMMS;
  }
}