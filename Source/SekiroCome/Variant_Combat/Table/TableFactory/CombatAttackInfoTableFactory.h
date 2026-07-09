#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "CombatAttackInfoTableFactory.generated.h"

/** Lets Content Browser create UCombatAttackInfoTable assets directly, pre-locked to FCombatAttackInfoRow */
UCLASS()
class UCombatAttackInfoTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

public:
	UCombatAttackInfoTableFactory();

	virtual bool ConfigureProperties() override;
	virtual FText GetDisplayName() const override;

protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};

#endif // WITH_EDITOR
