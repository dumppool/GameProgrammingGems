/* Copyright (C) James Boer, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) James Boer, 2000"
 */
class Sample
{
public:
	Sample()		{  Clear();  }
	~Sample()		{  Destroy();  }

	void Clear();
	
	bool Create();
	void Update();
	void Destroy();
};

