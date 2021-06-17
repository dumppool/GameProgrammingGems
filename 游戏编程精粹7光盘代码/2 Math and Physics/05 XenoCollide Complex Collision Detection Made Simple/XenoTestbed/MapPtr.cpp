
#include "stdafx.h"

#include <map>
#include <hash_map>

#include "MapPtr.h"

using namespace std;
using namespace stdext;

//map<void*, int> MapPtrBase::s_refCount;
hash_map<void*, int> MapPtrBase::s_refCount;
