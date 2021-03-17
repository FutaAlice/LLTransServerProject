// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

#include <map>
#include <string>

#include "Engine/Engine.h"
#include "Engine.h"

#include "Features/IModularFeatures.h"  // IModularFeatures
#include "LiveLinkClient.h" // ILiveLinkClient
#include "Roles/LiveLinkBasicRole.h"    // ULiveLinkBasicRole

#include "fuckwindows.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float x = 0.5, y = 1.1;
    // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f"), x, y));

    ILiveLinkClient* LiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
    // TSharedPtr<FCustomARKitLiveLinkSource> Source = MakeShareable(new FCustomARKitLiveLinkSource());

    TArray<FLiveLinkSubjectKey> SubjectKeyArray = LiveLinkClient->GetSubjects(false, false);
    for (auto SubjectKey : SubjectKeyArray) {
        const FLiveLinkSubjectName SubjectName = SubjectKey.SubjectName;
        if (SubjectName == "iPhoneXFaceAR") {
        }
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, SubjectName.ToString());
    }

    // Show valid and enable
    const FLiveLinkSubjectName LiveLinkSubjectName("iPhoneXFaceAR");
    FString message;
    message += "Valid: ";
    message += LiveLinkClient->IsSubjectValid(LiveLinkSubjectName) ? "true" : "false";
    message += ", Enabled: ";
    message += LiveLinkClient->IsSubjectEnabled(LiveLinkSubjectName) ? "true" : "false";
    // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

    // do something
    if (LiveLinkClient->IsSubjectValid(LiveLinkSubjectName) && LiveLinkClient->IsSubjectEnabled(LiveLinkSubjectName)) {

        // get a role
        TSubclassOf<ULiveLinkRole> SubClass = LiveLinkClient->GetSubjectRole(LiveLinkSubjectName);
        FString ClassName = SubClass->GetFName().ToString();
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ClassName);

        // Evaluate frame
        FLiveLinkSubjectFrameData LiveLinkSubjectFrameData;
        bool bSuccess = LiveLinkClient->EvaluateFrame_AnyThread(LiveLinkSubjectName, SubClass, LiveLinkSubjectFrameData);
        message = "";
        message += "bSuccess: ";
        message += bSuccess ? "true" : "false";
        message += ", ";
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

        //StaticData.FindPropertyValue(BasicData.FrameData, PropertyName, Value);
        //  bool FindPropertyValue(const FLiveLinkBaseFrameData & FrameData, const FName PropertyName, float& OutValue) const


        FLiveLinkBaseStaticData *StaticData = LiveLinkSubjectFrameData.StaticData.GetBaseData();
        FLiveLinkBaseFrameData *FrameData = LiveLinkSubjectFrameData.FrameData.GetBaseData();

        message = "";

        std::map<std::string, float> kv;

        TArray<FName> &PropertyNames = StaticData->PropertyNames;
        for (auto Name : PropertyNames) {
            float OutValue;
            StaticData->FindPropertyValue(*FrameData, Name, OutValue);
            message += Name.ToString();
            message += ":";
            message += FString::SanitizeFloat(OutValue);
            message += ", ";

            FString FS = Name.ToString();
            // const TCHAR* ch = FS.operator*();
            char* ch = TCHAR_TO_ANSI(*FS);
            kv[ch] = OutValue;
        }
        std::string json_string = foo(kv);

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(json_string.c_str()));
    }
}


