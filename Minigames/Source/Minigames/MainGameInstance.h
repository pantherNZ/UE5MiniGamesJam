// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AssetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
    TSoftObjectPtr<UWorld> Level;
};

USTRUCT(BlueprintType)
struct FLevelSequenceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
    TArray<FLevelData> Levels;
};

UCLASS( Blueprintable )
class MINIGAMES_API UMainGameInstance : public UGameInstance, public FTickableGameObject
{
    GENERATED_BODY()

public:
    void Init() override;
    void LoadComplete( const float LoadTime, const FString& MapName ) override;
    void Tick( float DeltaTime ) override;
    TStatId GetStatId() const override { return Super::GetStatID(); }

private:
    FString GetStrippedLevelName( FString FullMapName ) const;
    FString GetCurrentLevelName() const;
    void NextLevel();
    void CompleteLevel();
    void RestartGame();

public:
    UPROPERTY( EditAnywhere )
    TSoftObjectPtr<UWorld> HomeLevel;

    UPROPERTY( BlueprintReadOnly )
    int32 LevelSequence;

     UPROPERTY( EditAnywhere, BlueprintReadWrite )
    int32 LevelIndex;

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    int32 TotalScore;

    UPROPERTY( EditAnywhere )
    float GameStartTimeSec;

    UPROPERTY( EditAnywhere )
    TArray<FLevelSequenceData> LevelData;

    UPROPERTY( BlueprintReadOnly )
    float curTime = 0.0f;

private:
    TArray<FLevelSequenceData> RuntimeLevelData;
};