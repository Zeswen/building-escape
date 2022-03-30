// Copyright © Zeswen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UMainActorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	template <typename ComponentType, typename ClassType>
	void InitComponent(ClassType *Context, ComponentType **Component, void (ClassType::*MethodCallback)())
	{
		*Component = GetOwner()->FindComponentByClass<ComponentType>();

		if (!Component)
		{
			UE_LOG(LogTemp,
						 Error,
						 TEXT("No %s found on %s"),
						 *FString(typeid(ComponentType).name()),
						 *GetOwner()->GetName());
			return;
		}

		if (MethodCallback)
		{
			(Context->*MethodCallback)();
		}
	};
};
