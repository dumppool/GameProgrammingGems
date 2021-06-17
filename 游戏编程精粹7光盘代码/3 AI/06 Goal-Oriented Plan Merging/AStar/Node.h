//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef NODE_H
#define NODE_H

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>

namespace AI {
	class Node {
	public:
		Node();
		Node(const Node& rhs);
		virtual ~Node();
		void Reset();
		
		void SetParent(Node *parent);
		const Node* GetParent() const;
		
		void SetCost(float cost);
		const float GetCost() const;
		
		void SetTotal(float total);
		const float GetTotal() const;
		
		void SetOnOpen(bool onOpen);
		const bool IsOnOpen() const;
		
		void SetOnClosed(bool onClosed);
		const bool IsOnClosed() const;

		virtual bool operator=(Node &rhs);
		virtual std::vector<Node *>& GetChildren();
	protected:
		Node*	m_parent;
		float	m_cost;
		float	m_total;
		bool	m_onOpen;
		bool	m_onClosed;
		std::vector<Node *> m_children;
	};
}

#endif //NODE_H