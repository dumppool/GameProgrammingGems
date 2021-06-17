/******************************************************************************

  Copyright (C) 1999, 2000 NVIDIA Corporation
  This file is provided without support, instruction, or implied warranty of any
  kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
  not liable under any circumstances for any damages or loss whatsoever arising
  from the use or inability to use this file or items derived from it.
  
    Comments:
	To use:
	- Inherit from EBPropertySet
	- Call AddProperty(new EBtypeProperty("Name", OBJECT_MEMBER(membervaroftype))
	- External clients can do GetProperty(), GetNextProperty(), etc. to find
	out about properties of objects that inherit from EBPropertySet.

	It's not the fastest/memory saving solution, but it's about as simple as it could
	be to understand.

	rtti and templates would make this thing a whole lot simpler, but not easy to follow

	OBJECT_MEMBER is a little trick to find the address in a class of a member variable
    

******************************************************************************/
#ifndef __EB_PROPERTY_H
#define __EB_PROPERTY_H

#pragma warning(disable: 4786)
#include <map>
#pragma warning(disable: 4786)
#include <list>
#include <assert.h>
#include <string.h>

class EBPropertySet;
class EBProperty;
class EBEnumProperty;

#define OBJECT_MEMBER(member) ((BYTE*)(&member) - (BYTE*)this)

// Property flags
#define PROPFLAG_PRIVATE (1 << 0)	// Private properties to the class that the user never sees
#define PROPFLAG_PUBLIC  (1 << 1)	// Public properties for things like menu options
#define PROPFLAG_CALLBACK (1 << 2)	// Property that requires a callback when it is modified
#define PROPFLAG_ENUMSTATIC (1 << 3) // This is an enum property (it's value is fixed and can't change)

// Info about this object, containing properties
typedef std::map<EBString, EBProperty*> t_mapProperties;

// Allowable property types
typedef enum tagEBPropertyType
{
	EBTYPE_BOOL_PROP = 0,		// bools
	EBTYPE_DWORD_PROP = 1,		// dwords
	EBTYPE_ENUM_PROP = 2,		// enum props
	EBTYPE_UNUSED	 = 3,		// unused
	EBTYPE_STRING_PROP = 4,		// string props
	EBTYPE_TRIGGER_PROP = 5	// trigger props
} EBPropertyType;

// Type-safe types.  These are used in place of standard types
// and enable the property support to validate the type
class EBValue 
{
public:
	EBValue(EBPropertyType Type)
		: m_Type(Type)
	{}

	EBValue(bool bValue)
		: m_bValue(bValue),
		m_Type(EBTYPE_BOOL_PROP)
	{}

	EBValue(DWORD dwValue)
		: m_dwValue(dwValue),
		m_Type(EBTYPE_DWORD_PROP)
	{}

	EBValue(const EBString& strValue)
		: m_strValue(strValue),
		m_Type(EBTYPE_STRING_PROP)
	{}

	EBValue(const std::string& strValue)
		: m_strValue(strValue),
		m_Type(EBTYPE_STRING_PROP)
	{ }

	EBValue(const EBValue& rhs)
	{
		operator = (rhs);
	}

	virtual ~EBValue() {};

	virtual bool IsKindOf(EBPropertyType Type) const
	{
		if (Type == m_Type)
			return true;
		return false;
	}

	virtual const EBValue& operator = (const EBValue& rhs) 
	{
		switch(rhs.m_Type)
		{
			case EBTYPE_BOOL_PROP:
				m_bValue = rhs.m_bValue;
				break;
			case EBTYPE_STRING_PROP:
				m_strValue = rhs.m_strValue;
				break;
			case EBTYPE_DWORD_PROP:
				m_dwValue = rhs.m_dwValue;
				break;
			default:
				assert(0);
				break;
		}
		m_Type = rhs.m_Type;

		return *this;
	}

