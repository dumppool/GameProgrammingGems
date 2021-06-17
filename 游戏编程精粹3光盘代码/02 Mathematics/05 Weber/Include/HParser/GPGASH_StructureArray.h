
/******************************************************************************
IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

By downloading, copying, installing or using the software you agree to this
license. If you do not agree to this license, do not download, install,
copy or use the software.

Intel Open Source License for "Improved Bones Deformation" and
"Constrained Inverse Kinematics" sample code

Copyright (c) 2002, Intel Corporation.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
	list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.
* Neither the name of Intel Corporation nor the names of its contributors may
	be used to endorse or promote products derived from this software without
	specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE INTEL CORPORATION OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
EXPORT LAWS: THIS LICENSE ADDS NO RESTRICTIONS TO THE EXPORT LAWS OF YOUR
JURISDICTION. It is each licensee's responsibility to comply with any export
regulations applicable in each licensee's jurisdiction.
******************************************************************************/

#ifndef __GPGASH_StructureArray_h__
#define __GPGASH_StructureArray_h__



/******************************************************************************
class GPGASH_StructureArray

******************************************************************************/
class GPGASH_StructureArray
	{
	public:
							GPGASH_StructureArray(void)	{ Reset(); };

		void				Reset(void)
								{
								m_structures.Clear();
								};

		GPGASH_Structure	*Create(const char *structname)
								{
								GPGASH_Structure *structure=
											&m_structures.CreateNewElement();

								structure->SetStructureName(structname);
								return structure;
								};

		GPGASH_Structure	*Find(const char *structname)
								{
								long m,len=m_structures.GetNumberElements();
								for(m=0;m<len;m++)
									{
									if(!strcmp(structname,
											m_structures[m].GetStructureName()))
										return &m_structures[m];
									}

								return NULL;
								};

	private:
		GPGArray<GPGASH_Structure>	m_structures;
	};



#endif /* __GPGASH_StructureArray__ */
