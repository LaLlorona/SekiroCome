namespace UtilityLogic
{
	void PrintString(const FString& Message)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, Message);
		}
	}
}