	virtual const EBValue& operator = (const bool rhs) { assert(IsKindOf(EBTYPE_BOOL_PROP)); m_bValue = rhs; return *this; }
	virtual const EBValue& operator = (const DWORD rhs) { assert(IsKindOf(EBTYPE_DWORD_PROP)); m_dwValue = rhs; return *this;}
	virtual const EBValue& operator = (const EBString& rhs) { assert(IsKindOf(EBTYPE_STRING_PROP)); m_strValue = rhs; return *this;}

	virtual bool operator == (const EBValue& rhs) 
	{
		if (m_Type != rhs.m_Type)
			return false;

		switch(m_Type)
		{
			case EBTYPE_BOOL_PROP:
				return (m_bValue == rhs.m_bValue);
				break;
			case EBTYPE_DWORD_PROP:
				return (m_dwValue == rhs.m_dwValue);
				break;
			case EBTYPE_STRING_PROP:
				return (m_strValue == rhs.m_strValue);
				break;
			default:
				assert(0);
				break;
		}
		return false;
	}

	// WARNING: Must not provide an operator std::string() here.  That would cause the effect
	// to manufacture a string in it's own crt heap, and pass that back to the app.
	// Only EBString's are valid for passing across in this way.
	virtual operator bool() const { assert(IsKindOf(EBTYPE_BOOL_PROP)); return m_bValue; }
	virtual operator DWORD() const { assert(IsKindOf(EBTYPE_DWORD_PROP)); return m_dwValue; }
	virtual operator EBString() { assert(IsKindOf(EBTYPE_STRING_PROP)); return m_strValue; }
	virtual operator const EBString() const { assert(IsKindOf(EBTYPE_STRING_PROP)); return m_strValue; }
	virtual operator const char*() const { assert(IsKindOf(EBTYPE_STRING_PROP)); return m_strValue.c_str(); }

	virtual void Write(void* pAddress) const
	{
		switch(m_Type)
		{
			case EBTYPE_BOOL_PROP:
				*((bool*)pAddress) = m_bValue;
				break;
			case EBTYPE_STRING_PROP:
				*((EBString*)pAddress) = m_strValue;
				break;
			case EBTYPE_DWORD_PROP:
				*((DWORD*)pAddress) = m_dwValue;
				break;
			default:
				assert(0);
				break;
		}
	}

	virtual void Read(void* pAddress)
	{
		switch(m_Type)
		{
			case EBTYPE_BOOL_PROP:
				m_bValue = *((bool*)pAddress);
				break;
			case EBTYPE_STRING_PROP:
				m_strValue = *((EBString*)pAddress);
				break;
			case EBTYPE_DWORD_PROP:
				m_dwValue = *((DWORD*)pAddress);
				break;
			default:
				assert(0);
				break;
		}
	}

				
protected:
	EBPropertyType m_Type;

	// Not a union, because union members can't have copy constructors
	bool m_bValue;
	DWORD m_dwValue;
	EBString m_strValue;

};

// Base poperty class
class EBProperty
{
public:
	EBProperty(const EBString& strName, DWORD dwOffset, EBPropertyType Type, DWORD dwPropertyFlags = PROPFLAG_PUBLIC)
	: m_strPropertyName(strName),
	m_dwOffset(dwOffset),
	m_dwPropertyFlags(dwPropertyFlags),
	m_PropertyType(Type)
	{
	}

	virtual ~EBProperty()
	{ }

	virtual void WriteValue(EBPropertySet* pObj, const EBValue& Value);
	virtual EBValue ReadValue(EBPropertySet* pObj);

	virtual bool IsKindOf(EBPropertyType Type) const
	{
		if (Type == m_PropertyType)
			return true;
		return false;
	}

	virtual DWORD GetFlags() const { return m_dwPropertyFlags; }
	virtual const EBString GetPropertyName() const { return m_strPropertyName; }
	virtual void DiscardProperty() { };
	virtual DWORD GetOffset() const { return m_dwOffset; }

protected:
	DWORD m_dwOffset;
	EBPropertyType m_PropertyType;
	DWORD m_dwPropertyFlags;
	EBString m_strPropertyName;
};

