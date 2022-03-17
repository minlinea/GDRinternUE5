// Fill out your copyright notice in the Description page of Project Settings.


#include "RecvThread.h"
#include "GameFramework/Actor.h"


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
	Exit();
}


/*
*	FRunnableThread::Create 사용(스레드 생성)시 호출 됨 
*/
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
		if (true == ClientRecv(&pt, PACKETHEADER))
		{
			if (PACKETHEADER != pt.GetSize())		//패킷의 크기가 헤더가 아니라면(추가 데이터 수신 시)
			{
				ReadAddData(pt);					//ShotData, ActiveState, BallPlace 패킷 수신 시
			}
			else
			{
				SavePacket<Packet>(pt.GetType());	//ConnectCheck, 각종 Recv 패킷 수신 시
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			//	FString::Printf(TEXT("Error RecvThread Run no data Recv")), true, FVector2D{ 2.f, 2.f });

			Exit();
		}
	}
	return 0;
}

/*
*	일반 패킷이 아닌 추가 데이터를 받은 패킷인 경우
*	DataManagement에서 처리하도록 패킷을 해당 타입으로 큐에 저장(SavePacket<PacketTYPE>)
*/
void RecvThread::ReadAddData(Packet& packet)
{
	size_t recvsize{ packet.GetSize() - PACKETHEADER };	//추가 데이터의 size
	uint8* recvdata{ (uint8*)FMemory::Malloc(recvsize) };	//추가 데이터의 버퍼
	if (true == ClientRecv(recvdata, recvsize))		//추가 데이터 recv
	{
		if (PACKETTYPE::PT_BallPlace == packet.GetType())
		{
			BALLPLACE data;
			FMemory::Memmove(&data, recvdata, sizeof(BALLPLACE));	//데이터 이동
			SavePacket<PacketBallPlace>(data);						//큐 저장
		}
		else if (PACKETTYPE::PT_ShotData == packet.GetType())
		{
			ShotDataInfo data;
			FMemory::Memmove(&data, recvdata, sizeof(ShotDataInfo));//데이터 이동
			SavePacket<PacketShotDataInfo>(data);					//큐 저장
		}
		else if (PACKETTYPE::PT_ActiveState == packet.GetType())
		{
			bool data;
			FMemory::Memmove(&data, recvdata, sizeof(bool));		//데이터 이동
			SavePacket<PacketActiveState>(data);					//큐 저장
		}
		else														//알 수 없는 데이터 타입을 받은 경우
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
				FString::Printf(TEXT("ReadAddData unknown type")), true, FVector2D{ 2.f, 2.f });
		}
		return;
	}
	else															//Recv 자체가 실패한 경우 (소켓 에러) Recv 스레드 종료
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("ReadAddData no data Recv Error")), true, FVector2D{ 2.f, 2.f });
	}
	FMemory::Free(recvdata);
	Exit();
}

/*
*	스레드 소멸 시 호출되는 함수, 소멸자도 Exit를 호출하도록 연결
*/
void RecvThread::Exit()
{
	this->m_bRun = false;		//Run 함수 While 탈출

	if (true != this->m_qPacket.empty())	//큐에 데이터가 남아있는 경우
	{
		for (auto p = this->m_qPacket.front(); true != this->m_qPacket.empty(); )
		{
			p = this->m_qPacket.front();
			delete p;
			this->m_qPacket.pop();
		}
	}
}

/*
*	일시 정지 시 호출하는 함수
*	현재 프로젝트에선 별도의 동작이 없어 Exit로 연결
*/
void RecvThread::Stop()
{
	Exit();
}

bool RecvThread::ClientRecv(void* buf, int size)
{
	//Recv함수인자((uint8*)buf:버퍼, size:버퍼의 최대 사이즈, size:버퍼에 들어올 것으로 기대되는 사이즈)
	return this->m_sSocket->Recv((uint8*)buf, size, size);
}

/*
*	RecvThread에서 받은 패킷을 자신의 패킷큐에 저장하는 함수,  ADataManagement의 CheckQueue함수에서 감시하면서 처리됨 
*	호출 Type 1. PACKETDATA가 타입(PACKETTYPE)인 경우 : 일반 패킷을 저장, 이 때 P는 [Packet] 클래스로 생성
*	호출 Type 2. PACKETDATA가 데이터(ShotData, ActiveState, BallPlace)인 경우 : 해당하는 클래스를 저장, 이 때 P는 [Packet(TYPE)] 클래스로 생성
*/
template <class P, class PACKETDATA>
void RecvThread::SavePacket(PACKETDATA data)
{
	this->m_qPacket.push(new P(data));
}