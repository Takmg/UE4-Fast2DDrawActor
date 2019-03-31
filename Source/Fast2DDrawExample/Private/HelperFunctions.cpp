// Copyright 2018-2019 Takmg All Rights Reserved.
#include "HelperFunctions.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Object.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "Core/Public/Core.h"

void DrawLibraries::Init( UObject* WorldContextObject , UTextureRenderTarget2D * RenderTarget , DrawLibraryDelegate InFunc )
{
	// RenderTargetがnullなら何もしない
	if( RenderTarget == nullptr ) { return; }

	// 表示
	UCanvas* TargetCanvas = nullptr;
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	// 表示開始
	UKismetRenderingLibrary::ClearRenderTarget2D( WorldContextObject , RenderTarget , FLinearColor::Transparent );
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget( WorldContextObject , RenderTarget , TargetCanvas , Size , Context );

	InFunc.ExecuteIfBound( TargetCanvas );

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget( WorldContextObject , Context );
}

UTextureRenderTarget2D * DrawLibraries::CreateRenderTarget2D( UObject * Outer , FName Name , const uint32 Width , const uint32 Height , const ETextureRenderTargetFormat Format , const FLinearColor ClearColor )
{
	UTextureRenderTarget2D* RenderTarget2D = NewObject<UTextureRenderTarget2D>( Outer , Name );

	RenderTarget2D->RenderTargetFormat = Format;
	RenderTarget2D->ClearColor = ClearColor;
	RenderTarget2D->InitAutoFormat( Width , Height );
	RenderTarget2D->LODGroup = TextureGroup::TEXTUREGROUP_Pixels2D;
	RenderTarget2D->UpdateResourceImmediate( true );

	return RenderTarget2D;
}

void DrawLibraries::DrawFlipbook( UCanvas * TargetCanvas , UPaperFlipbookComponent *FlipBookComponent , const FVector2D Position , const bool CalcFromPivot )
{
	// 現在のフレームを取得
	const int32 NowFrame = FlipBookComponent->GetPlaybackPositionInFrames();

	// FlipBookの取得
	UPaperFlipbook* FlipBook = FlipBookComponent->GetFlipbook();
	if( FlipBook == nullptr ) { return; }

	// スプライトの取得
	UPaperSprite* Sprite = FlipBook->GetSpriteAtFrame( NowFrame );
	if( Sprite == nullptr ) { return; }

	// 回転度(Degree)を取得
	const float Rotate = FMath::Abs( FlipBookComponent->K2_GetComponentRotation().Yaw );

	DrawSprite( TargetCanvas , Sprite , Position , CalcFromPivot , 175 <= Rotate && Rotate <= 185 ? 180 : 0 );
}

void DrawLibraries::DrawSprite( UCanvas * TargetCanvas , UPaperSpriteComponent * Sprite , const FVector2D Position , const bool CalcFromPivot )
{
	// 回転度(Degree)を取得
	const float Rotate = FMath::Abs( Sprite->K2_GetComponentRotation().Yaw );

	// スプライトの取得
	DrawSprite( TargetCanvas , Sprite->GetSprite() , Position , CalcFromPivot , 175 <= Rotate && Rotate <= 185 ? 180 : 0 );
}

void DrawLibraries::DrawSprite( UCanvas * TargetCanvas , UPaperSprite * Sprite , const FVector2D Position , const bool CalcFromPivot , const float Rotation )
{
	// AtlasDataの取得
	FSlateAtlasData AtlasData = Sprite->GetSlateAtlasData();

	// Textureの取得
	UTexture2D* Texture = Cast<UTexture2D>( AtlasData.AtlasTexture );
	if( Texture == nullptr ) { return; }

	// テクスチャの情報取得
	const FVector2D StartUV = AtlasData.StartUV;
	const FVector2D SizeUV = AtlasData.SizeUV;
	const FVector2D SourceDimension = AtlasData.GetSourceDimensions();
	const FVector2D TextureDimension( Texture->GetSizeX() , Texture->GetSizeY() );

	// 計算後のポジション
	FVector2D NewPosition = Position;
	if( CalcFromPivot )
	{
		// Pivotから再計算 ※Pivotは取れないので描画用領域から計算を行う
		const FBoxSphereBounds Bounds = Sprite->GetRenderBounds();
		const FVector2D Origin( Bounds.Origin.X , -Bounds.Origin.Z );
		const FVector2D Extent( Bounds.BoxExtent.X , Bounds.BoxExtent.Z );
		const FVector2D Diff = Origin - Extent;

		NewPosition += Diff;

		/*
		// ※※※Pivotがエディタモード以外で取得できないので下記廃止※※※
		// Pivotポジションの取得
		FVector2D PivotPosition = Sprite->GetPivotPosition();

		// マイナスする位置
		const FVector2D StartPosition = (StartUV * TextureDimension);
		const float MinusX = PivotPosition.X - StartPosition.X;
		const float MinusY = PivotPosition.Y - StartPosition.Y;

		// Pivotポイント分マイナスした個所から表示する
		NewPosition.X -= MinusX;
		NewPosition.Y -= MinusY;
		*/
	}

	// テクスチャの描画
	DrawTexture( TargetCanvas , Texture , NewPosition , SourceDimension , StartUV , SizeUV , Rotation );
}

