//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "Node.h"
#include <stdlib.h>

using namespace AI;

Node::Node() :	m_parent(NULL),
				m_cost(0.f),
				m_total(0.f),
				m_onOpen(false),
				m_onClosed(false)
{}
Node::Node(const Node& rhs) : m_parent(rhs.m_parent),
							  m_cost(rhs.m_cost),
							  m_total(rhs.m_total),
							  m_onOpen(rhs.m_onOpen),
							  m_onClosed(rhs.m_onClosed)
{}
Node::~Node() {}

void Node::Reset() {
	m_parent = NULL;
	m_cost   = m_total		= 0.f;
	m_onOpen = m_onClosed	= false;
}

void Node::SetParent(Node *parent) {
	m_parent = parent;
}
const Node* Node::GetParent() const {
	return m_parent;
}

void Node::SetCost(float cost) {
	m_cost = cost;
}
const float Node::GetCost() const {
	return m_cost;
}

void Node::SetTotal(float total) {
	m_total = total;
}
const float Node::GetTotal() const {
	return m_total;
}

void Node::SetOnOpen(bool onOpen) {
	m_onOpen = onOpen;
}
const bool Node::IsOnOpen() const {
	return m_onOpen;
}

void Node::SetOnClosed(bool onClosed) {
	m_onClosed = onClosed;
}
const bool Node::IsOnClosed() const {
	return m_onClosed;
}

bool Node::operator =(AI::Node &rhs) {
	if (&rhs == this)
	{
		return true;
	}
	return false;
}

std::vector<Node *>& Node::GetChildren() 
{
	return m_children;
}
