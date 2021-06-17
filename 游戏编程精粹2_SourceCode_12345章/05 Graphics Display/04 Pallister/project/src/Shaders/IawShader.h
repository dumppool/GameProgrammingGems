// IawShader.h App Wizard Version 2.0 Beta 1
// ----------------------------------------------------------------------
// 
// Copyright © 2001 Intel Corporation
// All Rights Reserved
// 
// Permission is granted to use, copy, distribute and prepare derivative works of this 
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  This software is provided "AS IS." 
//
// Intel specifically disclaims all warranties, express or implied, and all liability,
// including consequential and other indirect damages, for the use of this software, 
// including liability for infringement of any proprietary rights, and including the 
// warranties of merchantability and fitness for a particular purpose.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

// ----------------------------------------------------------------------
// Authors: Kim Pallister,Dean Macri - Intel Technology Diffusion Team
// ----------------------------------------------------------------------

#if !defined(IawShader_h)
#define IawShader_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
//This class is used encapsulate a component of a shader pass. 
class IawShaderComponent {

//public methods
public:

	IawShaderComponent(char*	strname = "Unnamed");
	virtual		~IawShaderComponent();

	HRESULT		ActivateShaderComponent();

	HRESULT		SetStateBlock(DWORD	dwSB);
	HRESULT		SetVertexShader(DWORD	dwVS);
	HRESULT		SetPixelShader(DWORD	dwPS);
	HRESULT		SetMatrix(IawMatrix mx, DWORD mxtype);
	
	HRESULT		SetName(char *strname);

	inline DWORD		GetComponentType () {return m_dwComponentType};
	inline DWORD		GetStateBlock () {return m_dwStateBlock};
	inline DWORD		GetVertexShader () {return m_dwVertexShader};
	inline DWORD		GetPixelShader () {return m_dwPixelShader};
	inline IawMatrix	GetMatrix() {return m_mxMat};
	inline DWORD		GetMatrixType() {return m_dwMatType};

	HRESULT AddChild(IawShaderComponent *pComponent);
	inline IawShaderComponent* GetChild() {return m_pNext;}

	void TargetChanging();

	void CheckProperties(CD3DWrapper *pD3DWrapper, bool &b_alpha);

	HRESULT	Activate();
	

//private methods & member vars
private:

	char		m_strShaderComponentName[255];
	DWORD	m_dwShaderComponentType;

	DWORD	m_dwStateBlock;
	DWORD	m_dwVertexShader;
	DWORD	m_dwPixelShader;
	IawMatrix	m_mxMat;
	DWORD	m_dwMatType;

	bool	m_bValidOnCurrentDevice;

//	IawShaderComponent		*m_pNext;
	
	HRESULT	CheckAgainstChildren(IawShaderComponent	*pPotentialParent, IawShaderComponent *pPotentialChild);
};

  */

/**
 * This class describes a shader element.
 */
class IawShaderElement
{
public:
  /**
   * Constructor.
   * @param Id The Id for this element.
   * @param name A name for this element.
   */
  IawShaderElement(int Id, char* name = "Unnamed Element");

  /** Destructor */
  virtual ~IawShaderElement();

  /** Set the element name */
  HRESULT SetName(char* name);

  //@{
  /** Access method */
  IawShaderElement* GetNext() {return mpNext;}
  void SetNext(IawShaderElement* pTemp) {mpNext = pTemp;}
  int GetId() {return mElementId;}
  char* GetName() {return mElementName;}
  void SetValid(bool valid) {mValid = valid;}
  bool GetValid() {return mValid;}
  HRESULT SetStateBlock(DWORD stateBlock, DWORD flags);
  DWORD GetStateBlock() {return mStateBlock;}
  //@}

  /**
   * Delete the state block for this element.
   * @param pWrapper The wrapper for the device this element as a state block on
   */
  HRESULT DeleteStateBlock(IawD3dWrapper* pWrapper);

  /**
   * Add a child to this element's hierarchy.
   */
  HRESULT AddChild(IawShaderElement* pChildElement);

  /** Flag this element as a state block */
  static const int STATE_BLOCK;