void DrawLibraries::DrawTexture( UCanvas * TargetCanvas , UTexture * Texture , const FVector2D CanvasPosition , const FVector2D TexturePosition , const FVector2D TextureSize )
{
	// テクスチャ2Dの取得
	UTexture2D* Texture2D = Cast<UTexture2D>( Texture );
	UTextureRenderTarget2D* TextureRT2D = Cast<UTextureRenderTarget2D>( Texture );
	if( Texture2D == nullptr && TextureRT2D == nullptr ) { return; }

	// TextureDimensionの取得
	FVector2D TextureDimension;
	if( Texture2D != nullptr ) { TextureDimension = FVector2D( Texture2D->GetSizeX() , Texture2D->GetSizeY() ); }
	else { TextureDimension = FVector2D( TextureRT2D->SizeX , TextureRT2D->SizeY ); }

	// テクスチャの描画
	DrawTexture( TargetCanvas , Texture , CanvasPosition , TextureSize , TexturePosition / TextureDimension , TextureSize / TextureDimension , 0 );
}

void DrawLibraries::DrawTexture( UCanvas * TargetCanvas , UTexture * Texture , const FVector2D SourcePosition , const FVector2D SourceSize , const FVector2D StartUV , const FVector2D SizeUV , const float Rotation )
{
	// テクスチャの描画
	FTexture* RenderTextureResource = Texture->Resource;
	FCanvasTileItem TileItem( SourcePosition , RenderTextureResource , SourceSize , StartUV , StartUV + SizeUV , FLinearColor::White );
	TileItem.Rotation = FRotator( Rotation , 0 , 0 );
	TileItem.PivotPoint = FVector2D( 0.5f , 0.5f );
	TileItem.BlendMode = FCanvas::BlendToSimpleElementBlend( EBlendMode::BLEND_AlphaComposite );
	TargetCanvas->DrawItem( TileItem );
}




void ActorHelper::GetDisplayableComponents( USceneComponent * RootComponent , TArray<USceneComponent*>& Components , const bool Recursive )
{
	// Delegate
	const auto f = [&]( USceneComponent* Comp ) {

		// フリップブックかスプライト以外だったら何もしない。
		if( Cast<UPaperFlipbookComponent>( Comp ) == nullptr && Cast<UPaperSpriteComponent>( Comp ) == nullptr ) { return; }

		// コンポーネントに追加
		Components.Add( Comp );
	};

	// 再帰的にコンポーネントに設定
	ProcessComponent( RootComponent , f , Recursive );
}

template<class P>
inline void ActorHelper::ProcessComponent( USceneComponent * Component , const P& Proc , const bool Recursive ) {

	// コンポーネントがnullなら何もしない。
	if( Component == nullptr ) { return; }

	if( Recursive )
	{
		// 自分自身のアタッチコンポーネントを取得する(第一引数でfalseを指定)
		TArray<USceneComponent*> ChildComponents;
		Component->GetChildrenComponents( true , ChildComponents );

		// 子供のコンポーネントに対して再帰処理を行う
		for( USceneComponent* ChildComponent : ChildComponents )
		{
			Proc( ChildComponent );
		}
	}
	else
	{
		ProcessComponentRecursive( Component , Proc );
	}
}

template<class P>
inline void ActorHelper::ProcessComponentRecursive( USceneComponent * Component , const P& Proc )
{
	// コンポーネントがnullなら何もしない。
	if( Component == nullptr ) { return; }

	// 親元から渡されるDelegateを設定
	Proc( Component );

	// 自分自身のアタッチコンポーネントを取得する(第一引数でfalseを指定)
	TArray<USceneComponent*> ChildComponents;
	Component->GetChildrenComponents( false , ChildComponents );

	// 子供のコンポーネントに対して再帰処理を行う
	for( USceneComponent* ChildComponent : ChildComponents )
	{
		ProcessComponentRecursive( ChildComponent , Proc );
	}
}
