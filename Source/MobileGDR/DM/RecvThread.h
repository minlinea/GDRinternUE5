// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GameInstanceData.h"
#include "InterfaceThread.h"
/**
 * 
 */
class MOBILEGDR_API RecvThread : public InterfaceThread
{
public:
	RecvThread();
	RecvThread(FSocket* socket);
	~RecvThread();

	void ReadAddData(Packet& packet);
	bool ClientRecv(void* buf, int size);

	template <class P, class PACKETDATA>
	void SavePacket(PACKETDATA data);

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

private:

};