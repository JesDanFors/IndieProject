// Copyright 2015-2020 Piperift. All Rights Reserved.

#include "LatentActions/LoadGameAction.h"
#include "SaveManager.h"
#include "Serialization/SlotDataTask_Loader.h"
#include "SlotInfo.h"


FLoadGameAction::FLoadGameAction(USaveManager* Manager, FName SlotName, ELoadGameResult& OutResult, const FLatentActionInfo& LatentInfo)
	: Result(OutResult)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
{
	bool bStarted;
	if (Manager->IsAdvancedLoad)
	{
		bStarted = Manager->LoadSlotAdvance(SlotName, FOnGameLoaded::CreateRaw(this, &FLoadGameAction::OnLoadFinished));
	}
	else
	{
		bStarted = Manager->LoadSlot(SlotName, FOnGameLoaded::CreateRaw(this, &FLoadGameAction::OnLoadFinished));
	}
	
	if (!bStarted)
	{
		Result = ELoadGameResult::Failed;
	}
}

void FLoadGameAction::UpdateOperation(FLatentResponse& Response)
{
	Response.FinishAndTriggerIf(Result != ELoadGameResult::Loading, ExecutionFunction, OutputLink, CallbackTarget);
}

void FLoadGameAction::OnLoadFinished(USlotInfo* SavedSlot)
{
	Result = SavedSlot ? ELoadGameResult::Continue : ELoadGameResult::Failed;
}
