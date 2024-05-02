// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MBoxCollider.h"

#include "ObstacleActor.generated.h"


UCLASS()
class PATHFINDERPLUGIN_API AObstacleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleActor();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	void SetDrawCollider(bool inFlag) {
		IsDrawCollider = inFlag;
	}

	MBoxCollider* GetBoxCollider() {
		return &BoxCollider;
	}
#endif

protected:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// 위치 이동에 대한 처리
	virtual void OnConstruction(const FTransform& Transform) override;

	// 박스 충돌체 갱신
	void UpdateBoxCollider();

	virtual void Destroyed() override;

	
#endif

	// 틱처리 체크
	virtual bool ShouldTickIfViewportsOnly() const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent = nullptr;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	FVector BoxColliderSize;

	// 박스 충돌체
	MBoxCollider BoxCollider;

	bool IsColliderDirty;
	

	bool IsDrawCollider;

	// 충돌했는지
	bool IsCollision;

	// 각 포인트 리스트
	TArray<FVector> ColliderPointList;

	// 등록되었는지
	bool IsRegistered;
#endif
};
