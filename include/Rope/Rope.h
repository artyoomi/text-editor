#ifndef ROPE_H
#define ROPE_H

#include "RNode.h"

#include <algorithm>

// A rope represents a string as a binary tree wherein the leaves contain fragments of the
// string. More accurately, a rope consists of a pointer to a root rope_node, which
// describes a binary tree of string fragments.

// Examples:
//
//        X        |  null root pointer, represents empty string
//
//      "txt"      |
//     /     \     |  root is a leaf node containing a string fragment
//    X       X    |
//
//        O        |
//       / \       |
//  "some" "text"  |  root is an internal node formed by the concatenation of two distinct
//    /\     /\    |  ropes containing the strings "some" and "text"
//   X  X   X  X   |

class Rope { 
public:
	using RNode_uptr = std::unique_ptr<RNode>;
  	
  	// Construct a rope from the given string
  	Rope(const std::string& str = "");
  	
  	// Copy constructor
  	Rope(const Rope&);
  
	// Get the string stored in the rope
	std::string to_string(void) const;
	
	// Get the length of the stored string
	std::size_t length(void) const;
	
	// Get the character at the given position in the represented string
	char at(std::size_t index) const;
	
	// Return the substring of length (len) beginning at the specified index
	std::string substr(std::size_t start, std::size_t len) const;
	
	// Determine if rope is balanced
	bool is_balanced(void) const;
	
	// Balance the rope
	void balance(void);

	// Insert the given string/rope into the rope, beginning at the specified index (i)
	void insert(std::size_t i, const std::string& str);
	void insert(std::size_t i, const Rope& r);
	
	// Concatenate the existing string/rope with the argument
	void append(const std::string&);
	void append(const Rope&);
	
	// Delete the substring of (len) characters beginning at index (start)
	void rdelete(std::size_t start, std::size_t len);

	// OPERATORS
	Rope& operator=(const Rope& rhs);
	bool  operator==(const Rope& rhs) const;
	bool  operator!=(const Rope& rhs) const;
	
	friend std::ostream& operator<<(std::ostream& out, const Rope& r);
  
private:
	RNode_uptr _root;
};

std::size_t fibonacci(std::size_t n);
std::vector<std::size_t> build_fibonacci_vec(std::size_t len);
std::vector<int> rabin_karp(std::string const& substr, std::string const& text);

#endif //  ROPE_H
