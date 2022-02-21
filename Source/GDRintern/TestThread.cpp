// Fill out your copyright notice in the Description page of Project Settings.


#include "TestThread.h"

TestThread::TestThread()
{
	this->run = false;
}

TestThread::~TestThread()
{

}

bool TestThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		FString::Printf(TEXT("TestThread Init")), true, FVector2D{ 2.f, 2.f });

	this->run = true;

	return true;
}
uint32 TestThread::Run()
{
	uint32 t{ 1 };
	int cnt = 0;
	while (this->run)
	{

	}
	return t;
}
void TestThread::Exit()
{

}
void TestThread::Stop()
{
	this->run = false;
}