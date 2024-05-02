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

	// ��ġ �̵��� ���� ó��
	virtual void OnConstruction(const FTransform& Transform) override;

	// �ڽ� �浹ü ����
	void UpdateBoxCollider();

	virtual void Destroyed() override;

	
#endif

	// ƽó�� üũ
	virtual bool ShouldTickIfViewportsOnly() const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent = nullptr;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	FVector BoxColliderSize;

	// �ڽ� �浹ü
	MBoxCollider BoxCollider;

	bool IsColliderDirty;
	

	bool IsDrawCollider;

	// �浹�ߴ���
	bool IsCollision;

	// �� ����Ʈ ����Ʈ
	TArray<FVector> ColliderPointList;

	// ��ϵǾ�����
	bool IsRegistered;
#endif
};
