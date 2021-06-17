//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "Storage.h"

using namespace AI;

Storage::Storage()  {}
Storage::~Storage() {}

bool Storage::IsOpenListEmpty() const { return false; }
void Storage::PushOntoClosed(Node *node) {}
void Storage::PushOntoOpen(Node *node) {}
void Storage::UpdateNodeOnOpen(Node *node) {}
Node* Storage::PopBestNodeFromOpen() { return NULL; }
Node* Storage::GetLastNodeOnClosed() { return NULL; }