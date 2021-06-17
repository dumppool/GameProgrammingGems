#pragma once

#include <Vector>
#include <string>

//////////////////////////////////////////////////////////////////////////////
// TestModule is the base class for all modules using this testbed framework.

class TestModule
{

public:

	virtual ~TestModule() {}

	virtual void Init() {};
	virtual void Simulate(float32 dt) {};
	virtual void DrawScene() {};
	virtual void OnKeyDown(uint32 nChar) {};
	virtual void OnChar(uint32 nChar) {};
	virtual void OnLeftButtonDown(const Vector& rayOrigin, const Vector& rayDirection, const Vector& point) {};
	virtual void OnLeftButtonUp(const Vector& rayOrigin, const Vector& rayDirection, const Vector& point) {};
	virtual void OnMiddleButtonDown(const Vector& rayOrigin, const Vector& rayDirection, const Vector& point) {};
	virtual void OnMiddleButtonUp(const Vector& rayOrigin, const Vector& rayDirection, const Vector& point) {};
	virtual void OnRightButtonDown(const Vector& rayOrigin, const Vector& rayDirection, const Vector& point) {};
	virtual void OnRightButtonUp(const Vector& rayOrigin, const Vector& rayDirection, const Vector& point) {};
	virtual void OnMouseMove(const Vector& rayOrigin, const Vector& rayDirection, const Vector& point) {};

};


//////////////////////////////////////////////////////////////

class RegisterTestModule
{
public:

	typedef TestModule* FactoryMethod();

private:

	static std::vector<FactoryMethod*>* s_factoryMethods;
	static std::vector<std::string>* s_descriptions;

public:

	RegisterTestModule( FactoryMethod* factoryMethod, const std::string& description )
	{
		if (!s_factoryMethods)
		{
			s_factoryMethods = new std::vector<FactoryMethod*>;
			s_descriptions = new std::vector<std::string>;
		}

		s_factoryMethods->push_back(factoryMethod);
		s_descriptions->push_back(description);
	}

	static int32 ModuleCount()
	{
		if (!s_factoryMethods || !s_descriptions) return 0;

		ASSERT(s_factoryMethods->size() == s_descriptions->size());
		return (int32) s_factoryMethods->size();
	}

	static FactoryMethod* GetFactoryMethod(int32 index)
	{
		ASSERT(s_factoryMethods != NULL);
		ASSERT(index >= 0 && index < s_factoryMethods->size());
		return s_factoryMethods->at(index);
	}

	static std::string GetDescription(int32 index)
	{
		ASSERT(s_descriptions != NULL);
		ASSERT(index >= 0 && index < s_factoryMethods->size());
		return s_descriptions->at(index);
	}

	static void CleanUp()
	{
		delete s_factoryMethods;
		s_factoryMethods = NULL;

		delete s_descriptions;
		s_descriptions = NULL;
	}
};

//////////////////////////////////////////////////////////////

