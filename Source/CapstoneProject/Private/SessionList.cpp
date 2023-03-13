#include "SessionList.h"

void USessionList::SetLAN(bool LAN)
{
	if (LANCheckBox)
	{
		LANCheckBox->SetIsChecked(LAN);
	}
}
