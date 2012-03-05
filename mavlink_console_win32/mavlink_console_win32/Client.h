///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) OMG Plc 2009.
// All rights reserved.  This software is protected by copyright
// law and international treaties.  No part of this software / document
// may be reproduced or distributed in any form or by any means,
// whether transiently or incidentally to some other use of this software,
// without the written permission of the copyright owner.
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef _EXPORTING
	#define CLASS_DECLSPEC    __declspec(dllexport)
#else
	#define CLASS_DECLSPEC    __declspec(dllimport)
#endif

#include <string>

namespace ViconDataStreamSDK
{
namespace CPP
{

 
namespace Direction
{
  enum Enum
  {
    Up,
    Down,
    Left,
    Right,
    Forward,
    Backward
  };
}

namespace StreamMode
{
  enum Enum
  {
    ClientPull,
    ClientPullPreFetch,
    ServerPush
  };
}

namespace TimecodeStandard
{
  enum Enum
  {
    None,
    PAL,
    NTSC,
    NTSCDrop,
    Film
  };
}

namespace DeviceType
{
  enum Enum
  {
    Unknown,
    ForcePlate
  };
}

namespace Unit
{
  enum Enum
  {
    Unknown,
    Volt,
    Newton,
    NewtonMillimetre,
    Millimetre
  };
}

namespace Result
{
  enum Enum
  {
    Unknown,
    NotImplemented,
    Success,
    InvalidHostName,
    InvalidMulticastIP,
    ClientAlreadyConnected,
    ClientConnectionFailed,
    ServerAlreadyTransmittingMulticast,
    ServerNotTransmittingMulticast,
    NotConnected,
    NoFrame,
    InvalidIndex,
    InvalidSubjectName,
    InvalidSegmentName,
    InvalidMarkerName,
    InvalidDeviceName,
    InvalidDeviceOutputName,
    InvalidLatencySampleName,
    CoLinearAxes,
    LeftHandedAxes
  };
}

  class CLASS_DECLSPEC Output_GetVersion
  {
  public:
    unsigned int Major;
    unsigned int Minor;
    unsigned int Point;
  };

  class CLASS_DECLSPEC Output_Connect
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_ConnectToMulticast
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_Disconnect
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_IsConnected
  {
  public:
    bool Connected;
  };

  class CLASS_DECLSPEC Output_StartTransmittingMulticast
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_StopTransmittingMulticast
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_EnableSegmentData
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_EnableMarkerData
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_EnableUnlabeledMarkerData
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_EnableDeviceData
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_IsSegmentDataEnabled
  {
  public:
    bool Enabled;
  };

  class CLASS_DECLSPEC Output_IsMarkerDataEnabled
  {
  public:
    bool Enabled;
  };

  class CLASS_DECLSPEC Output_IsUnlabeledMarkerDataEnabled
  {
  public:
    bool Enabled;
  };

  class CLASS_DECLSPEC Output_IsDeviceDataEnabled
  {
  public:
    bool Enabled;
  };

  class CLASS_DECLSPEC Output_SetStreamMode
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_SetAxisMapping
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_GetAxisMapping
  {
  public:
    Direction::Enum XAxis;
    Direction::Enum YAxis;
    Direction::Enum ZAxis;
  };

  class CLASS_DECLSPEC Output_GetFrame
  {
  public:
    Result::Enum Result;
  };

  class CLASS_DECLSPEC Output_GetFrameNumber
  {
  public:
    Result::Enum Result;
    unsigned int FrameNumber;
  };

  class CLASS_DECLSPEC Output_GetTimecode
  {
  public:
    Result::Enum           Result;
    unsigned int           Hours;
    unsigned int           Minutes;
    unsigned int           Seconds;
    unsigned int           Frames;
    unsigned int           SubFrame;
    bool                   FieldFlag;
    TimecodeStandard::Enum Standard;
    unsigned int           SubFramesPerFrame;
    unsigned int           UserBits;
  };

  class CLASS_DECLSPEC Output_GetLatencySampleCount
  {
  public:
    Result::Enum Result;
    unsigned int Count;
  };

  class CLASS_DECLSPEC Output_GetLatencySampleName
  {
  public:
    Result::Enum Result;
    std::string  Name;
  };

  class CLASS_DECLSPEC Output_GetLatencySampleValue
  {
  public:
    Result::Enum Result;
    double       Value;
  };

  class CLASS_DECLSPEC Output_GetLatencyTotal
  {
  public:
    Result::Enum Result;
    double       Total;
  };

