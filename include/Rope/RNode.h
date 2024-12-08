#ifndef RNODE_H
#define RNODE_H

#include <string>
#include <memory>
#include <vector>

// A RNode represents a string as a binary tree of string substrings
//
// A RNode consists of:
//   - a non-negative integer weight
//   - a pointer to a left child RNode
//   - a pointer to a right child RNode
//   - a string fragment

class RNode {
public:
	using RNode_uptr = std::unique_ptr<RNode>;

	// construct leaf node from the given string
	RNode(const std::string &str);

	// construct internal node by concatenating the given nodes
	RNode(RNode_uptr l, RNode_uptr r);

	// copy constructor
	RNode(const RNode &another);

	// indexing operator
	char operator[](const std::size_t index) const;

	// return length of subtree with root in *this
	std::size_t length(void) const;

	// get the substring of (len) chars beginning at index (start)
	std::string substr(std::size_t start, std::size_t len) const;

	// get string contained in current node and its children
	std::string to_string(void) const;

	// split the represented string at the specified index
	friend std::pair<RNode_uptr, RNode_uptr> split_at(RNode_uptr node, std::size_t index);

	// functions used in balancingw
	std::size_t depth(void) const;
	void get_leaves(std::vector<RNode *> &v);
private:
	char _get_char_by_index(std::size_t) const;

private:
	// determine whether a node is a leaf
	bool _is_leaf(void) const;

	std::size_t _weight;
	
	RNode_uptr  _left;
	RNode_uptr  _right;
	
	std::string _substr;

};

#endif // RNODE_H
