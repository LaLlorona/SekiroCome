#pragma once

struct AttackAnimationSectionInfo
{
	FName SectionName;
	float AnimSectionPlaySpeed;

	AttackAnimationSectionInfo() = default;
	AttackAnimationSectionInfo(FName InSectionName, float InAnimSectionPlaySpeed) : SectionName(InSectionName), AnimSectionPlaySpeed(InAnimSectionPlaySpeed) {}
};
