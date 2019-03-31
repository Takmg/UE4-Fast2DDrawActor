# UE4-Fast2DDrawExample

UE4で高速に2D描画を行う方法の1つのアイデアとしてコミット

下記補足説明

** pngなど透過が上手に反映されない場合は下記を修正する。 **

  `/Source/Fast2DDrawExample/Private/HelperFunctions.cpp` 
  `DrawLibraries::DrawTexture` 内
  `TileItem.BlendMode = FCanvas::BlendToSimpleElementBlend( EBlendMode::BLEND_AlphaComposite );`

  上記 `EBlendMode::BLEND_AlphaComposite` を `EBlendMode::BLEND_Masked` に変更する事でうまく透過が動く可能性がある。
※ただしColorKeyを指定したBitmapを描画する場合はBLEND_AlphaCompositeに設定を戻す必要がある。



** PivotModeに応じた座標位置の再計算はデフォルトではしない。 **

透過付きpngから作成したSpriteからはRenderShapesが正しく取得できず、
現在のPivotModeがCenterCenterなのか、TopLeftなのか判断が出来ない。
その為、PivotModeに応じた座標位置の再計算はデフォルトでは行っていない。
※PivotModeからの座標位置再計算を行いたい場合は
  `/Source/Fast2DDrawExample/Private/Fast2DDrawActor.cpp` 
  `void AFast2DDrawActor::DrawComponents` の
  `DrawLibraries::DrawFlipbook` や `DrawLibraries::DrawSprite` の最終引数をtrueとする。


以上。