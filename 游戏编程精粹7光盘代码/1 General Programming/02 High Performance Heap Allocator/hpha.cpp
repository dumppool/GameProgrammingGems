// Copyright (c) 2007, Dimitar Lazarov, Luxoflux
// dimitar.lazarov@usa.net

#include "hpha.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

//////////////////////////////////////////////////////////////////////////
void intrusive_multi_rbtree_base::insert_fixup(node_base* node) {
	node_base* cur = node;
	node_base* p = cur->parent();
	while (p->red()) {
		node_base* pp = p->parent();
		assert(pp != &mHead);
		side s = (side)p->parent_side();
		side o = (side)(1 - s);
		node_base* pp_right = pp->child(o);
		if (pp_right->red()) {
			p->make_black();
			pp_right->make_black();
			pp->make_red();
			cur = pp;
			p = cur->parent();
		} else {
			if (cur == p->child(o)) {
				cur = p;
				cur->rotate(s);
				p = cur->parent();
			}
			p->make_black();
			pp->make_red();
			pp->rotate(o);
		} 
	}
	mHead.child(LEFT)->make_black();
}

void intrusive_multi_rbtree_base::erase_fixup(node_base* node) {
	node_base* cur = node;
	while (!cur->red() && cur != mHead.child(LEFT)) {
		node_base* p = cur->parent();
		side s = (side)cur->parent_side();
		side o = (side)(1 - s);
		node_base* w = p->child(o);
		assert(w != &mHead);
		if (w->red()) {
			assert(w->child(LEFT)->black() && w->child(RIGHT)->black());
			w->make_black();
			p->make_red();
			w = w->child(s);
			p->rotate(s);
		}
		assert(w != &mHead);
		if (w->child(LEFT)->black() && w->child(RIGHT)->black()) { 
			w->make_red();
			cur = p;
		} else {
			if (w->child(o)->black()) {
				w->child(s)->make_black();
				w->make_red();
				node_base* c = w->child(s);
				w->rotate(o);
				w = c;
				assert(w != &mHead);
			}
			assert(w->child(o)->red());
			w->make_red_black(p->red_black());
			p->make_black();
			w->child(o)->make_black();
			p->rotate(s);
			cur = mHead.child(LEFT);
		}
	}
	cur->make_black();
}

#ifdef DEBUG_MULTI_RBTREE
unsigned intrusive_multi_rbtree_base::check_height(node_base* node) const {
	if (node == &mHead)
		return 0;
	if (node->black())
		return check_height(node->child(LEFT)) + check_height(node->child(RIGHT)) + 1;
	assert(node->child(LEFT)->black() && node->child(RIGHT)->black());
	unsigned lh = check_height(node->child(LEFT));
	unsigned rh = check_height(node->child(RIGHT));
	assert(lh == rh);
	return lh;
}

void intrusive_multi_rbtree_base::check() const {
	assert(mHead.black());
	assert(mHead.child(RIGHT) == &mHead);
	assert(mHead.child(LEFT) == &mHead || mHead.child(LEFT)->black());
	check_height(mHead.child(LEFT));
}
#endif


//////////////////////////////////////////////////////////////////////////
allocator::allocator() : mMRFreeBlock(0) {
	#ifdef DEBUG_ALLOCATOR
	mTotalRequestedSizeBuckets = 0;
	mTotalRequestedSizeTree = 0;
	#endif
	mTotalAllocatedSizeBuckets = 0;
	mTotalAllocatedSizeTree = 0;
}

allocator::~allocator() {
	purge();
	// print any remaining allocated blocks
	#ifdef DEBUG_ALLOCATOR
	check();
	report();
	#endif
	// if any of these assert it must be a leak
	for (unsigned i = 0; i < NUM_BUCKETS; i++)
		assert(mBuckets[i].page_list_empty());
	assert(mFreeTree.empty());
	assert(mSmallFreeList.empty());
	assert(mMRFreeBlock == NULL);
}

