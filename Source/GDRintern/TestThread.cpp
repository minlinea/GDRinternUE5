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

		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("TestThread Init Fail")), true, FVector2D{ 2.f, 2.f });
		return false;
	}
}
uint32 TestThread::Run()
{
	while (this->m_bRun)
	{
		this->ClientRecv();
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
	//this->m_sSocket->Close();
}

bool TestThread::ConnectServer()
{
	this->m_sSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FIPv4Address ip;
	FIPv4Address::Parse(m_sServerAddress, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, m_iServerPort);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	return m_sSocket->Connect(*addr);
}

void TestThread::ClientRecv()
{
	Packet pt;
	uint8* recvdata{ (uint8*)FMemory::Malloc(sizeof(Packet)) };
	int32 Read{ sizeof(Packet) };

	if (true == this->m_sSocket->Recv(recvdata, sizeof(Packet), Read))
	{
		FMemory::Memmove(&pt, recvdata, sizeof(Packet));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("Input Value : packettype : %d, packetsize : %d"),
				(unsigned int)pt.GetType(), (unsigned int)pt.GetSize()), true, FVector2D{ 2.f, 2.f });
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("no data Recv")), true, FVector2D{ 2.f, 2.f });
	}
	FMemory::Free(recvdata);


}