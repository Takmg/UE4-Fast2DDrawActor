# UE4-Fast2DDrawExample

UE4�ō�����2D�`����s�����@��1�̃A�C�f�A�Ƃ��ăR�~�b�g

���L�⑫����

** png�ȂǓ��߂����ɔ��f����Ȃ��ꍇ�͉��L���C������B **

  `/Source/Fast2DDrawExample/Private/HelperFunctions.cpp` 
  `DrawLibraries::DrawTexture` ��
  `TileItem.BlendMode = FCanvas::BlendToSimpleElementBlend( EBlendMode::BLEND_AlphaComposite );`

  ��L `EBlendMode::BLEND_AlphaComposite` �� `EBlendMode::BLEND_Masked` �ɕύX���鎖�ł��܂����߂������\��������B
��������ColorKey���w�肵��Bitmap��`�悷��ꍇ��BLEND_AlphaComposite�ɐݒ��߂��K�v������B



** PivotMode�ɉ��������W�ʒu�̍Čv�Z�̓f�t�H���g�ł͂��Ȃ��B **

���ߕt��png����쐬����Sprite�����RenderShapes���������擾�ł����A
���݂�PivotMode��CenterCenter�Ȃ̂��ATopLeft�Ȃ̂����f���o���Ȃ��B
���ׁ̈APivotMode�ɉ��������W�ʒu�̍Čv�Z�̓f�t�H���g�ł͍s���Ă��Ȃ��B
��PivotMode����̍��W�ʒu�Čv�Z���s�������ꍇ��
  `/Source/Fast2DDrawExample/Private/Fast2DDrawActor.cpp` 
  `void AFast2DDrawActor::DrawComponents` ��
  `DrawLibraries::DrawFlipbook` �� `DrawLibraries::DrawSprite` �̍ŏI������true�Ƃ���B


�ȏ�B