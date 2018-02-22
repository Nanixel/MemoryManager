#pragma once
#include "./PoolAllocator.h";
#include "./GlobalMemoryUser.h";
#include <vector>
#include <list>
#include <cassert>

namespace Memory {
	
	template<class OBJECT_TYPE, size_t CAPACITY>
	class MemoryChunkAllocator : protected GlobalMemoryUser {
	private:
		static const size_t MAX_OBJECTS = typename CAPACITY;
		static const size_t ALLOC_SIZE = ((sizeof(OBJECT_TYPE) + __alignof(OBJECT_TYPE)) * MAX_OBJECTS);
		const char* _allocatorTag;

	public:

		/// A Memory Chunk is just a wrapper over the memory that is managed by a pool allocator. Its main purpose is to encasulate objects
		class MemoryChunk {

		public:
			PoolAllocator* Allocator;
			uintptr_t StartAddress;
			uintptr_t EndAddress;
			std::vector<OBJECT_TYPE*> Objects;

			MemoryChunk(PoolAllocator* allocator) : Allocator(allocator) {
				StartAddress = reinterpret_cast<uintptr_t>(allocator->getStart());				
				EndAddress = StartAddress + ALLOC_SIZE;
				Objects.clear();
			}

			void RemoveObject(void* addressOfObject) {
				if (Objects.size() >= 2) {					
					std::swap(Objects[FindObjectIndexForAddress(addressOfObject)], Objects.back());
					Objects.pop_back();
				}
				else {
					Objects.clear();
				}
			}

		private:

			int FindObjectIndexForAddress(void* address) {
				for (int i = 0; i < Objects.size() - 1; i++) {
					if (address == &*Objects[i]) {
						return i;
					}
				}
				return 0;
			}			
		};

		using MemoryChunks = std::list<MemoryChunk*>;

		/// To interate over objects stored in memory chunks, we must create a custom iteration since the memory chunks have a limited
		/// capacity.  For instance, if the capacity is 10 items and we have two full memory chunks (making a total of 20 items),
		/// the object iterator will iterate through the first 10 and then must be set to the first item of the second chunk.
		class ChunkIterator : public std::iterator<std::forward_iterator_tag, OBJECT_TYPE> {
			// Typename is needed here because of the subclass "iterator".
			typename MemoryChunks::iterator _currentChunk;
			typename MemoryChunks::iterator _end;
			typename std::vector<OBJECT_TYPE*>::iterator _currentObject;

		public:
			ChunkIterator(typename MemoryChunks::iterator begin, typename MemoryChunks::iterator end) : _currentChunk(begin), _end(end) {
				if (begin != end) {
					assert((*_currentChunk) != nullptr);
					// Set _currentObject to the first item in the current chunk's object list
					_currentObject = (*_currentChunk)->Objects.begin();
				}
				else {
					// Set _currentObject to the last object of the current chunk's object list.
					_currentObject = (*std::prev(_end))->Objects.end();
				}
			}

			inline ChunkIterator& operator++() {

				_currentObject++; // Take a look right here

				if (_currentObject == (*_currentChunk)->Objects.end()) {
					_currentChunk++;
					if (_currentChunk != _end) {
						assert((*_currentChunk) != nullptr);
						_currentObject = (*_currentChunk)->Objects.begin();
					}
				}
				return *this;
			}

			inline OBJECT_TYPE& operator*() const { return *_currentObject; }
			inline OBJECT_TYPE* operator->() const { return *_currentObject; }

			inline bool operator==(ChunkIterator& other) {
				return ((this->_currentChunk == other._currentChunk) && (this->_currentObject == other._currentObject));
			}

			inline bool operator!=(ChunkIterator& other) {
				return ((this->_currentChunk != other._currentChunk) && (this->_currentObject != other._currentObject));
			}
		};

	protected:
		std::list<MemoryChunk*> _memoryChunks;

	public:

		MemoryChunkAllocator(const char* allocatorTag = nullptr) : _allocatorTag(allocatorTag) {
			PoolAllocator* allocator = new PoolAllocator(ALLOC_SIZE, Allocate(allocatorTag, ALLOC_SIZE), sizeof(OBJECT_TYPE), 
				__alignof(OBJECT_TYPE));			
			_memoryChunks.push_back(new MemoryChunk(allocator));
		}

		virtual ~MemoryChunkAllocator() {
			// Release all of the memory in the chunks.
			for (auto chunk : _memoryChunks) {
				for (auto object : chunk->Objects) {
					static_cast<OBJECT_TYPE*>(object)->~OBJECT_TYPE();
				}
				chunk->Objects.clear();

				// Free allocated memory.
				Deallocate((void*)(chunk->StartAddress));
				delete chunk->Allocator;
				chunk->Allocator = nullptr;

				delete chunk;
				chunk = nullptr;
			}
		}
		
		/// Search for a free memory slot and allocate memory for the object type of this chunk allocator.
		void* CreateObject() {			
			void* memorySlot = nullptr;
			for (auto chunk : _memoryChunks) {
				if (chunk->Objects.size() > MAX_OBJECTS) {
					// Continue to the next interation in the loop to find a chunk that is not full.
					continue; 
				}

				memorySlot = chunk->Allocator->allocate(sizeof(OBJECT_TYPE), __alignof(OBJECT_TYPE));
				if (memorySlot != nullptr) {
					chunk->Objects.push_back(static_cast<OBJECT_TYPE*>(memorySlot));
					break;
				}
			}

			// All of the chunks were full.
			if (memorySlot == nullptr) {
				PoolAllocator* allocator = new PoolAllocator(ALLOC_SIZE, Allocate(_allocatorTag, ALLOC_SIZE), sizeof(OBJECT_TYPE),
					__alignof(OBJECT_TYPE));
				MemoryChunk* newChunk = new MemoryChunk(allocator);
				_memoryChunks.push_front(newChunk);
				memorySlot = newChunk->Allocator->allocate(sizeof(OBJECT_TYPE), __alignof(OBJECT_TYPE));

				assert(memorySlot != nullptr && "UNABLE TO ALLOCATE MEMORY FOR SLOT");

				newChunk->Objects.push_back(static_cast<OBJECT_TYPE*>(memorySlot));
			}
			
			return memorySlot;
		}

		/// Find the memory chunk the object exists in and delete that object from the chunk's object list.
		/// In order to determine which chunk contains the object, we just need to check if the objects address is in between
		/// that chunk's start and end addresses
		void DestroyObject(void* object) {
			uintptr_t address = reinterpret_cast<uintptr_t>(object);
						
			for (auto chunk : _memoryChunks) {

				// The object is within this chunk's memory address range.
				if (chunk->StartAddress <= address && address < chunk->EndAddress) {					
					chunk->RemoveObject(object);
					chunk->Allocator->deallocate(object);
					return;						
				}		
			}

			assert(false && "FAILED TO DESTROY OBJECT");						
		}

		///Define a begin and end method for our memory chunk allocator
		inline ChunkIterator begin() { return ChunkIterator(_memoryChunks.begin(), _memoryChunks.end()); }
		inline ChunkIterator end() { return ChunkIterator(_memoryChunks.end(), _memoryChunks.end()); }
	};
}
