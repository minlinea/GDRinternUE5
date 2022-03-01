// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GameInstanceData.h"
#include "InterfaceThread.h"
/**
 * 
 */
class MOBILEGDR_API SendThread : public InterfaceThread
{
public:
	static SendThread& Instance()
	{
		static SendThread Instance;
		return Instance;
	}

	SendThread();
	SendThread(FSocket* socket);
	~SendThread();

	bool ClientSend(Packet* packet);

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

private:

};
