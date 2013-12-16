#if 0
	Copyright 2013 Christopher J. Hanks  
	
	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:
	
	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endif 

#ifndef __MEMORY_MAP_H_
#define __MEMORY_MAP_H_


#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <cassert>
#include <memory>
#include <string>
#include <stdexcept>
#include <sstream>

#include <iostream>

#include "ipc.hh"


namespace ipc {

/*=--------------------------------------------------------------------------=*/
/*= class: Exceptions                                                        =*/
/*=--------------------------------------------------------------------------=*/

class FileIOException : public std::runtime_error {
public:
	FileIOException(const std::string &file_path) 
		: std::runtime_error("FAILED TO OPEN: " + file_path) 
	{}
};

class FilePurityException : public std::runtime_error {
public:
	/* The complicated ostringstream() manipulation here is in order to
	 * construct the std::runtime_error() string from disparate types.
	 */
	FilePurityException(const std::string &file_path,
			    const size_t      &file_size,
			    const size_t      &offset   ,
			    const size_t      &obj_size ) 
		: std::runtime_error(
		    ((std::ostringstream&)
		     (std::ostringstream() << "FILE: " 
			                   << file_path 
			                   << " was non pure by "
			                   << (file_size - offset) % obj_size 
			                   << " bytes ")).str()
			)
	{}
};


/*=--------------------------------------------------------------------------=*/
/*= class: Miterator                                                         =*/
/*=--------------------------------------------------------------------------=*/

template<class _T>
class Miterator : public std::iterator<std::random_access_iterator_tag, _T> { 
public:
	typedef typename
	std::iterator<std::random_access_iterator_tag, 
		_T>::value_type value_type;
	
	typedef typename
	std::iterator<std::random_access_iterator_tag, 
		_T>::difference_type difference_type;
	
	typedef typename
	std::iterator<std::random_access_iterator_tag,
		_T>::pointer pointer;
	
	typedef typename
	std::iterator<std::random_access_iterator_tag, 
		_T>::reference reference;

	/* ------------------------------------------------------------------ */
	
	Miterator()
		:value(0x0)
	{}

	Miterator(pointer value)
		:value(value)
	{
	}

	template<typename _T2>
	Miterator(const Miterator<_T2> &rhs) 
		: value(&(*rhs))
	{
	}

	template<typename _T2>
	Miterator& operator=(const Miterator<_T2> &rhs) {
		this->value = rhs.value;//&(*rhs);
		return *this;
	}
	
	Miterator& operator++() { this->value++; return *this; }
	Miterator& operator--() { this->value--; return *this; }

	Miterator& operator++(int) { return Miterator(this->value++); }
	Miterator& operator--(int) { return Miterator(this->value--); }
	
	Miterator operator+(const difference_type &d) const {
		return Miterator(this->value + d);
	}
	Miterator operator-(const difference_type &d) const {
		return Miterator(pointer(this->value - d));
	}
	
	Miterator operator+=(const difference_type &d) {
		this->value += d;
                return *this;
	}
	Miterator& operator-=(const difference_type &d) {
		this->value += d;
                return *this;
	}

	reference operator*() const {
		return *(this->value);
	}

	pointer operator->() const {
		return this->value;
	}

	reference operator[](const difference_type &d) const {
		return this->value[d];
	}
	
	template<typename _Tp>
	friend bool operator==(const Miterator<_Tp>&, const Miterator<_Tp>&);

	template<typename _Tp>
	friend bool operator!=(const Miterator<_Tp>&, const Miterator<_Tp>&);
	
	template<typename _Tp>
	friend bool operator>=(const Miterator<_Tp>&, const Miterator<_Tp>&);

	template<typename _Tp>
	friend bool operator<=(const Miterator<_Tp>&, const Miterator<_Tp>&);

	template<typename _Tp>
	friend bool operator>(const Miterator<_Tp>&, const Miterator<_Tp>&);

	template<typename _Tp>
	friend bool operator<(const Miterator<_Tp>&, const Miterator<_Tp>&);
	
	
	template<typename _Tp>
	friend typename Miterator<_Tp>::difference_type operator+(
			const Miterator<_Tp>&,
			const Miterator<_Tp>&
			);
	
