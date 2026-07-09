#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "CombatAttackTypeDataTableFactory.generated.h"

/** Lets Content Browser create UCombatAttackTypeDataTable assets directly, pre-locked to FCombatAttackTypeRow */
UCLASS()
class UCombatAttackTypeDataTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

public:
	UCombatAttackTypeDataTableFactory();

	virtual bool ConfigureProperties() override;
	virtual FText GetDisplayName() const override;

protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};

#endif // WITH_EDITOR
