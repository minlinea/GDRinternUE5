// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Packet\packet.h"
#include "C:\Program Files\Epic Games\UE_5.0EA\Engine\Source\Runtime\Networking\Public\Interfaces\IPv4\IPv4Address.h"
#include "C:\Program Files\Epic Games\UE_5.0EA\Engine\Source\Runtime\Networking\Public\Interfaces\IPv4\IPv4SubnetMask.h"
#include "HAL/RunnableThread.h"
/**
 * 
 */
class GDRINTERN_API TestThread : public FRunnable
{
	bool m_bRun;
	FSocket* m_sSocket;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString m_sServerAddress;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 m_iServerPort;

public:
	TestThread();
	~TestThread();

	bool ConnectServer();
	void ClientRecv();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Exit();
	virtual void Stop();

};
