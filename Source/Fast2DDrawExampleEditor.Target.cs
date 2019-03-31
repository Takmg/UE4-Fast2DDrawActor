// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Fast2DDrawExampleEditorTarget : TargetRules
{
	public Fast2DDrawExampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Fast2DDrawExample" } );
	}
}
