#include "StdAfx.h"
#include "Cell.h"
#include "Group.h"

CCell::CCell(void)
{
	groups[0] = groups[1] = groups[2] = 0;
	value = 0;
	type = UNDEFINED;
}

CCell::~CCell(void)
{
}

void CCell::SetValue(int v, CellTypesEnum t)
{
	if (CanSetValue(v))
	{
		SetValue(v);
		type = t;
	}
}

void CCell::ResetCurrentValue()
{
	if (HasValue())
	{
		for (int i=0; i<3; i++)
			groups[i]->RemoveValue(GetValue());
		type = UNDEFINED;
		value = 0;
	}
}

void CCell::SetValue(int v)
{
	ASSERT(CanSetValue(v));
	ResetCurrentValue();
	for (int i=0; i<3; i++)
		groups[i]->AddValue(v);
	type = CALCULATED;
	value = v;
}

bool CCell::CanSetValue(int v) const
{
	for (int i=0; i<3; i++)
		if (!groups[i]->CanAddValue(v)) return false;
	return true;
}

bool CCell::IsTheOnlyPlaceForValue(int v) const
{
	for (int i=0; i<3; i++)
		if (groups[i]->HowManyPlacesFor(v) == 1) return true;
	return false;	
}

int CCell::GetNumberOfPossiblevalues() const
{
	return 9 - (groups[0]->GetBitfield() | groups[1]->GetBitfield() | groups[2]->GetBitfield()).count();
}

void CCell::SetGroups(CGroup* g1, CGroup* g2, CGroup* g3) 
{
	groups[0] = g1; 
	groups[1] = g2; 
	groups[2] = g3; 
	g1->AddCell(this);
	g2->AddCell(this);
	g3->AddCell(this);
}

void CCell::GetAllPossibleValues(CArray<int>& arr) const
{
	Bitfield bits = groups[0]->GetBitfield() | groups[1]->GetBitfield() | groups[2]->GetBitfield();
	for (int i=1; i<=9; i++)
		if (!bits.has(i)) arr.Add(i);
}

bool CCell::AutoFill()
{
	if (!HasValue())
	{
		CArray<int> arr;
		GetAllPossibleValues(arr);
		if (arr.GetSize() == 1)
		{
			SetValue(arr[0]);
			return true;
		}
		else
		{
			for (int i=0; i<arr.GetSize(); i++)
				if (IsTheOnlyPlaceForValue(arr[i]))
				{
					SetValue(arr[i]);
					return true;
				}
		}
	}
	return false;
}