allocator::bucket::bucket() 
	#ifdef MULTITHREADED
	: mLock(SPIN_COUNT)
	#endif
{
	// generate a random marker to be hashed with the page info address
	// on Win32 rand max is merely 16 bit so we use two random values
	#if (RAND_MAX <= SHRT_MAX)
	mMarker = (rand()*(RAND_MAX+1) + rand()) ^ MARKER;
	#else
	mMarker = rand() ^ MARKER;
	#endif
}

allocator::page* allocator::bucket::get_free_page() {
	if (!mPageList.empty()) {
		page* p = &mPageList.front();
		if (p->mFreeList)
			return p;
	}
	return NULL;
}

void* allocator::bucket::alloc(page* p) {
	// get an element from the free list
	assert(p && p->mFreeList);
	p->inc_ref();
	free_link* free = p->mFreeList;
	free_link* next = free->mNext;
	p->mFreeList = next;
	if (!next) {
		// if full, auto sort to back
		p->unlink();
		mPageList.push_back(p);
	}
	return (void*)free;
}

void allocator::bucket::free(page* p, void* ptr) {
	// add the element back to the free list
	free_link* free = p->mFreeList;
	free_link* lnk = (free_link*)ptr;
	lnk->mNext = free;
	p->mFreeList = lnk;
	p->dec_ref();
	if (!free) {
		// if the page was previously full, auto sort to front
		p->unlink();
		mPageList.push_front(p);
	}
}

void* allocator::bucket_system_alloc() {
	void* ptr = virtual_alloc(PAGE_SIZE);
	if (ptr) {
		// page address must be naturally aligned
		assert(((size_t)ptr & (PAGE_SIZE-1)) == 0);
		#ifdef MULTITHREADED
		mutex::scoped lock(mTreeMutex);
		#endif
		mTotalAllocatedSizeBuckets += PAGE_SIZE;
	}
	return ptr;
}

void allocator::bucket_system_free(void* ptr) {
	assert(ptr);
	virtual_free(ptr);
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	mTotalAllocatedSizeBuckets -= PAGE_SIZE;
}

allocator::page* allocator::bucket_grow(size_t elemSize, unsigned marker) {
	// make sure mUseCount won't overflow
	assert((PAGE_SIZE-sizeof(page))/elemSize <= USHRT_MAX);
	if (void* mem = bucket_system_alloc()) {
		// build the free list inside the new page
		// the page info sits at the end of the page
		// most of the time it hides in the alignment remainder and ends up costing no memory
		size_t i = 0;
		size_t n = ((PAGE_SIZE-sizeof(page))/elemSize)*elemSize;
		for (; i < n-elemSize; i += elemSize)
			((free_link*)((char*)mem + i))->mNext = (free_link*)((char*)mem + i + elemSize);
		((free_link*)((char*)mem + i))->mNext = NULL;
		assert(i + elemSize + sizeof(page) <= PAGE_SIZE);
		page* p = ptr_get_page(mem);
		new (p) page((free_link*)mem, (unsigned short)elemSize, marker);
		return p;
	}
	return NULL;
}

void* allocator::bucket_alloc(size_t size) {
	assert(size <= MAX_SMALL_ALLOCATION);
	unsigned bi = bucket_spacing_function(size);
	assert(bi < NUM_BUCKETS);
	#ifdef MULTITHREADED
	mutex::scoped lock(mBuckets[bi].get_lock());
	#endif
	// get the page info and check if there's any available elements
	page* p = mBuckets[bi].get_free_page();
	if (!p) {
		// get a page from the OS, initialize it and add it to the list
		size_t bsize = bucket_spacing_function_inverse(bi);
		p = bucket_grow(bsize, mBuckets[bi].marker());
		if (!p)
			return NULL;
		mBuckets[bi].add_free_page(p);
	}
	assert(p->elem_size() >= size);
	return mBuckets[bi].alloc(p);
}

void* allocator::bucket_alloc_direct(unsigned bi) {
	assert(bi < NUM_BUCKETS);
	#ifdef MULTITHREADED
	mutex::scoped lock(mBuckets[bi].get_lock());
	#endif
	page* p = mBuckets[bi].get_free_page();
	if (!p) {
		size_t bsize = bucket_spacing_function_inverse(bi);
		p = bucket_grow(bsize, mBuckets[bi].marker());
		if (!p)
			return NULL;
		mBuckets[bi].add_free_page(p);
	}
	return mBuckets[bi].alloc(p);
}

