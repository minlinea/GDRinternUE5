// Fill out your copyright notice in the Description page of Project Settings.


#include "RecvThread.h"

RecvThread::RecvThread()
{
	this->m_bRun = false;
	this->m_sSocket = nullptr;
}

RecvThread::RecvThread(FSocket *socket)
{
	this->m_bRun = false;
	this->m_sSocket = socket;
}

RecvThread::~RecvThread()
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

bool RecvThread::Init()
{
	this->m_bRun = true;

	return true;
};

uint32 RecvThread::Run()
{
	Packet pt;
	while (this->m_bRun)
	{
		FMemory::Memzero(pt);
		if (true == this->ClientRecv(&pt, PACKETHEADER))
		{
			if (PACKETHEADER != pt.GetSize())
			{
				ReadAddData(pt);
			}
			else
			{
				SavePacket<Packet>(pt.GetType());
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
				FString::Printf(TEXT("Error RecvThread Run no data Recv")), true, FVector2D{ 2.f, 2.f });

			this->m_bRun = false;
		}
	}
	return 0;
}

void RecvThread::ReadAddData(Packet& packet)
{
	size_t recvsize{ packet.GetSize() - PACKETHEADER };
	uint8* recvdata{ (uint8*)FMemory::Malloc(recvsize) };
	if (true == this->ClientRecv(recvdata, recvsize))
	{
		if (PACKETTYPE::PT_BallPlace == packet.GetType())
		{
			BALLPLACE data;
			FMemory::Memmove(&data, recvdata, sizeof(BALLPLACE));
			SavePacket<PacketBallPlace>(data);
		}
		else if (PACKETTYPE::PT_ShotData == packet.GetType())
		{
			ShotData data;
			FMemory::Memmove(&data, recvdata, sizeof(ShotData));
			SavePacket<PacketShotData>(data);
		}
		else if (PACKETTYPE::PT_ActiveState == packet.GetType())
		{
			bool data;
			FMemory::Memmove(&data, recvdata, sizeof(bool));
			SavePacket<PacketActiveState>(data);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
				FString::Printf(TEXT("ReadAddData unknown type")), true, FVector2D{ 2.f, 2.f });
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("ReadAddData no data Recv Error")), true, FVector2D{ 2.f, 2.f });

		this->m_bRun = false;
	}
	FMemory::Free(recvdata);
}

void RecvThread::Exit()
{
	this->Stop();
}

void RecvThread::Stop()
{
	this->m_bRun = false;
}

bool RecvThread::ClientRecv(void* buf, int size)
{
	return this->m_sSocket->Recv((uint8*)buf, size, size);
}

template <class P, class PACKETDATA>
void RecvThread::SavePacket(PACKETDATA data)
{
	this->m_qPacket.push(new P(data));
}