// Copyright 2018-2019 Takmg All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fast2DDrawActor.generated.h"


/**
 *
 */
UCLASS()
class FAST2DDRAWEXAMPLE_API AFast2DDrawActor : public AActor
{

	GENERATED_BODY()

public:
	AFast2DDrawActor();



	//-----------------------------------------------------------------------------------
	// override functions
protected:
	virtual void OnConstruction( const FTransform& Transform ) override;
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;

protected:

	//-----------------------------------------------------------------------------------
	// functions
	virtual void OnDraw();
	virtual void DrawComponents( UCanvas* TargetCanvas , const TArray<USceneComponent*>& DisplayComponents );

private:

	// RenderTarget2Dを描画する対象のスプライトコンポーネント
	UPROPERTY( BlueprintReadOnly , Category = RenderTarget , Meta = (AllowPrivateAccess = true) )
		class UPaperSpriteComponent* RenderTargetSprite;

	// RenderTarget2D
	UPROPERTY( BlueprintReadOnly , Category = RenderTarget , Meta = (AllowPrivateAccess = true) )
		class UTextureRenderTarget2D* RenderTarget2D;

private:

	//-------------------------------------------------------------------------------------
	// 以下エディタから設定するもの

	// RenderTarget2Dを描画する対象のスプライト
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly , Category = RenderTarget , Meta = (AllowPrivateAccess = true) )
		class UPaperSprite* ResolutionSprite;

	// 親マテリアル
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly , Category = RenderTarget , Meta = (AllowPrivateAccess = true) )
		class UMaterialInterface* ParentMaterial;

	// マテリアルのテクスチャパラメータ名
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly , Category = RenderTarget , Meta = (AllowPrivateAccess = true) )
		FName TextureParameterName;

private:

	// 描画対象スプライトのマテリアルインスタンス
	class UMaterialInstanceDynamic* RenderTargetMID;
};
