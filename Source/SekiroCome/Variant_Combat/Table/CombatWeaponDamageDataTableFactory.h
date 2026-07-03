#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "CombatWeaponDamageDataTableFactory.generated.h"

/** Lets Content Browser create UCombatWeaponDamageDataTable assets directly, pre-locked to FCombatWeaponDamageRow */
UCLASS()
class UCombatWeaponDamageDataTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

public:
	UCombatWeaponDamageDataTableFactory();

	virtual bool ConfigureProperties() override;
	virtual FText GetDisplayName() const override;

protected:
	virtual UDataTable* MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags) override;
};

#endif // WITH_EDITOR
