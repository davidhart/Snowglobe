#pragma once

#include <string>
#include <map>
#include <list>

class LSystem
{

public:

	LSystem();
	~LSystem();
	
	LSystem(const LSystem& lsystem);
	LSystem& operator=(const LSystem& lsystem);

	void AddRule(const std::string& predecessor, const std::string& successor, float probabilityWeight = 1.0f); 
	void ClearRules();
	void EvaluateRules(const std::string& axiom, std::string& result, unsigned int iterations = 1) const;

private:

	class Successor
	{

	public:

		Successor(const std::string& successor, float probabilityWeight);
		void AppendSelf(std::string& string) const;
		inline float ProbabilityWeight() const;

	private:
		
		std::string _successor;
		float _probabilityWeight;

	};

	typedef std::list<const Successor*> SuccessorList;
	typedef std::map<std::string, SuccessorList> RuleMap;

	void CloneRules(const LSystem& lsystem);
	void DeleteSuccessors();

	void PerformIteration(const std::string& axiom, std::string& result) const;
	void AppendSuccessor(const SuccessorList& successors, std::string& result) const;

	RuleMap _rules;

};

inline float LSystem::Successor::ProbabilityWeight() const
{
	return _probabilityWeight;
}