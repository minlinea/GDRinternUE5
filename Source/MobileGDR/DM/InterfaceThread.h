// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "packet.h"
#include <queue>
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

/**
 * �𸮾��� �������� FRunnable�� ��ӹ޴� InterfaceThread
 * FRunnable���� �ʼ��� �����ؾ� �ϴ� �Լ�[Init, Run, Exit, Stop]
 * Send, Recv���� ���ÿ� �ʿ��� ���, ���� ����
 */
class MOBILEGDR_API InterfaceThread : public FRunnable
{


	
public:
	InterfaceThread();
	InterfaceThread(FSocket* socket);
	~InterfaceThread();

	virtual bool Init() = 0;
	virtual uint32 Run() = 0;
	virtual void Exit() = 0;
	virtual void Stop() = 0;

	std::queue<Packet*>& GetQueue()
	{
		return m_qPacket;
	}

	void SetQueue(const std::queue<Packet*>& queuepacket)
	{
		this->m_qPacket = queuepacket;
	}

	bool GetRun()
	{
		return m_bRun;
	}

	void SetRun(const bool& run)
	{
		this->m_bRun = run;
	}

protected:
	bool m_bRun;
	FSocket* m_sSocket;
	std::queue<Packet*> m_qPacket;
};
