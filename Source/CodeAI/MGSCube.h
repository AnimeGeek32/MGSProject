// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MGSCube.generated.h"

UCLASS()
class CODEAI_API AMGSCube : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CubeMesh;
	
public:	
	// Sets default values for this actor's properties
	AMGSCube();

};
