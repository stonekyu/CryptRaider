// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// 생성자: 기본적으로 틱을 활성화함
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true; // 매 프레임 TickComponent 실행
}

// 게임 시작 시 호출되는 함수
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}

// 매 프레임마다 호출되는 함수
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 PhysicsHandle이 활성화되어 있는지 확인
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		// 잡은 물체를 현재 컴포넌트 위치 + 앞 방향 * HoldDistance 만큼 이동
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

// 물체를 집는 함수
void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	// 잡을 수 있는 물체가 있는지 확인
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);  // 물리 활성화
		HitComponent->WakeAllRigidBodies(); // 모든 물리 바디 활성화
		AActor* HitActor = HitResult.GetActor();
		
		// "Grabbed" 태그를 추가하여 상태 관리
		HitActor->Tags.Add("Grabbed");

		// 액터가 다른 곳에 부착되어 있다면 해제
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		// PhysicsHandle을 사용하여 물체를 잡음
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
}

// 물체를 놓는 함수
void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		// 잡고 있던 액터의 "Grabbed" 태그 제거
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");

		// PhysicsHandle을 사용하여 물체를 놓음
		PhysicsHandle->ReleaseComponent();
	}
}

// UPhysicsHandleComponent를 가져오는 함수
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return Result;
}

// 잡을 수 있는 물체가 있는지 확인하는 함수
bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	// 시작 위치: 현재 컴포넌트 위치
	FVector Start = GetComponentLocation();
	
	// 종료 위치: 시작 위치 + 앞 방향 * MaxGrabDistance
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	// 충돌을 감지할 구형 범위 설정
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	// 지정된 거리 내에서 충돌 감지 (ECC_GameTraceChannel2 사용)
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2,
		Sphere
	);
}
