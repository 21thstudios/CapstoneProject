#include "PlayerStats.generated.h"


UCLASS(BlueprintType)
/**
 * 
 */
class CAPSTONEPROJECT_API UPlayerStats : public UObject
{
  GENERATED_BODY()

public:
	UPlayerStats();
	~UPlayerStats();

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int PlayerKills;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int PlayerDeaths;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName PlayerName;

  UFUNCTION(BlueprintCallable)
  void AddKill();
  UFUNCTION(BlueprintCallable)
  void AddDeath();
  UFUNCTION(BlueprintCallable)
  void HandleDeath(UPlayerStats* Actor);
  UFUNCTION(BlueprintCallable)
  void ClearStats();

  // getter and setter for each 
  UFUNCTION(BlueprintCallable)
  int GetPlayerKills();
  UFUNCTION(BlueprintCallable)
  int GetPlayerDeaths();
  UFUNCTION(BlueprintCallable)
  void SetPlayerKills(int Kills);
  UFUNCTION(BlueprintCallable)
  void SetPlayerDeaths(int Deaths);
  UFUNCTION(BlueprintCallable)
  void SetPlayerName(FName Name);
  UFUNCTION(BlueprintCallable)
  FName GetPlayerName();
};