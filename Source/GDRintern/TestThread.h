// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
/**
 * 
 */
class GDRINTERN_API TestThread : public FRunnable
{
	bool run;
public:
	TestThread();
	~TestThread();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Exit();
	virtual void Stop();

};
