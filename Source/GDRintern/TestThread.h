// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "Packet\packet.h"
#include <queue>
#include "HAL/RunnableThread.h"
/**
 * 
 */
class GDRINTERN_API TestThread : public FRunnable
{
	bool m_bRun;
	FSocket* m_sSocket;


public:
	TestThread();
	TestThread(FSocket* socket);
	~TestThread();

	bool ClientRecv(void* buf, int size);

	void ReadAddData(Packet& pt);

	template <class P, class PACKETDATA>
	void SendPacket(PACKETDATA data);

	std::queue<Packet*> m_qPacket;

	virtual bool Init();
	virtual uint32 Run();
	virtual void Exit();
	virtual void Stop();

};
