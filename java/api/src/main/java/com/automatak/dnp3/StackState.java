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
* Enumeration for possible communication states of a stack
*/
public enum StackState
{
  /**
  * communications are online
  */
  COMMS_UP(0),
  /**
  * communication difficulties
  */
  COMMS_DOWN(1),
  /**
  * unknown state
  */
  UNKNOWN(2);

  private final int id;

  private StackState(int id)
  {
    this.id = id;
  }

  public int toType()
  {
    return id;
  }

  public static StackState fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return COMMS_UP;
      case(1):
        return COMMS_DOWN;
      case(2):
        return UNKNOWN;
    }
    return UNKNOWN;
  }
}
