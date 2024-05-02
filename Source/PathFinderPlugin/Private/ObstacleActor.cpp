// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleActor.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
#include "GridEditSubsystem.h"
#endif

// Sets default values
AObstacleActor::AObstacleActor()
#if WITH_EDITOR
	:BoxColliderSize(100,100,100)
	, IsColliderDirty(false)
	, IsDrawCollider(false)
	, IsCollision(false)
	, IsRegistered(false)
#endif
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 스태틱 메시
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}


#if WITH_EDITOR
void AObstacleActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 여기 이름 설정하는거 변경해줘야함
	if (TEXT("BoxColliderSize") == PropertyChangedEvent.MemberProperty->GetFName())
	{
		IsColliderDirty = true;
	}
}


void AObstacleActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (false == IsRegistered)
	{
		IsRegistered = true;
		if (UGridEditSubsystem* gridEditSubsystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>()) {
			gridEditSubsystem->AddObstacleActor(this);
		}
	}


	IsColliderDirty = true;
}


void AObstacleActor::UpdateBoxCollider()
{
	MTransform transform;
	{
		FTransform temp = GetActorTransform();

		FVector position = temp.GetLocation();
		FQuat rotation = temp.GetRotation();
		FVector scale = temp.GetScale3D();

		transform.Position.Set(position.X, position.Y, position.Z);
		transform.Rotation.Set(rotation.X, rotation.Y, rotation.Z, rotation.W);
		transform.Scale.Set(scale.X, scale.Y, scale.Z);
	}


	BoxCollider.Set(transform, MVector3(BoxColliderSize.X, BoxColliderSize.Y, BoxColliderSize.Z));
}


void AObstacleActor::Destroyed()
{
	if (UGridEditSubsystem* gridEditSubsystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>()) {
		gridEditSubsystem->RemoveObstacleActor(this);
	}

	IsRegistered = false;

	Super::Destroyed();
}


#endif

bool AObstacleActor::ShouldTickIfViewportsOnly() const
{
	// 체크
	if (nullptr != GetWorld() && GetWorld()->WorldType == EWorldType::Editor)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}


// Called when the game starts or when spawned
void AObstacleActor::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AObstacleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	{
		if (true == IsColliderDirty)
		{
			IsColliderDirty = false;

			UpdateBoxCollider();

			// 출력용 포인트 정보 설정
			{
				std::vector<MVector3> pointList;
				BoxCollider.GetPointList(pointList);

				ColliderPointList.Empty();
				for (MVector3& pos : pointList) {
					ColliderPointList.Emplace(FVector(pos.X, pos.Y, pos.Z));
				}
			}
		}

		if (true == IsDrawCollider)
		{
			if (8 == ColliderPointList.Num())
			{
				FLinearColor color = FLinearColor::Red;
				if (IsCollision) {
					color = FLinearColor::Blue;
				}


				// 후면
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[0], ColliderPointList[1], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[1], ColliderPointList[2], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[2], ColliderPointList[3], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[3], ColliderPointList[0], color, 0, 1);

				// 전면
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[4], ColliderPointList[5], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[5], ColliderPointList[6], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[6], ColliderPointList[7], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[7], ColliderPointList[4], color, 0, 1);

				// 측면
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[0], ColliderPointList[4], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[1], ColliderPointList[5], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[2], ColliderPointList[6], color, 0, 1);
				UKismetSystemLibrary::DrawDebugLine(this, ColliderPointList[3], ColliderPointList[7], color, 0, 1);
			}
		}
	}
#endif
}



