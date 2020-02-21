#include "StdAfx.h"
#include "Group.h"
#include "Cell.h"
CGroup::CGroup(void) 
{
	memset(cells, 0, sizeof(cells));
}

CGroup::~CGroup(void)
{
}

void CGroup::AddValue(int v)
{
	ASSERT(CanAddValue(v));
	bitfield += v;
}

int CGroup::HowManyPlacesFor(int v) const
{
	int res = 0;
	for (int i=0; i<9; i++)
	{
		ASSERT(cells[i]);
		if (!cells[i]->HasValue() && cells[i]->CanSetValue(v)) 
			res++;
	}
	return res;
}

void CGroup::RemoveValue(int v)
{
	ASSERT(HasValue(v));
	bitfield -= v;
}
bool CGroup::HasValue(int v) const
{
	return bitfield.has(v);
}


void CGroup::AddCell(CCell* pCell)
{
	for (int i=0; i<9; i++)
		if (!cells[i])
		{
			cells[i] = pCell;
			return;
		}
	ASSERT(0);
	return;
}
	
int Bitfield::count() const
{
	int res = 0;
	for (int i=1; i<=9; i++)
		if (has(i)) res ++;
	return res;
}

