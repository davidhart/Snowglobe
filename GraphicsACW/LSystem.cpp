// David Hart - 2011

#include "LSystem.h"
#include <cassert>
#include <exception>
#include <cmath>

LSystem::LSystem()
{

}

LSystem::~LSystem()
{
	try
	{
		DeleteSuccessors();
	}
	catch (const std::exception&)
	{

	}
}

LSystem::LSystem(const LSystem& lsystem)
{
	CloneRules(lsystem);
}

LSystem& LSystem::operator=(const LSystem& lsystem)
{
	if (this == &lsystem)
	{
		return *this;
	}

	DeleteSuccessors();

	_rules = RuleMap();

	CloneRules(lsystem);

	return *this;
}

void LSystem::AddRule(const std::string& predecessor, const std::string& successor, float probabilityWeight)
{
	_rules[predecessor].push_back(new Successor(successor, probabilityWeight));
}

void LSystem::EvaluateRules(const std::string& axiom, std::string& result, unsigned int iterations) const
{
	if (iterations == 0)
	{
		result = axiom;
		return;
	}

	PerformIteration(axiom, result);

	for (unsigned int i = 1; i < iterations; ++i) 
	{
		std::string initial;
		result.swap(initial);

		PerformIteration(initial, result);
	}
}

void LSystem::CloneRules(const LSystem& lsystem)
{
	for (RuleMap::const_iterator i = lsystem._rules.begin(); i != _rules.end(); ++i)
	{
		SuccessorList& successors = _rules[i->first];

		for (SuccessorList::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			successors.push_back(new Successor(**j));
		}
	}
}

void LSystem::DeleteSuccessors()
{
	for (RuleMap::iterator i = _rules.begin(); i != _rules.end(); ++i)
	{
		for (SuccessorList::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			delete *j;
		}
	}
}

void LSystem::PerformIteration(const std::string& axiom, std::string& result) const
{
	result.clear();

	for (unsigned int i = 0; i < axiom.size(); ++i)
	{
		bool rulematch = false;
		for (RuleMap::const_iterator j = _rules.begin(); j != _rules.end(); ++j)
		{
			if (axiom.compare(i, j->first.size(), j->first) == 0)
			{
				AppendSuccessor(j->second, result);
				rulematch = true;

				i += j->first.size() - 1;

				break;
			}
		}

		if (!rulematch)
		{
			result.append(1, axiom[i]);
		}
	}
}

void LSystem::AppendSuccessor(const SuccessorList& successors, std::string& result) const
{
	if (successors.size() == 1)
	{
		(*successors.begin())->AppendSelf(result);
		return;
	}
	
	float totalWeighting = 0;
	for (SuccessorList::const_iterator i = successors.begin(); i != successors.end(); ++i)
	{
		totalWeighting += (*i)->ProbabilityWeight();
	}

	float randomNumber = totalWeighting * (float)rand() / RAND_MAX;
	
	float currentProbability = 0;

	for (SuccessorList::const_iterator i = successors.begin(); i != successors.end(); ++i)
	{
		currentProbability += (*i)->ProbabilityWeight();

		if (randomNumber <= currentProbability)
		{
			(*i)->AppendSelf(result);
			return;
		}
	}
}

LSystem::Successor::Successor(const std::string& successor, float probabilityWeight) :
	_successor(successor),
	_probabilityWeight(probabilityWeight)
{

}

void LSystem::Successor::AppendSelf(std::string& string) const
{
	string.append(_successor);
}