  class CLASS_DECLSPEC Output_GetSubjectCount
  {
  public:
    Result::Enum Result;
    unsigned int SubjectCount;
  };

  class CLASS_DECLSPEC Output_GetSubjectName
  {
  public:
    Result::Enum Result;
    std::string  SubjectName;
  };

  class CLASS_DECLSPEC Output_GetSegmentCount
  {
  public:
    Result::Enum Result;
    unsigned int SegmentCount;
  };

  class CLASS_DECLSPEC Output_GetSegmentName
  {
  public:
    Result::Enum Result;
    std::string  SegmentName;
  };

  class CLASS_DECLSPEC Output_GetSegmentGlobalTranslation
  {
  public:
    Result::Enum Result;
    double       Translation[ 3 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentGlobalRotationHelical
  {
  public:
    Result::Enum Result;
    double       Rotation[ 3 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentGlobalRotationMatrix
  {
  public:
    Result::Enum Result;
    double       Rotation[ 9 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentGlobalRotationQuaternion
  {
  public:
    Result::Enum Result;
    double       Rotation[ 4 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentGlobalRotationEulerXYZ
  {
  public:
    Result::Enum Result;
    double       Rotation[ 3 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentLocalTranslation
  {
  public:
    Result::Enum Result;
    double       Translation[ 3 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentLocalRotationHelical
  {
  public:
    Result::Enum Result;
    double       Rotation[ 3 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentLocalRotationMatrix
  {
  public:
    Result::Enum Result;
    double       Rotation[ 9 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentLocalRotationQuaternion
  {
  public:
    Result::Enum Result;
    double       Rotation[ 4 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetSegmentLocalRotationEulerXYZ
  {
  public:
    Result::Enum Result;
    double       Rotation[ 3 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetMarkerCount
  {
  public:
    Result::Enum Result;
    unsigned int MarkerCount;
  };

  class CLASS_DECLSPEC Output_GetMarkerName
  {
  public:
    Result::Enum Result;
    std::string  MarkerName;
  };

  class CLASS_DECLSPEC Output_GetMarkerGlobalTranslation
  {
  public:
    Result::Enum Result;
    double       Translation[ 3 ];
    bool         Occluded;
  };

  class CLASS_DECLSPEC Output_GetUnlabeledMarkerCount
  {
  public:
    Result::Enum Result;
    unsigned int MarkerCount;
  };

  class CLASS_DECLSPEC Output_GetUnlabeledMarkerGlobalTranslation
  {
  public:
    Result::Enum Result;
    double       Translation[ 3 ];
  };

  class CLASS_DECLSPEC Output_GetDeviceCount
  {
  public:
    Result::Enum Result;
    unsigned int DeviceCount;
  };

  class CLASS_DECLSPEC Output_GetDeviceName
  {
  public:
    Result::Enum     Result;
    std::string      DeviceName;
    DeviceType::Enum DeviceType;
  };

  class CLASS_DECLSPEC Output_GetDeviceOutputCount
  {
  public:
    Result::Enum Result;
    unsigned int DeviceOutputCount;
  };

  class CLASS_DECLSPEC Output_GetDeviceOutputName
  {
  public:
    Result::Enum Result;
    std::string  DeviceOutputName;
    Unit::Enum   DeviceOutputUnit;
  };

  class CLASS_DECLSPEC Output_GetDeviceOutputValue
  {
  public:
    Result::Enum Result;
    double       Value;
    bool         Occluded;
  };


  class ClientImpl;

  class CLASS_DECLSPEC Client
  {
  public:
    Client();
    ~Client();

    Output_GetVersion  GetVersion() const;

    Output_Connect     Connect( const std::string & HostName );
    Output_ConnectToMulticast ConnectToMulticast( const std::string & HostName, const std::string & MulticastIP );
    Output_Disconnect  Disconnect();
    Output_IsConnected IsConnected() const;
    Output_StartTransmittingMulticast StartTransmittingMulticast( const std::string & ServerIP,
                                                                  const std::string & MulticastIP );

    Output_StopTransmittingMulticast StopTransmittingMulticast();

    Output_EnableSegmentData         EnableSegmentData();
    Output_EnableMarkerData          EnableMarkerData();
    Output_EnableUnlabeledMarkerData EnableUnlabeledMarkerData();
    Output_EnableDeviceData          EnableDeviceData();

    Output_IsSegmentDataEnabled         IsSegmentDataEnabled() const;
    Output_IsMarkerDataEnabled          IsMarkerDataEnabled() const;
    Output_IsUnlabeledMarkerDataEnabled IsUnlabeledMarkerDataEnabled() const;
    Output_IsDeviceDataEnabled          IsDeviceDataEnabled() const;
    
    Output_SetStreamMode SetStreamMode( const StreamMode::Enum Mode );

    Output_SetAxisMapping SetAxisMapping( const Direction::Enum XAxis, const Direction::Enum YAxis, const Direction::Enum ZAxis );
    Output_GetAxisMapping GetAxisMapping() const;

    Output_GetFrame GetFrame();
    Output_GetFrameNumber GetFrameNumber() const;

    Output_GetTimecode GetTimecode() const;

    Output_GetLatencySampleCount GetLatencySampleCount() const;
    Output_GetLatencySampleName  GetLatencySampleName( const unsigned int LatencySampleIndex ) const;
    Output_GetLatencySampleValue GetLatencySampleValue( const std::string & LatencySampleName ) const;
    Output_GetLatencyTotal       GetLatencyTotal() const;

    Output_GetSubjectCount GetSubjectCount() const;
    Output_GetSubjectName GetSubjectName( const unsigned int SubjectIndex ) const;

    Output_GetSegmentCount GetSegmentCount( const std::string  & SubjectName ) const;
    
    Output_GetSegmentName GetSegmentName( const std::string  & SubjectName,
                                          const unsigned int   SegmentIndex ) const;

    Output_GetSegmentGlobalTranslation GetSegmentGlobalTranslation( const std::string & SubjectName,
                                                                    const std::string & SegmentName ) const;

    Output_GetSegmentGlobalRotationHelical GetSegmentGlobalRotationHelical( const std::string & SubjectName,
                                                                            const std::string & SegmentName ) const;

    Output_GetSegmentGlobalRotationMatrix GetSegmentGlobalRotationMatrix( const std::string & SubjectName,
                                                                          const std::string & SegmentName ) const;

    Output_GetSegmentGlobalRotationQuaternion GetSegmentGlobalRotationQuaternion( const std::string & SubjectName,
                                                                                  const std::string & SegmentName ) const;

    Output_GetSegmentGlobalRotationEulerXYZ GetSegmentGlobalRotationEulerXYZ( const std::string & SubjectName,
                                                                              const std::string & SegmentName ) const;

    Output_GetSegmentLocalTranslation GetSegmentLocalTranslation( const std::string & SubjectName,
                                                                  const std::string & SegmentName ) const;

    Output_GetSegmentLocalRotationHelical GetSegmentLocalRotationHelical( const std::string & SubjectName,
                                                                          const std::string & SegmentName ) const;

    Output_GetSegmentLocalRotationMatrix GetSegmentLocalRotationMatrix( const std::string & SubjectName,
                                                                        const std::string & SegmentName ) const;

    Output_GetSegmentLocalRotationQuaternion GetSegmentLocalRotationQuaternion( const std::string & SubjectName,
                                                                                const std::string & SegmentName ) const;

    Output_GetSegmentLocalRotationEulerXYZ GetSegmentLocalRotationEulerXYZ( const std::string & SubjectName,
                                                                            const std::string & SegmentName ) const;

    Output_GetMarkerCount GetMarkerCount( const std::string  & SubjectName ) const;

    Output_GetMarkerName GetMarkerName( const std::string & SubjectName,
                                        const unsigned int  MarkerIndex ) const;

    Output_GetMarkerGlobalTranslation GetMarkerGlobalTranslation( const std::string & SubjectName,
                                                                  const std::string & MarkerName ) const;


    Output_GetUnlabeledMarkerCount GetUnlabeledMarkerCount() const;
    
    Output_GetUnlabeledMarkerGlobalTranslation GetUnlabeledMarkerGlobalTranslation( const unsigned int MarkerIndex ) const;

    Output_GetDeviceCount GetDeviceCount() const;
    Output_GetDeviceName  GetDeviceName( const unsigned int DeviceIndex ) const;

    Output_GetDeviceOutputCount GetDeviceOutputCount( const std::string  & DeviceName ) const;

    Output_GetDeviceOutputName GetDeviceOutputName( const std::string  & DeviceName,
                                                    const unsigned int   DeviceOutputIndex ) const;
    
    Output_GetDeviceOutputValue GetDeviceOutputValue( const std::string & DeviceName,
                                                      const std::string & DeviceOutputName ) const;
  private:
    ClientImpl * m_pClientImpl;
  };
} // End of namespace CPP
} // End of namespace ViconDataStreamSDK