  /** Flag this element as a vertex shader */
  static const int oft Word 95"
<Bucket not triggered>

[RISKY, WORD97] "Microsoft Word 97"
<Bucket not triggered>

[RISKY, WORD97, WORDMSIREG] "Microsoft Word"
<Bucket not triggered>

[SAFE, ACCESS94, ACCESS94RT] "Microsoft Access 2.0"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS94, ACCESS94RT, EXCEL94, POWERPOINT94, WORD94] "Microsoft Office 4.x"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95] "Microsoft Access 95"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95, ACCESS97, ACCESSMSIREG, EXCEL95, EXCEL97, EXCELMSIREG, POWERPOINT95, POWERPOINT97, POWERPOINTMSIREG, SCHEDULE95, WORD95, WORD97, WORDMSIREG] "Microsoft Access, Excel, PowerPoint and Word "
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95, ACCESS97, EXCEL95, EXCEL97, OUTLOOK97, POWERPOINT95, POWERPOINT97, PROJECT95, PROJECT98, SCHEDULE95, WORD95, WORD97] "Microsoft Office"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95, EXCEL95, POWERPOINT95, PROJECT95, WORD95] "Microsoft Office 95 Setup"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95, EXCEL95, POWERPOINT95, PROJECT95, WORD95, ACCESS97, EXCEL97, OUTLOOK97, POWERPOINT97, WORD97, PROJECT98, ACCESSMSIREG, EXCELMSIREG, OUTLOOKMSIREG, POWERPOINTMSIREG, WORDMSIREG, PROJECTMSIREG] "Microsoft Office"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95, EXCEL95, POWERPOINT95, SCHEDULE95, WORD95] "Microsoft Office 95 ShortCut Bar"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95, EXCEL95, POWERPOINT95, SCHEDULE95, WORD95, ACCESS97, EXCEL97, OUTLOOK97, POWERPOINT97, WORD97] "Microsoft Office ShortCut Bar"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS95, EXCEL95, POWERPOINT95, WORD95] "Microsoft Office 95 ShortCut Bar"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS97] "Microsoft Access 97"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS97, ACCESS97RT] "Microsoft Access 97"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS97, ACCESS97RT, POWERPOINT97, WORD97] "Microsoft Graph 97"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS97, EXCEL97, MSDRAW97, MSDRAW98, OUTLOOK97, PHOTODRAW1, POWERPOINT97, PROJECT98, PUBLISHER5, WORD97] "Microsoft Office"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS97, EXCEL97, OUTLOOK97, POWERPOINT97, PROJECT98, PUBLISHER5, WORD97] "Microsoft Office"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS97, EXCEL97, OUTLOOK97, POWERPOINT97, PROJECT98, WORD97] "Microsoft Office"
<Triggered for Cleanup/Removal>

[SAFE, ACCESS97, EXCEL97, OUTLOOK97, POWERPOINT97, WORD97] "Microsoft Office 97 ShortCut Bar"
<Triggered for Cleanup/Removal>

[SAFE, DMM97, DMM9PLUS] "Direct Mail Manager"
<Triggered for Cleanup/Removal>

[SAFE, EQUATION95, EQUATION97, EQUATIONMSIREG] "Microsoft Office Equation Editor"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL94] "Microsoft Excel 5.0"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL95] "Microsoft Excel 95"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL95, EXCEL97, EXCELMSIREG] "DataMap"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL95, POWERPOINT95, PROJECT94, PROJECT95] "Microsoft Excel, PowerPoint and Project"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL95, POWERPOINT95, WORD95, EXCEL97, WORD97, POWERPOINT97] "Microsoft Office Binder"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL95, POWERPOINT95, WORD95, EXCEL97, WORD97, POWERPOINT97, EXCELMSIREG, POWERPOINTMSIREG, WORDMSIREG] "Microsoft Office Binder"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL97] "Microsoft Excel 97"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL97, POWERPOINT97, WORD97] "Microsoft Office 97 Bookshelf Basics"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL97, WORD97, POWERPOINT97] "Microsoft Office 97 Binder"
<Triggered for Cleanup/Removal>

[SAFE, FRONTPAGE1X] "Microsoft FrontPage 1.1"
<Triggered for Cleanup/Removal>

[SAFE, GRAPH95, ACCESS95RT] "Microsoft Graph"
<Triggered for Cleanup/Removal>

[SAFE, IMAGER95] "Microsoft Imager 95"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL94, MSQUERY94, EXCEL95, MSQUERY95] "Microsoft Office Query"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL94, MSQUERY94, EXCEL95, MSQUERY95, EXCEL97, MSQUERY97] "Microsoft Office Query"
<Triggered for Cleanup/Removal>

