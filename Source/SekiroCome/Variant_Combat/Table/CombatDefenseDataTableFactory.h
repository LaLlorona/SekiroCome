#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "CombatDefenseDataTableFactory.generated.h"

/** Lets Content Browser create UCombatDefenseDataTable assets directly, pre-locked to FCombatDefenseRow */
UCLASS()
class UCombatDefenseDataTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

public:
	UCombatDefenseDataTableFactory();

	virtual bool ConfigureProperties() override;
	virtual FText GetDisplayName() const override;

protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};

#endif // WITH_EDITOR