	template<typename _Tp>
	friend typename Miterator<_Tp>::difference_type operator-(
			const Miterator<_Tp>&,
			const Miterator<_Tp>&
			);

private:
	_T* value;
};


template<typename T>
bool operator==(const Miterator<T> &r1, const Miterator<T> &r2) {
	return r1.value == r2.value;
}

template<typename T>
bool operator!=(const Miterator<T> &r1, const Miterator<T> &r2) {
	return r1.value != r2.value;
}

template<typename T>
bool operator>=(const Miterator<T> &r1, const Miterator<T> &r2) {
	return r1.value >= r2.value;
}

template<typename T>
bool operator<=(const Miterator<T> &r1, const Miterator<T> &r2) {
	return r1.value <= r2.value;
}

template<typename T>
bool operator>(const Miterator<T> &r1, const Miterator<T> &r2) {
	return r1.value > r2.value;
}

template<typename T>
bool operator<(const Miterator<T> &r1, const Miterator<T> &r2) {
	return r1.value < r2.value;
}

template<typename T>
typename Miterator<T>::difference_type operator+(const Miterator<T> &r1,
                                                 const Miterator<T> &r2) {
	return Miterator<T>(r1.value + r2.value);
}

template<typename T>
typename Miterator<T>::difference_type operator-(const Miterator<T> &r1,
                                                 const Miterator<T> &r2) {
	return r1.value - r2.value;
}


/*=--------------------------------------------------------------------------=*/
/*= class: MemoryMap                                                         =*/
/*=--------------------------------------------------------------------------=*/

template<class _T, size_t _HeadSize = 0> 
class MemoryMap {

	/* @class MemoryMap 
	 *
	 *
	 */

public:
	/*
	 *
	 * @param permission 
	 * @param scope      
	 */
	MemoryMap(const ipc::Permission &permission = ipc::ReadWrite, 
	          const ipc::Scope      &scope      = ipc::Shared ) 
		: permission(permission),
		  scope(scope),
		  map_pod(new map_pod_t(), MemoryMap::deleter)
	{
		this->nullify_map_pod();
	}
	

	/* Creates a sparse file which matches the sizeof(_T) * entries +
	 * _HeadSize that will be used for the memory map.
	 *
	 * This presumes the OS supports sparse files.
	 *
	 * @param file_path   Path to use for the memory mapped segment 
	 * @param entries     Number of entries to put into the file.
	 * @param header      The header to append to the file, this /must/ be 
	 *                    the same length as _HeadSize 
	 * @param create_mode File creation octal. 
	 *
	 * @return number of _T entries in file. 
	 */
	size_t create(const std::string &file_path, const size_t &entries,
	              const std::string &header = "", 
	              const int         &create_mode = 0666) {
	
		
		if (header.size() != _HeadSize)
			throw FileIOException("Invalid header size");

		int fd = ::open(file_path.c_str(), O_CREAT | O_WRONLY, 
				create_mode);

		if (fd < 0)
			throw FileIOException("Failed to create file");

		if (_HeadSize != ::write(fd, header.c_str(), _HeadSize))
			throw FileIOException("Failed to write file header");
		
		if (::ftruncate(fd, (sizeof(_T) * entries) + _HeadSize) < 0)
			throw FileIOException("Failed to create sparse file");

		if (::close(fd) < 0)
			throw FileIOException("Failed to close file");
		
		return this->open(file_path); 
	}

	/*
	 */
	size_t open(const std::string &file_path) {
		/* check the file size and confirm the data properly aligns to
		 * the template type.
		 */
		this->map_pod->size = this->file_size(file_path);
		
		if (((this->map_pod->size - _HeadSize) % sizeof(_T)) != 0)
			throw FilePurityException(file_path, 
						  this->map_pod->size, 
						  _HeadSize, 
						  sizeof(_T));
			

		/* open the file in the appropriate permissions */
		this->map_pod->fd = this->open_file(file_path, this->permission);

		if (this->map_pod->fd < 0)
			throw FileIOException(file_path);
		
	
		/* map the file desciptor into the memory map */
		this->map_pod->head = this->map_file(this->map_pod->fd, 
		                                     this->permission,
		                                     this->scope,
		                                     this->map_pod->size);
		this->map_pod->ptr  = this->map_pod->head + _HeadSize;

		return (this->map_pod->size - _HeadSize) / sizeof(_T);
	}
	
	void sync(const bool async = true, const bool invalidate = false) {

		int flags = (async      ? MS_ASYNC      : MS_SYNC) | 
			    (invalidate ? MS_INVALIDATE : 0x0    ) ;
		
		if (msync(this->map_pod->head, this->map_pod->size, flags) < 0)
			throw FileIOException("Failed to sync");
	}