void* allocator::bucket_realloc(void* ptr, size_t size) {
	page* p = ptr_get_page(ptr);
	size_t elemSize = p->elem_size();
	if (size <= elemSize)
		return ptr;		
	void* newPtr = bucket_alloc(size);
	if (!newPtr)
		return NULL;
	memcpy(newPtr, ptr, elemSize - MEMORY_GUARD_SIZE);
	bucket_free(ptr);
	return newPtr;
}

void allocator::bucket_free(void* ptr) {
	page* p = ptr_get_page(ptr);
	unsigned bi = p->bucket_index();
	assert(bi < NUM_BUCKETS);
	#ifdef MULTITHREADED
	mutex::scoped lock(mBuckets[bi].get_lock());
	#endif
	mBuckets[bi].free(p, ptr);
}

void allocator::bucket_free_direct(void* ptr, unsigned bi) {
	assert(bi < NUM_BUCKETS);
	page* p = ptr_get_page(ptr);
	// if this asserts, the free size doesn't match the allocated size
	// most likely a class needs a base virtual destructor
	assert(bi == p->bucket_index());
	#ifdef MULTITHREADED
	mutex::scoped lock(mBuckets[bi].get_lock());
	#endif
	mBuckets[bi].free(p, ptr);
}

void allocator::bucket_purge() {
	for (unsigned i = 0; i < NUM_BUCKETS; i++) {
		#ifdef MULTITHREADED
		mutex::scoped lock(mBuckets[i].get_lock());
		#endif
		page *pageEnd = mBuckets[i].page_list_end();
		for (page* p = mBuckets[i].page_list_begin(); p != pageEnd; ) {
			// early out if we reach fully occupied page (the remaining should all be full)
			if (p->mFreeList == NULL) 
				break;
			page* next = p->next();
			if (p->empty()) {
				assert(p->mFreeList);
				p->unlink();
				void* memAddr = align_down((char*)p, PAGE_SIZE);
				bucket_system_free(memAddr);
			}
			p = next;
		}
	}
}

void allocator::split_block(block_header* bl, size_t size) {
	assert(size + sizeof(block_header) + sizeof(free_node) <= bl->size());
	block_header* newBl = (block_header*)((char*)bl + size + sizeof(block_header));
	newBl->link_after(bl);
	newBl->set_unused();
}

allocator::block_header* allocator::shift_block(block_header* bl, size_t offs) {
	assert(offs > 0);
	block_header* prev = bl->prev();
	bl->unlink();
	bl = (block_header*)((char*)bl + offs);
	bl->link_after(prev);
	bl->set_unused();
	return bl;
}

allocator::block_header* allocator::coalesce_block(block_header* bl) {
	assert(!bl->used());
	block_header* next = bl->next();
	if (!next->used()) {
		tree_detach(next);
		next->unlink();
	}
	block_header* prev = bl->prev();
	if (!prev->used()) {
		tree_detach(prev);
		bl->unlink();
		bl = prev;
	}
	return bl;
}

void* allocator::tree_system_alloc(size_t size) {
	assert(size/PAGE_SIZE*PAGE_SIZE == size);
	void* ptr = virtual_alloc(size);
	if (ptr)
		mTotalAllocatedSizeTree += size;
	return ptr;
}

void allocator::tree_system_free(void* ptr, size_t size) {
	assert(ptr);
	assert(size/PAGE_SIZE*PAGE_SIZE == size);
	virtual_free(ptr);
	mTotalAllocatedSizeTree -= size;
}

allocator::block_header* allocator::tree_add_block(void* mem, size_t size) {
	// create a dummy block to avoid prev() NULL checks and allow easy block shifts
	// potentially this dummy block might grow (due to shift_block) but not more than sizeof(free_node)
	block_header* front = (block_header*)mem;
	front->prev(0);
	front->size(0);
	front->set_used();
	block_header* back = (block_header*)front->mem();
	back->prev(front);
	back->size(0);
	back->set_used();
	// now the real free block
	front = back;
	assert(front->used());
	back = (block_header*)((char*)mem + size - sizeof(block_header));
	back->size(0);
	back->set_used();
	front->set_unused();
	front->next(back);
	back->prev(front);
	front = coalesce_block(front);     
	return front;
}

