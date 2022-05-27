#pragma once
#include "GPCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AGPCamera : public APawn
{
    GENERATED_BODY()
    
public:
    AGPCamera();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    void EdgePanning(float deltaSeconds);
    void Zoom(float deltaSeconds);
    void Rotation(float deltaSeconds);

    virtual void SetupPlayerInputComponent(UInputComponent* inputComponent) override;
    void ScrollZoomInput(float axisValue);
    void RotationInput(float axisValue);
    
    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneComponent;
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;
    
    UPROPERTY(EditAnywhere, Category="Edge Panning")
    bool bEnableEdgePanning = true;
    UPROPERTY(EditAnywhere, Category="Edge Panning")
    float EdgeScrollSpeed = 1000.f;
    UPROPERTY(EditAnywhere, Category="Edge Panning")
    float EdgeRangePercentage = 0.05f;
    UPROPERTY(EditAnywhere, Category="Edge Panning")
    float minEdgeSpeedPercentage = 0.2f;

    UPROPERTY(EditAnywhere, Category="Zoom")
    float ZoomStrength = 500.f;
    UPROPERTY(EditAnywhere, Category="Zoom")
    float ZoomSpeed = 4.f;
    UPROPERTY(EditAnywhere, Category="Zoom")
    float ZoomMin = 1000.f;
    UPROPERTY(EditAnywhere, Category="Zoom")
    float ZoomMax = 2500.f;

    UPROPERTY(EditAnywhere, Category="Rotation")
    float RotationStrength = 20.f;
    UPROPERTY(EditAnywhere, Category="Rotation")
    float RotationSpeed = 6.f;
    
private:
    FVector2D mousePos;
    FVector2D viewportSize;

    FVector moveVector;

    float currentSpringArmLength;
    float nextSpringArmLength;
    float zoomModifier;

    float newRotationYaw;

    FRotator currentRotation;
    FRotator nextRotation;
    FRotator newRotation;
};