[SAFE, EXCEL95, MSQUERY95] "Microsoft Office 95 Query"
<Triggered for Cleanup/Removal>

[SAFE, OUTLOOK97] "Microsoft Outlook"
<Triggered for Cleanup/Removal>

[SAFE, OUTLOOK97, OUTLOOKMSIREG] "Microsoft Outlook"
<Triggered for Cleanup/Removal>

[SAFE, PHOTOED97] "Microsoft PhotoEditor 97"
<Triggered for Cleanup/Removal>

[SAFE, POWERPOINT94, POWERPOINT95, POWERPOINT97] "Microsoft PowerPoint"
<Triggered for Cleanup/Removal>

[SAFE, POWERPOINT95] "Microsoft PowerPoint 95"
<Triggered for Cleanup/Removal>

[SAFE, POWERPOINT97] "Microsoft PowerPoint 97"
<Triggered for Cleanup/Removal>

[SAFE, PUBLISHER3, PUBLISHER4] "Microsoft Publisher"
<Triggered for Cleanup/Removal>

[SAFE, PUBLISHER3, PUBLISHER4, PUBLISHER5, PUBLISHERMSIREG] "Microsoft Publisher"
<Triggered for Cleanup/Removal>

[SAFE, PUBLISHER4, PUBLISHER5] "Microsoft Publisher"
<Triggered for Cleanup/Removal>

[SAFE, PUBLISHER5] "Microsoft Publisher 98"
<Triggered for Cleanup/Removal>

[SAFE, RUNFROMNETCD95] "Microsoft Office 95"
<Triggered for Cleanup/Removal>

[SAFE, SBFM97] "Small Business Financial Manager 97"
<Triggered for Cleanup/Removal>

[SAFE, WORD94] "Microsoft Word 6.0"
<Triggered for Cleanup/Removal>

[SAFE, WORD94, WORD95, WORD97] "Microsoft Word"
<Triggered for Cleanup/Removal>

[SAFE, WORD95] "Microsoft Word 95"
<Triggered for Cleanup/Removal>

[SAFE, WORD95, WORD97] "Microsoft Word"
<Triggered for Cleanup/Removal>

[SAFE, WORD97] "Microsoft Word 97"
<Triggered for Cleanup/Removal>

[SAFE, WORD97, PUBLISHER5] "Microsoft Publisher and Word"
<Triggered for Cleanup/Removal>


#### Custom Data File: OCLNCUST.OPC ####

[SAFE, ACCESS94, ACCESS94RT] "Microsoft Access 2.0."
<Triggered for Cleanup/Removal>

[RISKY, ACCESS94, ACCESS94RT] "Microsoft Access 2.0."
<Bucket not triggered>

[RISKY, CUSTOMDIC94] "Custom dictionary used by Microsoft Office 4.x."
<Bucket not triggered>

[RISKY, MSQUERY94] "Microsoft Office 4.x Query."
<Bucket not triggered>

[RISKY, SYSTEM94] "Microsoft Office."
<Bucket not triggered>

[RISKY, WORD94] "Word 6.0."
<Bucket not triggered>

[SAFE, ACCESS95] "Microsoft Access 95."
<Triggered for Cleanup/Removal>

[RISKY, ACCESS95] "Microsoft Access 95."
<Bucket not triggered>

[RISKY, BOOKS95] "Microsoft Office 95 Bookshelf."
<Bucket not triggered>

[RISKY, CUSTOMDIC95] "Custom dictionary used by Microsoft Office 95."
<Bucket not triggered>

[RISKY, DAO95] "Data Access Object file."
<Bucket not triggered>

[RISKY, EXCEL95] "Microsoft Excel 95."
<Bucket not triggered>

[RISKY, OFFICEACL95] "Microsoft Office 95."
<Bucket not triggered>

[RISKY, SYSTEM95] "Microsoft Office."
<Bucket not triggered>

[RISKY, WINDOWS95] "Microsoft Office 95."
<Bucket not triggered>

[RISKY, ACCESS97] "Microsoft Access 97."
<Bucket not triggered>

[RISKY, ACCESS95, ACCESS97, ACCESS9] "Microsoft Access."
<Bucket not triggered>

[RISKY, EXCEL97, POWERPOINT97, WORD97] "Microsoft Office 97 Bookshelf Basics."
<