allocator::block_header* allocator::tree_grow(size_t size) {
	size += 3*sizeof(block_header); // two fences plus one fake
	size = round_up(size, PAGE_SIZE);
	if (void* mem = tree_system_alloc(size))
		return tree_add_block(mem, size);
	return NULL;
}

allocator::block_header* allocator::tree_extract(size_t size) {
	// first check the most recently used block
	block_header* bestBlock = mMRFreeBlock;
	if (bestBlock && bestBlock->size() >= size) {
		tree_detach(bestBlock);
		return bestBlock;
	}
	// search the tree and get the smallest fitting block
	free_node* bestNode = mFreeTree.lower_bound(size);
	if (bestNode == mFreeTree.end())
		return NULL;
	bestNode = bestNode->next(); // improves removal time
	bestBlock = bestNode->get_block();
	tree_detach(bestBlock);
	return bestBlock;
}

allocator::block_header* allocator::tree_extract_aligned(size_t size, size_t alignment) {
	block_header* bestBlock = mMRFreeBlock;
	if (bestBlock) {
		size_t alignmentOffs = align_up((char*)bestBlock->mem(), alignment) - (char*)bestBlock->mem();
		if (bestBlock->size() >= size + alignmentOffs) {
			tree_detach(bestBlock);
			return bestBlock;
		}
	}
	// get the sequence of nodes from size to (size + alignment - 1) including
	size_t sizeUpper = size + alignment;
	free_node* bestNode = mFreeTree.lower_bound(size);
	free_node* lastNode = mFreeTree.upper_bound(sizeUpper);
	while (bestNode != lastNode) {
		size_t alignmentOffs = align_up((char*)bestNode, alignment) - (char*)bestNode;
		if (bestNode->get_block()->size() >= size + alignmentOffs)
			break;
		// keep walking the sequence till we find a match or reach the end
		// the larger the alignment the more linear searching will be done
		bestNode = bestNode->succ();
	}
	if (bestNode == mFreeTree.end())
		return NULL;
	if (bestNode == lastNode)
		bestNode = bestNode->next(); // improves removal time
	bestBlock = bestNode->get_block();
	tree_detach(bestBlock);
	return bestBlock;
}

void allocator::tree_attach(block_header* bl) {
	if (mMRFreeBlock) {
		block_header* lastBl = mMRFreeBlock;
		if (lastBl->size() > MAX_SMALL_ALLOCATION) {
			mFreeTree.insert((free_node*)lastBl->mem());
		} else {
			mSmallFreeList.push_back((small_free_node*)lastBl->mem());
		}
	}
	mMRFreeBlock = bl;
}

void allocator::tree_detach(block_header* bl) {
	if (mMRFreeBlock == bl) {
		mMRFreeBlock = NULL;
		return;
	}
	if (bl->size() > MAX_SMALL_ALLOCATION) {
		mFreeTree.erase((free_node*)bl->mem());
	} else {
		mSmallFreeList.erase((small_free_node*)bl->mem());
	}
}

void* allocator::tree_alloc(size_t size) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	// modify the size to make sure we can fit the block header and free node
	if (size < sizeof(free_node))
		size = sizeof(free_node);
	size = round_up(size, sizeof(block_header));
	// extract a block from the tree if found
	block_header* newBl = tree_extract(size);
	if (!newBl) {
		// ask the OS for more memory
		newBl = tree_grow(size);
		if (!newBl)
			return NULL;
	}
	// chop off any remainder
	assert(newBl && newBl->size() >= size);
	if (newBl->size() >= size + sizeof(block_header) + sizeof(free_node)) {
		split_block(newBl, size);
		tree_attach(newBl->next());
	}
	newBl->set_used();
	return newBl->mem();
}

