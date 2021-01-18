#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AudioTableRow.generated.h"

/**
 * Table row for use in looking up sounds based on physical materials. 
 */
USTRUCT(BlueprintType)
struct FAudioTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    USoundBase* Audio;
};