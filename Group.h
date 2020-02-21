#pragma once

class Bitfield
{
	int bits;
	static Bitfield fromBits(int bits){Bitfield res; res.bits = bits; return res;}	

public:
	Bitfield(int value):bits(1 << value){ASSERT(value > 0 && value < 10);}
	Bitfield():bits(0){}

	bool has(int value) const {return *this & Bitfield(value);}
	void operator +=(Bitfield b){bits |= b.bits;}
	void operator -=(Bitfield b){bits &= ~b.bits;}
	Bitfield operator& (Bitfield a) const {return fromBits(bits & a.bits);}
	Bitfield operator| (Bitfield a) const {return fromBits(bits | a.bits);}	
	operator bool () const {return bits != 0;}
	int count() const;
};

class CCell;

class CGroup
{
	Bitfield bitfield;
	CCell* cells[9];
public:
	CGroup(void);
	~CGroup(void);

	void AddCell(CCell* pCell);

	bool CanAddValue(int v) const {return !HasValue(v);}
	int HowManyPlacesFor(int v) const;
	void AddValue(int v);
	void RemoveValue(int v);
	bool HasValue(int v) const;

	Bitfield GetBitfield() const {return bitfield;}
};

