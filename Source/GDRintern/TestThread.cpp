// Fill out your copyright notice in the Description page of Project Settings.


#include "TestThread.h"

TestThread::TestThread()
{
	this->m_bRun = false;
	this->m_sServerAddress = TEXT("192.168.245.130");
	this->m_iServerPort = 8989;
	this->m_sSocket = nullptr;

}

TestThread::~TestThread()
{

}

bool TestThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		FString::Printf(TEXT("TestThread Init")), true, FVector2D{ 2.f, 2.f });

	if (true == ConnectServer())
	{
		this->m_bRun = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect OK")), true, FVector2D{ 2.f, 2.f });
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("Connect Fail")), true, FVector2D{ 2.f, 2.f });
		return false;
	}
}
uint32 TestThread::Run()
{
	Packet pt;
	while (this->m_bRun)
	{
		FMemory::Memzero(pt);
		uint8* recvdata{ (uint8*)FMemory::Malloc(sizeof(Packet)) };

		if (true == this->ClientRecv(recvdata, PACKETHEADER))
		{
			FMemory::Memmove(&pt, recvdata, sizeof(Packet));

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
				FString::Printf(TEXT("Input Value : packettype : %d, packetsize : %d"),
					(unsigned int)pt.GetType(), (unsigned int)pt.GetSize()), true, FVector2D{ 2.f, 2.f });

			if (PACKETHEADER != pt.GetSize())
			{
				ReadAddData(pt);
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
				FString::Printf(TEXT("Run no data Recv Error")), true, FVector2D{ 2.f, 2.f });

			this->m_bRun = false;
		}
		FMemory::Free(recvdata);
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

bool TestThread::ConnectServer()
{
	this->m_sSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FIPv4Address ip;
	FIPv4Address::Parse(m_sServerAddress, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, m_iServerPort);

	return m_sSocket->Connect(*addr);
}

void TestThread::ReadAddData(Packet& packet)
{
	uint8* recvdata{ (uint8*)FMemory::Malloc(sizeof(Packet)) };
	unsigned int recvsize{ packet.GetSize() - PACKETHEADER };

	if (true == this->ClientRecv(recvdata, recvsize))
	{
		if (PACKETTYPE::PT_BallPlace == packet.GetType())
		{
			BALLPLACE temp;
			FMemory::Memmove(&temp, recvdata, sizeof(BALLPLACE));

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
				FString::Printf(TEXT("BallPlace : %s"), *FString(to_string(temp))), true, FVector2D{ 2.f, 2.f });
			//Server.SetClubSetting(recvdata);

			//Server.SendPacket<Packet>(PACKETTYPE::PT_ClubSettingRecv);
		}
		else if (PACKETTYPE::PT_ShotData == packet.GetType())
		{
			ShotData temp;
			FMemory::Memmove(&temp, recvdata, sizeof(ShotData));

			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
			//	FString::Printf(TEXT("BallPlace : %s"), FString(to_string(temp))), true, FVector2D{ 2.f, 2.f });
			//Server.SetTeeSetting(recvdata);

			//Server.SendPacket<Packet>(PACKETTYPE::PT_TeeSettingRecv);
		}
		else if (PACKETTYPE::PT_ActiveState == packet.GetType())
		{
			bool temp = false;
			FMemory::Memmove(&packet, recvdata, sizeof(bool));

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
				FString::Printf(TEXT("ActiveState : %s"), *FString(to_string(temp))), true, FVector2D{ 2.f, 2.f });
			//Server.SetActiveState(recvdata);
		
			//Server.SendPacket<Packet>(PACKETTYPE::PT_ActiveStateRecv);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
				FString::Printf(TEXT("ReadAddData recv unknwon type")), true, FVector2D{ 2.f, 2.f });
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

bool TestThread::ClientRecv(uint8* buf, int size)
{
	return this->m_sSocket->Recv(buf, size, size);
}