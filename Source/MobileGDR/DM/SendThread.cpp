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
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
				//	FString::Printf(TEXT("SendThread Run %s"), *FString(to_string(packet->GetType()))), true, FVector2D{ 2.f, 2.f });
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

	bool result = this->m_sSocket->Send((uint8*)senddata, sendsize, sendsize);

	FMemory::Free(senddata);

	return result;
}