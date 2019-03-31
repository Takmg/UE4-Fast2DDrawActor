// Copyright 2018-2019 Takmg All Rights Reserved.
#include "Fast2DDrawActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "HelperFunctions.h"


AFast2DDrawActor::AFast2DDrawActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SetTickGroup( ETickingGroup::TG_PostUpdateWork );

	// Spriteを作成する
	RenderTargetSprite = CreateDefaultSubobject<UPaperSpriteComponent>( TEXT( "RenderSprite" ) );
	RenderTargetSprite->SetSimulatePhysics( false );
	RenderTargetSprite->SetEnableGravity( false );
	RenderTargetSprite->BodyInstance.bLockRotation = true;

	// ルートに設定
	RootComponent = RenderTargetSprite;
}


void AFast2DDrawActor::OnConstruction( const FTransform & Transform )
{
	// 親を呼び出す
	Super::OnConstruction( Transform );

	// バックバッファの作成
	RenderTarget2D = DrawLibraries::CreateRenderTarget2D( this , TEXT( "FastRenderTarget" ) , 640 , 480 );
}


void AFast2DDrawActor::BeginPlay()
{
	Super::BeginPlay();

	// Spriteの設定
	if( ResolutionSprite )
	{
		RenderTargetSprite->SetSprite( ResolutionSprite );
	}

	// 親マテリアル
	if( ParentMaterial )
	{
		// 親マテリアルを設定する
		RenderTargetMID = UKismetMaterialLibrary::CreateDynamicMaterialInstance( this , ParentMaterial );
		RenderTargetMID->SetTextureParameterValue( TextureParameterName , RenderTarget2D );

		// スプライトにマテリアルをくっ付ける
		RenderTargetSprite->SetMaterial( 0 , RenderTargetMID );
	}

	OnDraw();
}


void AFast2DDrawActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	OnDraw();
}


void AFast2DDrawActor::OnDraw()
{
	// 描画対象を取得
	TArray<USceneComponent*> DisplayComponents;
	ActorHelper::GetDisplayableComponents( RootComponent , DisplayComponents , true );
	DisplayComponents.Remove( RootComponent );

	// 描画対象が存在しない場合は何もしない
	if( DisplayComponents.Num() <= 0 ) { return; }

	// Y軸(奥行き)を基準に小さい順にスワップ
	DisplayComponents.Sort( []( const auto& A , const auto& B ) {
		const FVector LA1 = A.K2_GetComponentLocation();
		const FVector LA2 = B.K2_GetComponentLocation();
		return LA1.Y < LA2.Y;
	} );

	// 表示処理
	DrawLibraries::Init( this , RenderTarget2D , DrawLibraryDelegate::CreateLambda( [&]( UCanvas* Canvas ) {
		DrawComponents( Canvas , DisplayComponents );
	} ) );
}

void AFast2DDrawActor::DrawComponents( UCanvas* TargetCanvas , const TArray<USceneComponent*>& DisplayComponents )
{
	// 
	for( USceneComponent* Component : DisplayComponents )
	{
		// 現在のロケーションを取得
		FVector WorldLocation = Component->K2_GetComponentLocation();

		// キャンバス上のロケーションを指定
		FVector2D CanvasLocation( WorldLocation.X , -WorldLocation.Z );	// Z座標とY座標の入れ替え

		// キャスト成功した場合は表示
		if( auto* Comp = Cast< UPaperFlipbookComponent>( Component ) )
		{
			DrawLibraries::DrawFlipbook( TargetCanvas , Comp , CanvasLocation , false );
		}
		// キャストに成功した場合は表示
		else if( auto* Comp = Cast< UPaperSpriteComponent>( Component ) )
		{
			DrawLibraries::DrawSprite( TargetCanvas , Comp , CanvasLocation , false );
		}
		else
		{
			// 表示失敗した場合はここに来る
			continue;
		}

		// 表示したものは非表示にする
		Component->SetHiddenInGame( true );
	}

}