void* allocator::tree_alloc_aligned(size_t size, size_t alignment) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	if (size < sizeof(free_node))
		size = sizeof(free_node);
	size = round_up(size, sizeof(block_header));
	block_header* newBl = tree_extract_aligned(size, alignment);
	if (!newBl) {
		newBl = tree_grow(size + alignment);
		if (!newBl)
			return NULL;
	}
	assert(newBl && newBl->size() >= size);
	size_t alignmentOffs = align_up((char*)newBl->mem(), alignment) - (char*)newBl->mem();
	assert(newBl->size() >= size + alignmentOffs);
	if (alignmentOffs >= sizeof(block_header) + sizeof(free_node)) {
		split_block(newBl, alignmentOffs - sizeof(block_header));
		tree_attach(newBl);
		newBl = newBl->next();
	} else if (alignmentOffs > 0) {
		newBl = shift_block(newBl, alignmentOffs);
	}
	if (newBl->size() >= size + sizeof(block_header) + sizeof(free_node)) {
		split_block(newBl, size);
		tree_attach(newBl->next());
	}
	newBl->set_used();
	assert(((size_t)newBl->mem() & (alignment-1)) == 0);
	return newBl->mem();
}

void* allocator::tree_realloc(void* ptr, size_t size) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	if (size < sizeof(free_node))
		size = sizeof(free_node);
	size = round_up(size, sizeof(block_header));
	block_header* bl = ptr_get_block_header(ptr); 
	size_t blSize = bl->size();
	if (blSize >= size) {
		// the block is being shrunk, truncate and mark the remainder as free
		if (blSize >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			block_header* next = bl->next();
			next = coalesce_block(next);
			tree_attach(next);
		}
		assert(bl->size() >= size);
		return ptr;
	}
	// check if the following block is free and if it can accomodate the requested size
	block_header* next = bl->next();
	size_t nextSize = next->used() ? 0 : next->size() + sizeof(block_header);
	if (blSize + nextSize >= size) {
		assert(!next->used());
		tree_detach(next);
		next->unlink();
		assert(bl->size() >= size);
		if (bl->size() >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			tree_attach(bl->next());
		}
		return ptr;
	}
	// check if the previous block can be used to avoid searching
	block_header* prev = bl->prev();
	size_t prevSize = prev->used() ? 0 : prev->size() + sizeof(block_header);
	if (blSize + prevSize + nextSize >= size) {
		assert(!prev->used());
		tree_detach(prev);
		bl->unlink();
		if (!next->used()) {
			tree_detach(next);
			next->unlink();
		}
		bl = prev;
		bl->set_used();
		assert(bl->size() >= size);
		void* newPtr = bl->mem();
		// move the memory before we split the block
		memmove(newPtr, ptr, blSize - MEMORY_GUARD_SIZE);
		if (bl->size() >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			tree_attach(bl->next());
		}
		return newPtr;
	}
	// fall back to alloc/copy/free
	void* newPtr = tree_alloc(size);
	if (newPtr) {
		memcpy(newPtr, ptr, blSize - MEMORY_GUARD_SIZE);
		tree_free(ptr);
		return newPtr;
	}
	return NULL;
}

void* allocator::tree_realloc_aligned(void* ptr, size_t size, size_t alignment) {
	assert(((size_t)ptr & (alignment-1)) == 0);
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	if (size < sizeof(free_node))
		size = sizeof(free_node);
	size = round_up(size, sizeof(block_header));
	block_header* bl = ptr_get_block_header(ptr);
	size_t blSize = bl->size();
	if (blSize >= size) {
		if (blSize >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			block_header* next = bl->next();
			next = coalesce_block(next);
			tree_attach(next);
		}
		assert(bl->size() >= size);
		return ptr;
	}
	block_header* next = bl->next();
	size_t nextSize = next->used() ? 0 : next->size() + sizeof(block_header);
	if (blSize + nextSize >= size) {
		assert(!next->used());
		tree_detach(next);
		next->unlink();
		assert(bl->size() >= size);
		if (bl->size() >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			tree_attach(bl->next());
		}
		return ptr;
	}
	block_header* prev = bl->prev();
	size_t prevSize = prev->used() ? 0 : prev->size() + sizeof(block_header);
	size_t alignmentOffs = prev->used() ? 0 : align_up((char*)prev->mem(), alignment) - (char*)prev->mem();
	if (blSize + prevSize + nextSize >= size + alignmentOffs) {
		assert(!prev->used());
		tree_detach(prev);
		bl->unlink();
		if (!next->used()) {
			tree_detach(next);
			next->unlink();
		}
		if (alignmentOffs >= sizeof(block_header) + sizeof(free_node)) {
			split_block(prev, alignmentOffs - sizeof(block_header));
			tree_attach(prev);
			prev = prev->next();
		} else if (alignmentOffs > 0) {
			prev = shift_block(prev, alignmentOffs);
		}
		bl = prev;
		bl->set_used();
		assert(bl->size() >= size && ((size_t)bl->mem() & (alignment-1)) == 0);
		void* newPtr = bl->mem();
		memmove(newPtr, ptr, blSize - MEMORY_GUARD_SIZE);
		if (bl->size() >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			tree_attach(bl->next());
		}
		return newPtr;
	}
	void* newPtr = tree_alloc_aligned(size, alignment);
	if (newPtr) {
		memcpy(newPtr, ptr, blSize - MEMORY_GUARD_SIZE);
		tree_free(ptr);
		return newPtr;
	}
	return NULL;
}

