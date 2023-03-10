// Copyright 2015-2020 Piperift. All Rights Reserved.

#include "LatentActions/SaveGameAction.h"
#include "SaveManager.h"
#include "SlotInfo.h"


FSaveGameAction::FSaveGameAction(USaveManager* Manager, FName SlotName, bool bOverrideIfNeeded, bool bScreenshot, const FScreenshotSize Size, ESaveGameResult& OutResult, const FLatentActionInfo& LatentInfo)
	: Result(OutResult)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
{
	bool bStarted;
	if(Manager->IsAdvancedSave)
	{
		bStarted = Manager->SaveSlotAdvance(SlotName, bOverrideIfNeeded, bScreenshot, Size, FOnGameSaved::CreateRaw(this, &FSaveGameAction::OnSaveFinished));
	}
	else
	{
		bStarted = Manager->SaveSlot(SlotName, bOverrideIfNeeded, bScreenshot, Size, FOnGameSaved::CreateRaw(this, &FSaveGameAction::OnSaveFinished));
	}
	
	
	if (!bStarted)
	{
		Result = ESaveGameResult::Failed;
	}
}

void FSaveGameAction::UpdateOperation(FLatentResponse& Response)
{
	Response.FinishAndTriggerIf(Result != ESaveGameResult::Saving, ExecutionFunction, OutputLink, CallbackTarget);
}

void FSaveGameAction::OnSaveFinished(USlotInfo* SavedSlot)
{
	Result = SavedSlot ? ESaveGameResult::Continue : ESaveGameResult::Failed;
}