// Base class for all objects - enables property support
class EBPropertySet
{
public:						
	EBPropertySet::EBPropertySet()
	{
		m_mapProperties.clear();
	}

	virtual EBPropertySet::~EBPropertySet()
	{
		DiscardProperties();
	}

	// Property support
	virtual bool AddProperty(EBProperty* pProperty)
	{
		assert(pProperty);
		m_mapProperties[pProperty->GetPropertyName()] = pProperty;
		return true;
	}

	virtual EBProperty* GetProperty(const EBString& PropertyKey)
	{
		if (m_mapProperties.empty())
			return NULL;

		t_mapProperties::const_iterator itrProperties = m_mapProperties.find(PropertyKey);
		if (itrProperties != m_mapProperties.end())
		{
			return itrProperties->second;
		}
		return NULL;
	}

	// Override the callback if you want to catch triggers.
	virtual void PropertyUpdateCallback(const EBProperty* pProperty, bool bWritten)
	{
		assert(pProperty);	
	}

	virtual void UpdateProperties()
	{
		DiscardProperties();
		m_itrProperties = m_mapProperties.end();

	}

	virtual void DiscardProperties()
	{
		while(!m_mapProperties.empty())
		{
			t_mapProperties::iterator itrProperties = m_mapProperties.begin();
			
			if ((*itrProperties).second)
			{
				EBProperty* pProperty = ((*itrProperties).second);
				pProperty->DiscardProperty();
			}

			delete itrProperties->second;
			m_mapProperties.erase(itr  €       ' _sims2.exe:D3DOGL_67207556' H' Èÿÿÿvk   €       ' _sims2ep1.exe:D3DOGL_67207556' Èÿÿÿvk   €       ' _sims2sp1.exe:D3DOGL_67207556+' Èÿÿÿvk   €       ' _sims2ep2.exe:D3DOGL_67207556' Èÿÿÿvk   €       ' _sims2ep3.exe:D3DOGL_67207556' Èÿÿÿvk   €	 @     ' _sar2007.exe:D3DOGL_67207556Xr' ¸ÿÿÿvk-   € @    ' _Sniper Elite/SniperElite.exe:D3DOGL_67207556' Èÿÿÿvk   €       ' _sof2.exe:OGL_TexClampBehavior' Èÿÿÿvk    €       ' _sof2mp.exe:OGL_TexClampBehaviorÈÿÿÿvk   €       ' _sof2.exe:D3DOGL_67207556ˆ' €ˆ' Èÿÿÿvk   €         _sof2mp.exe:D3DOGL_67207556     Èÿÿÿvk   €         _soldiers.exe:D3DOGL_67207556   Àÿÿÿvk'   € C      _soeldnersecretwars.exe:D3DOGL_67207556 Èÿÿÿvk    €	 @      _sonicriders.exe:D3DOGL_67207556Àÿÿÿvk$   €	 @      _sonicridersdemo.exe:D3DOGL_67207556    Àÿÿÿvk$   €         _neowiz/launcher.exe:D3DOGL_67207556    Èÿÿÿvk   €         _spellforce.exe:D3DOGL_67207556 Èÿÿÿvk    € @       _spellforce2.exe:D3DOGL_67207556Èÿÿÿvk    €	D@      _scda_online.exe:D3DOGL_67207556Èÿÿÿvk   €       ' _prey.exe:D3DOGL_67207556»' ¼' Ğÿÿÿvk   €      ' _prey.exe:OGL_18920725' Èÿÿÿvk   €C    ' _-007.exe:D3DOGL_67207556( ¨( Èÿÿÿvk   €C    ( _POP3.exe:D3DOGL_67207556à' ˜à' Ğÿÿÿvk   €C    ' _PoP.exe:D3DOGL_67207556Èÿÿÿvk   €C    ' _PoP2.exe:D3DOGL_67207556æ' ç' Àÿÿÿvk#   €C    ' _PrinceofPersia.exe:D3DOGL_67207556 ˜ê' Ğÿÿÿvk   €À    ' _pcm.exe:D3DOGL_67207556       hbin 1                        ¸ÿÿÿvk/   €          _shadowstrike_static_retail.exe:D3DOGL_70835937 Àÿÿÿvk!   €          _splintercell.exe:D3DOGL_70835937       ¸ÿÿÿvk/   €         _shadowstrike_static_retail.exe:D3DOGL_67207556 Àÿÿÿvk!   €         _splintercell.exe:D3DOGL_67207556       ¸ÿÿÿvk/   €         _shadowstrike_static_retail.exe:D3DOGL_64877940 Àÿÿÿvk!   €         _splintercell.exe:D3DOGL_64877940       Àÿÿÿvk"   €          _splintercell3.exe:D3DOGL_70835937      Àÿÿÿvk"   €	 @      _splintercell3.exe:D3DOGL_67207556      Àÿÿÿvk"   €         _splintercell3.exe:D3DOGL_64877940      Èÿÿÿvk   €   €      _splintercell3.exe:D3D_32661390 Àÿÿÿvk"   €	 @      _splintercell4.exe:D3DOGL_67207556      Èÿÿÿvk   €         _pandora.exe:D3DOGL_67207556    Àÿÿÿvk"   €         _splintercell2.exe:D3DOGL_67207556      Èÿÿÿvk   €         _jk2sp.exe:OGL_TexClampBehavior Èÿÿÿvk   €         _jk2mp.exe:D3DOGL_67207556      Èÿÿÿvk   €         _jk2sp.exe:D3DOGL_67207556      Èÿÿÿvk   €         _jk2mp.exe:OGL_App_SupportBits  Èÿÿÿvk   €         _jk2sp.exe:OGL_App_SupportBits  Èÿÿÿvk   €         _swkotor.exe:D3DOGL_67207556    Èÿÿÿvk   €         _swkotor2.exe:D3DOGL_67207556   Àÿÿÿvk'   €@      _swrepubliccommando.exe:D3DOGL_67207556 Èÿÿÿvk    € @      _starwolves2.exe:D3DOGL_67207556Àÿÿÿvk%   € @      _starwolves2_demo.exe:D3DOGL_67207556   Àÿÿÿvk"   €"À       _battlefrontii.exe:D3DOGL_67207556      ¸ÿÿÿvk0   €‰ @       _Stargate SG-1 WIP/system/uw.exe:D3DOGL_67207556Èÿÿÿvk   € @      _stgame.exe:D3DOGL_67207556     Ğÿÿÿvk   €       _sm.exe:D3DOGL_67207556 Èÿÿÿvk   €       _sm_release.exe:D3DOGL_67207556 Èÿÿÿvk    €         _stokedrider.exe:D3DOGL_67207556Èÿÿÿvk   €         _stolen.exe:D3DOGL_67207556     Èÿÿÿvk   € @      _Stranger.exe:D3DOGL_67207556   Àÿÿÿvk!   € @      _Strangerdemo.exe:D3DOGL_67207556       Ğÿÿÿvk   €	 Ã      _srs.exe:D3DOGL_67207556Èÿÿÿvk    €         _stronghold2.exe:D3DOGL_67207556Àÿÿÿvk&   € @      _strongholdlegends.exe:D3DOGL_67207556  Èÿÿÿvk    €         _Stubbs Demo.exe:D3DOGL_67207556Èÿÿÿvk   €         _Stubbs.exe:D3DOGL_67207556     Àÿÿÿvk!   €	 @      _suddenattack.exe:D3DOGL_67207556       Ğÿÿÿvk   €