size_t allocator::tree_resize(void* ptr, size_t size) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	if (size < sizeof(free_node))
		size = sizeof(free_node);
	size = round_up(size, sizeof(block_header));
	block_header* bl = ptr_get_block_header(ptr); 
	size_t blSize = bl->size();
	if (blSize >= size) {
		if (blSize >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			block_header* next = bl->next();
			next = coalesce_block(next);
			tree_attach(next);
		}
		assert(bl->size() >= size);
		return bl->size();
	}
	block_header* next = bl->next();
	if (!next->used() && blSize + next->size() + sizeof(block_header) >= size) {
		tree_detach(next);
		next->unlink();
		if (bl->size() >= size + sizeof(block_header) + sizeof(free_node)) {
			split_block(bl, size);
			tree_attach(bl->next());
		}
		assert(bl->size() >= size);
	}
	return bl->size();
}

void allocator::tree_free(void* ptr) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	block_header* bl = ptr_get_block_header(ptr);
	bl->set_unused();
	bl = coalesce_block(bl);
	tree_attach(bl);
}

void allocator::tree_purge_block(block_header* bl) {
	assert(!bl->used());
	assert(bl->prev() && bl->prev()->used());
	assert(bl->next() && bl->next()->used());
	if (bl->prev()->prev() == NULL && bl->next()->size() == 0) {
		tree_detach(bl);
		char* memStart = (char*)bl->prev();
		char* memEnd = (char*)bl->mem() + bl->size() + sizeof(block_header);
		void* mem = memStart;
		size_t size = memEnd - memStart;
		assert(((size_t)mem & (PAGE_SIZE-1)) == 0);
		assert((size & (PAGE_SIZE-1)) == 0);
		tree_system_free(mem, size);
	}
}

void allocator::tree_purge() {
	#ifdef MULTITHREADED
	mutex::scoped lock(mTreeMutex);
	#endif
	// purge MR block
	tree_attach(NULL);
	// look for pages that are at least PAGE_SIZE big, minus two fences and 
	// one fake block (up to sizeof(block)+sizeof(free_node))
	size_t pageSize = PAGE_SIZE-3*sizeof(block_header)-sizeof(free_node);
	free_node* node = mFreeTree.lower_bound(pageSize);
	free_node* end = mFreeTree.end();
	while (node != end) {
		block_header* cur = node->get_block();
		node = node->succ();
		tree_purge_block(cur);
	}
	tree_attach(NULL);
}

#ifdef DEBUG_ALLOCATOR
void allocator::debug_record::record_stack() {
	// record the names of the top MAX_CALLSTACK_DEPTH function names
	// this is usually very system specific so here we just clear all
	for (unsigned i = 0; i < MAX_CALLSTACK_DEPTH; i++)
		mCallstack[i] = NULL;
}

void allocator::debug_record::write_guard() {
	unsigned char guardByte = (unsigned char)rand();
	unsigned char* guard = (unsigned char*)mPtr+mSize;
	mGuardByte = guardByte;
	for (unsigned i = 0; i < MEMORY_GUARD_SIZE; i++)
		guard[i] = guardByte++;
}

