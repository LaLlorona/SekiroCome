#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "CombatMontageTableFactory.generated.h"

/** Lets Content Browser create UCombatMontageTable assets directly, pre-locked to FCombatMontageRow */
UCLASS()
class UCombatMontageTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

public:
	UCombatMontageTableFactory();

	virtual bool ConfigureProperties() override;
	virtual FText GetDisplayName() const override;

protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};

#endif // WITH_EDITOR