	void close() {
		this->unmap(this->map_pod->head);
		this->close(this->map_pod->fd  );
	}
	
	
	/* ------------------------------------------------------------------ */
	/* ACCESSOR                                                           */ 
	
	template<typename _H = char>
	_H* header() {
		return (_H*)(this->map_pod->head);
	}

	_T& operator[](const size_t &index) {
		return this->map_pod->ptr[index];
	}
	
	_T& operator[](const size_t &index) const {
		return this->map_pod->ptr[index];
	}

	_T& at(const size_t &index) {
		if (index < this->size())
			return this->map_pod->ptr[index];
		else 
			throw std::out_of_range("Out of range");
	}
	
	_T& at(const size_t &index) const {
		if (index < this->size())
			return this->map_pod->ptr[index];
		else 
			throw std::out_of_range("Out of range");
	}
	
	size_t size() const {
		return (this->map_pod->size - _HeadSize) / sizeof(_T);
	}

	
	/* ------------------------------------------------------------------ */
	/* ITERATOR                                                           */ 
	typedef Miterator<_T>       iterator;
	typedef Miterator<const _T> const_iterator;


	iterator begin() {
		return iterator(this->map_pod->ptr);
	}

	iterator end() {
		return iterator(this->map_pod->ptr + this->size());
	}

	iterator const_begin() const {
		return const_iterator(this->map_pod->head);
	}

	iterator const_end() const {
		return const_iterator(this->map_pod->ptr + this->size());
	}

private:
	const ipc::Permission permission;
	const ipc::Scope      scope; 
	
	typedef struct {
		_T*    head; 
		_T*    ptr;
		int    fd;
		size_t size;
	} map_pod_t;

	std::shared_ptr<map_pod_t> map_pod;
	
	/* ------------------------------------------------------------------ */
	
	void nullify_map_pod() {
		this->map_pod->head = 0x0;
		this->map_pod->ptr  = 0x0; 
		this->map_pod->fd   = -1;
	}

	/* ------------------------------------------------------------------ */

	static void deleter(map_pod_t *pod) {
		if (0x0 != pod->ptr) 
			MemoryMap::unmap(pod->ptr, pod->size);

		if (pod->fd > 0)
			MemoryMap::close(pod->fd);

		delete pod;
	}

	static size_t file_size(const std::string &file_path) {
		struct stat stat_fd;

		if (::stat(file_path.c_str(), &stat_fd) < 0)
			return 0;
		else
			return stat_fd.st_size;
	}

	static int open_file(const std::string &file_path, 
			     const ipc::Permission  &permission) {
		
		int fd = -1;

		switch (permission) {
		case ipc::ReadOnly:
			fd = ::open(file_path.c_str(), O_RDONLY);
			break;

		case ipc::WriteOnly:
			fd = ::open(file_path.c_str(), O_WRONLY | O_CREAT);
			break;

		case ipc::ReadWrite:
			fd = ::open(file_path.c_str(), O_RDWR   | O_CREAT);
		}
		
		return fd; 
	}
	
	static _T* map_file(const int              &fd, 
			    const ipc::Permission &permission,
			    const ipc::Scope      &scope, 
			    const size_t           &size ) {
		
		// --
		int prot;

		switch (permission) {
		case ipc::ReadWrite:
			prot = PROT_READ | PROT_WRITE;
			break;

		case ipc::ReadOnly:
			prot = PROT_READ;
			break;

		case ipc::WriteOnly:
			prot = PROT_WRITE;
			break;
		}

		// --	
		int flags;

		switch (scope) {
		case ipc::Private:
			flags = MAP_PRIVATE;
			break;

		case ipc::Shared:
			flags = MAP_SHARED;
			break;
		} 

		// --
		void* ptr = mmap(0x0, size, prot, flags, fd, 0);
		
		if (MAP_FAILED == ptr)
			throw FileIOException("FAILED TO MAP POINTER");

		return static_cast<_T*>(ptr);

	}
	
	static void unmap(_T* ptr, size_t len) {
		assert(0x0 != ptr);
		
		::munmap(ptr, len);
	}

	static void close(int fd) {
		assert(fd >= 0);
		::close(fd);
	}
};

} // ns ipc 

#endif // __MEMORY_MAP_H_
