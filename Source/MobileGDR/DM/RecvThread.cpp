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
*	FRunnableThread::Create ���(������ ����)�� ȣ�� �� 
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
			if (PACKETHEADER != pt.GetSize())		//��Ŷ�� ũ�Ⱑ ����� �ƴ϶��(�߰� ������ ���� ��)
			{
				ReadAddData(pt);					//ShotData, ActiveState, BallPlace ��Ŷ ���� ��
			}
			else
			{
				SavePacket<Packet>(pt.GetType());	//ConnectCheck, ���� Recv ��Ŷ ���� ��
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
*	�Ϲ� ��Ŷ�� �ƴ� �߰� �����͸� ���� ��Ŷ�� ���
*	DataManagement���� ó���ϵ��� ��Ŷ�� �ش� Ÿ������ ť�� ����(SavePacket<PacketTYPE>)
*/
void RecvThread::ReadAddData(Packet& packet)
{
	size_t recvsize{ packet.GetSize() - PACKETHEADER };	//�߰� �������� size
	uint8* recvdata{ (uint8*)FMemory::Malloc(recvsize) };	//�߰� �������� ����
	if (true == ClientRecv(recvdata, recvsize))		//�߰� ������ recv
	{
		if (PACKETTYPE::PT_BallPlace == packet.GetType())
		{
			BALLPLACE data;
			FMemory::Memmove(&data, recvdata, sizeof(BALLPLACE));	//������ �̵�
			SavePacket<PacketBallPlace>(data);						//ť ����
		}
		else if (PACKETTYPE::PT_ShotData == packet.GetType())
		{
			ShotDataInfo data;
			FMemory::Memmove(&data, recvdata, sizeof(ShotDataInfo));//������ �̵�
			SavePacket<PacketShotDataInfo>(data);					//ť ����
		}
		else if (PACKETTYPE::PT_ActiveState == packet.GetType())
		{
			bool data;
			FMemory::Memmove(&data, recvdata, sizeof(bool));		//������ �̵�
			SavePacket<PacketActiveState>(data);					//ť ����
		}
		else														//�� �� ���� ������ Ÿ���� ���� ���
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
				FString::Printf(TEXT("ReadAddData unknown type")), true, FVector2D{ 2.f, 2.f });
		}
		return;
	}
	else															//Recv ��ü�� ������ ��� (���� ����) Recv ������ ����
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("ReadAddData no data Recv Error")), true, FVector2D{ 2.f, 2.f });
	}
	FMemory::Free(recvdata);
	Exit();
}

/*
*	������ �Ҹ� �� ȣ��Ǵ� �Լ�, �Ҹ��ڵ� Exit�� ȣ���ϵ��� ����
*/
void RecvThread::Exit()
{
	this->m_bRun = false;		//Run �Լ� While Ż��

	if (true != this->m_qPacket.empty())	//ť�� �����Ͱ� �����ִ� ���
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
*	�Ͻ� ���� �� ȣ���ϴ� �Լ�
*	���� ������Ʈ���� ������ ������ ���� Exit�� ����
*/
void RecvThread::Stop()
{
	Exit();
}

bool RecvThread::ClientRecv(void* buf, int size)
{
	//Recv�Լ�����((uint8*)buf:����, size:������ �ִ� ������, size:���ۿ� ���� ������ ���Ǵ� ������)
	return this->m_sSocket->Recv((uint8*)buf, size, size);
}

/*
*	RecvThread���� ���� ��Ŷ�� �ڽ��� ��Ŷť�� �����ϴ� �Լ�,  ADataManagement�� CheckQueue�Լ����� �����ϸ鼭 ó���� 
*	ȣ�� Type 1. PACKETDATA�� Ÿ��(PACKETTYPE)�� ��� : �Ϲ� ��Ŷ�� ����, �� �� P�� [Packet] Ŭ������ ����
*	ȣ�� Type 2. PACKETDATA�� ������(ShotData, ActiveState, BallPlace)�� ��� : �ش��ϴ� Ŭ������ ����, �� �� P�� [Packet(TYPE)] Ŭ������ ����
*/
template <class P, class PACKETDATA>
void RecvThread::SavePacket(PACKETDATA data)
{
	this->m_qPacket.push(new P(data));
}