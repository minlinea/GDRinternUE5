// Fill out your copyright notice in the Description page of Project Settings.


#include "SendThread.h"

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
	if (0 != this->m_qPacket.size())
	{
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
}

bool SendThread::Init()
{
	this->m_bRun = true;

	return true;
};

uint32 SendThread::Run()
{
	Packet pt;
	while (this->m_bRun)
	{
		if (true != this->m_qPacket.empty())
		{
			auto queuepacket = this->m_qPacket;
			for (auto packet = queuepacket.front(); true != queuepacket.empty(); )
			{
				packet = queuepacket.front();

				if (false == ClientSend(packet))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
						FString::Printf(TEXT("Error SendThread ClientSend")), true, FVector2D{ 2.f, 2.f });

					this->m_bRun = false;
					break;
				}
				delete packet;
				queuepacket.pop();
			}
			this->m_qPacket = queuepacket;
		}
	}
	return 0;
}
void SendThread::Exit()
{
	this->Stop();
}

void SendThread::Stop()
{
	this->m_bRun = false;
}

bool SendThread::ClientSend(Packet* packet)
{
	int sendsize = packet->GetSize();
	uint8* senddata = nullptr;

	senddata = static_cast<uint8*>(FMemory::Malloc(sendsize));

	FMemory::Memmove(senddata, packet, sendsize);

	return this->m_sSocket->Send((uint8*)senddata, sendsize, sendsize);
}