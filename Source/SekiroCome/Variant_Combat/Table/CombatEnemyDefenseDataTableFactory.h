#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "CombatEnemyDefenseDataTableFactory.generated.h"

/** Lets Content Browser create UCombatEnemyDefenseDataTable assets directly, pre-locked to FCombatEnemyDefenseRow */
UCLASS()
class UCombatEnemyDefenseDataTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

public:
	UCombatEnemyDefenseDataTableFactory();

	virtual bool ConfigureProperties() override;
	virtual FText GetDisplayName() const override;

protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};

#endif // WITH_EDITOR
