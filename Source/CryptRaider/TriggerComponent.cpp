// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

// 생성자: 기본적으로 틱을 활성화함
UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;  // 매 프레임 TickComponent가 실행되도록 설정
}

// 게임이 시작될 때 호출되는 함수
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// 매 프레임마다 호출되는 함수
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 현재 트리거 영역 내에서 조건을 만족하는 액터를 찾음
    AActor* Actor = GetAcceptableActor();

    if (Actor != nullptr) // 조건을 만족하는 액터가 있으면
    {
        // 해당 액터의 루트 컴포넌트를 가져와서 물리 시뮬레이션을 비활성화
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component != nullptr)
        {
            Component->SetSimulatePhysics(false);
        }
        // 트리거 컴포넌트에 액터를 부착 (위치 유지)
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        
        // Mover를 활성화하여 이동 시작
        Mover->SetShouldMove(true);
    }
    else // 조건을 만족하는 액터가 없으면
    {
        // Mover를 비활성화하여 이동 중지
        Mover->SetShouldMove(false);
    }
}

// Mover 객체를 설정하는 함수
void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}

// 트리거 박스 내에서 특정 태그를 가진 액터를 찾아 반환하는 함수
AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors); // 현재 트리거 박스와 겹쳐 있는 액터 목록을 가져옴

    for (AActor* Actor : Actors) // 모든 겹쳐 있는 액터를 확인
    {
        // 액터가 지정된 태그(AcceptableActorTag)를 가지고 있는지 확인
        bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        // 액터가 "Grabbed" 태그를 가지고 있는지 확인 (잡혀있는 상태인지 체크)
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");

        // 특정 태그를 가진 액터 중에서 "Grabbed" 태그가 없는 경우 반환
        if (HasAcceptableTag && !IsGrabbed)
        {
            return Actor;
        }
    }
    return nullptr; // 조건을 만족하는 액터가 없으면 nullptr 반환
}
