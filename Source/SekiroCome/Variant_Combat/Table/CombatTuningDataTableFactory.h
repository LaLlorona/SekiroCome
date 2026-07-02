#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "CombatTuningDataTableFactory.generated.h"

/** Lets Content Browser create UCombatTuningDataTable assets directly, pre-locked to FCombatTuningRow */
UCLASS()
class UCombatTuningDataTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

public:
	UCombatTuningDataTableFactory();

	virtual bool ConfigureProperties() override;
	virtual FText GetDisplayName() const override;

protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};

#endif // WITH_EDITOR
