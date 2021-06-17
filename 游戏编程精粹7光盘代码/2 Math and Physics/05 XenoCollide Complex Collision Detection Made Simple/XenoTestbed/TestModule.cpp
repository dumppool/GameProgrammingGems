#include "StdAfx.h"

#include <list>

#include "TestModule.h"

using namespace std;

vector<RegisterTestModule::FactoryMethod*>* RegisterTestModule::s_factoryMethods = NULL;
vector<string>* RegisterTestModule::s_descriptions = NULL;

