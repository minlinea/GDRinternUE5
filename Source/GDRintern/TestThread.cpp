// Fill out your copyright notice in the Description page of Project Settings.


#include "TestThread.h"

TestThread::TestThread()
{
	this->m_bRun = false;
	this->m_sSocket = nullptr;
}


TestThread::TestThread(FSocket* socket)
{
	this->m_bRun = false;
	this->m_sSocket = socket;
}

TestThread::~TestThread()
{

}

bool TestThread::Init()
{
	this->m_bRun = true;

	return true;
}

uint32 TestThread::Run()
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
				SendPacket<Packet>(pt.GetType());
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
				FString::Printf(TEXT("Run no data Recv Error")), true, FVector2D{ 2.f, 2.f });

			this->m_bRun = false;
		}
	}
	return 0;
}
void TestThread::Exit()
{
	this->Stop();
}
void TestThread::Stop()
{
	this->m_bRun = false;
}

template <class P, class PACKETDATA>
void TestThread::SendPacket(PACKETDATA data)
{
	m_qPacket.push(new P(data));
}


void TestThread::ReadAddData(Packet& packet)
{
	size_t recvsize{ packet.GetSize() - PACKETHEADER };
	uint8* recvdata{ (uint8*)FMemory::Malloc(recvsize) };
	if (true == this->ClientRecv(recvdata, recvsize))
	{
		if (PACKETTYPE::PT_BallPlace == packet.GetType())
		{
			BALLPLACE data;
			FMemory::Memmove(&data, recvdata, sizeof(BALLPLACE));
			SendPacket<PacketBallPlace>(data);
		}
		else if (PACKETTYPE::PT_ShotData == packet.GetType())
		{
			ShotData data;
			FMemory::Memmove(&data, recvdata, sizeof(ShotData));
			SendPacket<PacketShotData>(data);
		}
		else if (PACKETTYPE::PT_ActiveState == packet.GetType())
		{
			bool data;
			FMemory::Memmove(&data, recvdata, sizeof(bool));
			SendPacket<PacketActiveState>(data);
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

bool TestThread::ClientRecv(void* buf, int size)
{
	return this->m_sSocket->Recv((uint8*)buf, size, size);
}