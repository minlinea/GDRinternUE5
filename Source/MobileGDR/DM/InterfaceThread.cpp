// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceThread.h"

InterfaceThread::InterfaceThread()
{
	this->m_bRun = false;
}

InterfaceThread::InterfaceThread(FSocket* socket)
{
	this->m_bRun = false;
	this->m_sSocket = socket;
}

InterfaceThread::~InterfaceThread()
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
