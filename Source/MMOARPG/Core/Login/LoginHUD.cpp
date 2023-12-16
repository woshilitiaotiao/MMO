// Fill out your copyright notice in the Description page of Project Settings.
#include "LoginHUD.h"
#include "../../UI/Login/UI_LoginMain.h"

ALoginHUD::ALoginHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_LoginMain> LoginMain_BPClass(TEXT("/Game/UI/Login/UI_LoginMain_BP"));
	LoginMainClass = LoginMain_BPClass.Class;
}

void ALoginHUD::BeginPlay()
{
	Super::BeginPlay();

	if (LoginMainClass)
	{
		LoginMain = CreateWidget<UUI_LoginMain>(GetWorld(), LoginMainClass);
		LoginMain->AddToViewport();
	}
}