bool allocator::debug_record::check_guard() const {
	unsigned char guardByte = mGuardByte;
	unsigned char* guard = (unsigned char*)mPtr+mSize;
	for (unsigned i = 0; i < MEMORY_GUARD_SIZE; i++)
		if (guardByte++ != guard[i])
			return false;
	return true;
}

void allocator::debug_record_map::initial_fill(void* ptr, size_t size) {
	unsigned char sFiller[] = {0xFF,0xC0,0xC0,0xFF}; // QNAN (little OR big endian)
	unsigned char* p = (unsigned char*)ptr;
	for (size_t s = 0; s < size; s++) 
		p[s] = sFiller[s % sizeof(sFiller)/sizeof(sFiller[0])];
}

bool allocator::debug_record_map::add(void* ptr, size_t size, debug_source source) {
	// make sure this record is unique
	assert(this->find(ptr) == this->end());
	if (!mRecords.push_back(debug_record(ptr, size, source)))
		return false;
	debug_record* record = &mRecords.back();
	this->insert(record);
	initial_fill(ptr, size);
	return true;
}

allocator::debug_info allocator::debug_record_map::remove(void* ptr) {
	iterator it = this->find(ptr);
	// if this asserts most likely the pointer was already deleted
	// or the pointer points to a static or a global variable.
	assert(it != this->end());
	debug_record* record = it;
	// if this asserts then the memory was corrupted past the end of the block
	assert(record->check_guard());
	size_t size = record->size();
	debug_source source = record->source();
	initial_fill(ptr, record->size());
	this->erase(record);
	debug_record* lastRecord = &mRecords.back();
	if (record != lastRecord) {
		this->erase(lastRecord);
		*record = *lastRecord;
		this->insert(record);
	}
	mRecords.pop_back();
	return debug_info(size, source);
}

allocator::debug_info allocator::debug_record_map::remove(void* ptr, size_t size) {
	iterator it = this->find(ptr);
	// if this asserts most likely the pointer was already deleted
	// or the pointer points to a static or a global variable.
	assert(it != this->end());
	debug_record* record = it;
	// make sure the free size matches the allocation size
	assert(size == record->size());
	// if this asserts then the memory was corrupted past the end of the block
	assert(record->check_guard());
	debug_source source = record->source();
	initial_fill(ptr, record->size());
	this->erase(record);
	debug_record* lastRecord = &mRecords.back();
	if (record != lastRecord) {
		this->erase(lastRecord);
		*record = *lastRecord;
		this->insert(record);
	}
	mRecords.pop_back();
	return debug_info(size, source);
}

allocator::debug_info allocator::debug_record_map::replace(void* ptr, void* newPtr, size_t size, debug_source source) {
	iterator it = this->find(ptr);
	// if this asserts most likely the pointer was already deleted
	assert(it != this->end());
	debug_record* record = it;
	size_t origSize = record->size();
	debug_source origSource = record->source();
	this->erase(record);
	*record = debug_record(newPtr, size, source);
	this->insert(record);
	return debug_info(origSize, origSource);
}

allocator::debug_info allocator::debug_record_map::update(void* ptr, size_t size) {
	iterator it = this->find(ptr);
	// if this asserts most likely the pointer was already deleted
	assert(it != this->end());
	debug_record* record = it;
	size_t origSize = record->size();
	record->set_size(size);
	record->record_stack();
	record->write_guard();
	return debug_info(origSize, record->source());
}

void allocator::debug_record_map::check(void* ptr) const {
	const_iterator it = this->find(ptr);
	// if this asserts most likely the pointer was already deleted
	assert(it != this->end());
	const debug_record* record = it;
	// if this asserts then the memory was corrupted past the end of the block
	assert(record->check_guard());
}

void allocator::debug_record_map::purge() {
	mRecords.purge();
}

