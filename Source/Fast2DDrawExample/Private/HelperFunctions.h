#pragma once
// Copyright 2018-2019 Takmg All Rights Reserved.
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/BoxComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"



DECLARE_DELEGATE_OneParam( DrawLibraryDelegate , class UCanvas* )


class DrawLibraries
{
public:

	// 初期化
	static void Init( UObject* WorldContextObject , UTextureRenderTarget2D* RenderTarget , DrawLibraryDelegate InFunc );

	// 描画系
	static UTextureRenderTarget2D* CreateRenderTarget2D( UObject* Outer , FName Name , const uint32 Width , const uint32 Height , const ETextureRenderTargetFormat Format = ETextureRenderTargetFormat::RTF_RGBA8 , const FLinearColor ClearColor = FLinearColor::Transparent );
	static void DrawStringFromFont( UCanvas * TargetCanvas , UTexture2D* FontTexture , const FString Text , const FVector2D Position , const bool HighliteFlg = true );
	static void DrawFlipbook( UCanvas * TargetCanvas , class UPaperFlipbookComponent *FlipBookComponent , const FVector2D Position , const bool CalcFromPivot = true );
	static void DrawSprite( UCanvas * TargetCanvas , class UPaperSpriteComponent *Sprite , const FVector2D Position , const bool CalcFromPivot = true );
	static void DrawSprite( UCanvas * TargetCanvas , class UPaperSprite *Sprite , const FVector2D Position , const bool CalcFromPivot = true , const float Rotation = 0 );
	static void DrawTexture( UCanvas * TargetCanvas , UTexture* Texture , const FVector2D CanvasPosition , const FVector2D TexturePosition , const FVector2D TextureSize );
	static void DrawTexture( UCanvas * TargetCanvas , UTexture* Texture , const FVector2D SourcePosition , const FVector2D SourceSize , const FVector2D StartUV , const FVector2D SizeUV , const float Rotation );
};



class ActorHelper
{
public:


	// 
	static void GetDisplayableComponents( class USceneComponent* RootComponent , TArray<class USceneComponent*>& Components , const bool Recursive = false );

private:

	template<class P>
	static void ProcessComponent( USceneComponent* Component , const P& Proc , const bool Recursive = false );

	template<class P>
	static void ProcessComponentRecursive( USceneComponent* Component , const P& Proc );
};
