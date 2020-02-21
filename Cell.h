#pragma once

#include <afxtempl.h>
class CGroup;

enum CellTypesEnum
{
	LOADED_FROM_FILE,
	USER_DEFINED,
	CALCULATED_SAVED,
	CALCULATED,
	UNDEFINED	
};

class CCell
{
	CGroup* groups[3];
	int value;
	CellTypesEnum type;

public:
	CCell(void);
	~CCell(void);

	void SetGroups(CGroup* g1, CGroup* g2, CGroup* g3);

	void SetUserValue(int value){SetValue(value, USER_DEFINED);}
	void SetLoadedValue(int value){SetValue(value, LOADED_FROM_FILE);}
	bool HasValue() const {return value != 0;}
	int GetValue() const {return value;}
	CellTypesEnum GetType() const {return type;}
	void ResetCurrentValue();
	void SetValue(int v);
	void SetValue(int v, CellTypesEnum type);
	void SetType(CellTypesEnum t){type = t;}
	bool CanSetValue(int v) const;
	bool IsTheOnlyPlaceForValue(int v) const;

	int GetNumberOfPossiblevalues() const;
	void GetAllPossibleValues(CArray<int>& arr) const;
	bool AutoFill();
};