void* allocator::debug_add(void* ptr, size_t size, debug_source source) {
	if (!ptr)
		return NULL;
	{
		#ifdef MULTITHREADED
		mutex::scoped lock(mDebugMutex);
		#endif
		assert(size <= this->size(ptr));
		if (mDebugMap.add(ptr, size, source)) {
			if (source == DEBUG_SOURCE_BUCKETS)
				mTotalRequestedSizeBuckets += size + MEMORY_GUARD_SIZE;
			else
				mTotalRequestedSizeTree += size + MEMORY_GUARD_SIZE;
			return ptr;
		}
	}
	if (source == DEBUG_SOURCE_BUCKETS)
		bucket_free(ptr);
	else
		tree_free(ptr);
	return NULL;
}

void allocator::debug_remove(void* ptr) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mDebugMutex);
	#endif
	debug_info info = mDebugMap.remove(ptr);
	if (info.source == DEBUG_SOURCE_BUCKETS)
		mTotalRequestedSizeBuckets -= info.size + MEMORY_GUARD_SIZE;
	else
		mTotalRequestedSizeTree -= info.size + MEMORY_GUARD_SIZE;
}

void allocator::debug_remove(void* ptr, size_t size) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mDebugMutex);
	#endif
	debug_info info = mDebugMap.remove(ptr, size);
	if (info.source == DEBUG_SOURCE_BUCKETS)
		mTotalRequestedSizeBuckets -= info.size + MEMORY_GUARD_SIZE;
	else
		mTotalRequestedSizeTree -= info.size + MEMORY_GUARD_SIZE;
}

void allocator::debug_replace(void* ptr, void* newPtr, size_t size, debug_source source) {
	if (!newPtr)
		return;
	#ifdef MULTITHREADED
	mutex::scoped lock(mDebugMutex);
	#endif
	assert(size <= this->size(newPtr));
	debug_info info = mDebugMap.replace(ptr, newPtr, size, source);
	if (info.source == DEBUG_SOURCE_BUCKETS)
		mTotalRequestedSizeBuckets -= info.size + MEMORY_GUARD_SIZE;
	else
		mTotalRequestedSizeTree -= info.size + MEMORY_GUARD_SIZE;
	if (source == DEBUG_SOURCE_BUCKETS)
		mTotalRequestedSizeBuckets += size + MEMORY_GUARD_SIZE;
	else
		mTotalRequestedSizeTree += size + MEMORY_GUARD_SIZE;
}

void allocator::debug_update(void* ptr, size_t size) {
	#ifdef MULTITHREADED
	mutex::scoped lock(mDebugMutex);
	#endif
	assert(size <= this->size(ptr));
	debug_info info = mDebugMap.update(ptr, size);
	if (info.source == DEBUG_SOURCE_BUCKETS)
		mTotalRequestedSizeBuckets += size - info.size;
	else
		mTotalRequestedSizeTree += size - info.size;
}

void allocator::debug_check(void* ptr) {
	mDebugMap.check(ptr);
}

void allocator::debug_purge() {
	#ifdef MULTITHREADED
	mutex::scoped lock(mDebugMutex);
	#endif
	mDebugMap.purge();
}

void allocator::check() {
	#ifdef MULTITHREADED
	mutex::scoped lock(mDebugMutex);
	#endif
	for (debug_record_map::const_iterator it = mDebugMap.begin(); it != mDebugMap.end(); ++it) {
		assert(it->size() <= size(it->ptr()));
		assert(it->check_guard());
	}
}

void allocator::report() {
	#ifdef MULTITHREADED
	mutex::scoped lock(mDebugMutex);
	#endif
	printf("REPORT =================================================\n");
	printf("Total requested size=%i bytes\n", mTotalRequestedSizeBuckets + mTotalRequestedSizeTree);
	printf("Total allocated size=%i bytes\n", mTotalAllocatedSizeBuckets + mTotalAllocatedSizeTree);
	printf("Currently allocated blocks:\n");
	for (debug_record_map::const_iterator it = mDebugMap.begin(); it != mDebugMap.end(); ++it) {
		printf("ptr=%X, size=%i", (size_t)it->ptr(), it->size());
		for (unsigned i = 0; i < debug_record::MAX_CALLSTACK_DEPTH; i++) {
			if (it->stack(i))
				printf(", %s", it->stack(i));
		}
		printf("\n");
	}
	printf("===========================================================\n");
}
#endif // DEBUG_ALLOCATOR

