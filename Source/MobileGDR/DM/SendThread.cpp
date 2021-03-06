// Fill out your copyright notice in the Description page of Project Settings.


#include "SendThread.h"
#include "GameFramework/Actor.h"

SendThread::SendThread()
{
	this->m_bRun = false;
	this->m_sSocket = nullptr;
}

SendThread::SendThread(FSocket* socket)
{
	this->m_bRun = false;
	this->m_sSocket = socket;
}

SendThread::~SendThread()
{
	Exit();
}

bool SendThread::Init()
{
	this->m_bRun = true;

	return true;
};

void SendThread::Exit()
{
	this->m_bRun = false;
	if (true != this->m_qPacket.empty())
	{
		for (auto p = this->m_qPacket.front(); true != this->m_qPacket.empty(); )
		{
			p = this->m_qPacket.front();
			delete p;
			this->m_qPacket.pop();
		}
	}
}

uint32 SendThread::Run()
{
	while (true == this->m_bRun)
	{
		if (false == this->m_qPacket.empty())
		{
			for (auto packet = this->m_qPacket.front(); false == this->m_qPacket.empty(); )
			{
				packet = this->m_qPacket.front();
				if (false == ClientSend(packet))
				{
					break;
				}
				delete packet;
				this->m_qPacket.pop();
			}
		}
	}
	return 0;
}

void SendThread::Stop()
{
	Exit();
}

bool SendThread::ClientSend(Packet* packet)
{
	int sendsize = packet->GetSize();
	uint8* senddata = nullptr;

	senddata = static_cast<uint8*>(FMemory::Malloc(sendsize));

	FMemory::Memmove(senddata, packet, sendsize);

	//Send함수인자((uint8*)buf:보낼 데이터가 담긴 버퍼, size:버퍼의 최대 사이즈, size:버퍼에 보낼 것으로 기대되는 사이즈)
	bool result = this->m_sSocket->Send((uint8*)senddata, sendsize, sendsize);

	FMemory::Free(senddata);

	return result;
}