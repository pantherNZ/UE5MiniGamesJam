// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

#include <random>
#include <Kismet/GameplayStatics.h>

void UMainGameInstance::Init()
{
	RuntimeLevelData = LevelData;

	UE_LOG( LogTemp, Log, TEXT( "Init: %s %s" ), *GetCurrentLevelName(), *HomeLevel->GetMapName() );

	for( auto&& sequence : RuntimeLevelData )
	{
		const int32 NumShuffles = sequence.Levels.Num() - 1;
		for( int32 i = 0; i < NumShuffles; ++i )
		{
			int32 SwapIdx = FMath::Rand() % ( sequence.Levels.Num() - 1 );
			sequence.Levels.Swap( i, SwapIdx );
		}
	}

	if( GetCurrentLevelName() == GetStrippedLevelName(HomeLevel->GetMapName()) )
	{
		curTime = GameStartTimeSec;
	}
}

FString UMainGameInstance::GetStrippedLevelName(FString FullMapName ) const
{
	// Optional: Remove the path prefix to get just the level name
	int32 LastSlashIndex;
	if( FullMapName.FindLastChar( '/', LastSlashIndex ) )
	{
		return FullMapName.RightChop( LastSlashIndex + 1 );
	}
	FullMapName.RemoveFromStart( GetWorld()->StreamingLevelsPrefix );
	return FullMapName; // Return full name if no slash found
}

FString UMainGameInstance::GetCurrentLevelName() const
{
	UWorld* World = GetWorld();
	if( World )
	{
		// GetMapName() returns the full path (e.g., /Game/Maps/MyLevelName).
		// You might want to strip the path to get just the name.
		FString FullMapName = World->GetMapName();
		return GetStrippedLevelName( FullMapName );
	}
	return FString( TEXT( "Invalid World" ) );
}

void UMainGameInstance::LoadComplete( const float LoadTime, const FString& MapName )
{
	
}

void UMainGameInstance::Tick( float DeltaTime )
{
	if( curTime > 0.0f )
	{
		curTime -= DeltaTime;

		if( curTime <= 0.0f )
		{
			NextLevel();
		}
	}
}

void UMainGameInstance::NextLevel()
{
	const auto& nextLevel = RuntimeLevelData[LevelSequence].Levels[LevelIndex];

	UGameplayStatics::LoadStreamLevel(
		this,
		FName( *nextLevel.Level.GetAssetName() ),
		true, // bMakeVisibleAfterLoad
		true, // bShouldBlockOnLoad
		FLatentActionInfo{}
	);
}

void UMainGameInstance::CompleteLevel()
{
	TotalScore++;
	LevelIndex++;

	if( LevelIndex >= RuntimeLevelData[LevelSequence].Levels.Num() )
	{
		LevelIndex = 0;
		LevelSequence++;
		if( LevelSequence >= RuntimeLevelData.Num() )
		{
			RestartGame();
			return;
		}
	}

	curTime = GameStartTimeSec;
}

void UMainGameInstance::RestartGame()
{
	TotalScore = 0;
	LevelSequence = 0;
	LevelIndex = 0;

	UGameplayStatics::LoadStreamLevel(
		this,
		FName( *HomeLevel.GetAssetName() ),
		true, // bMakeVisibleAfterLoad
		true, // bShouldBlockOnLoad
		FLatentActionInfo{}
	);
}