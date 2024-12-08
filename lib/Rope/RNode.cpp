#include "Rope/RNode.h"

#include <stdexcept>

using RNode_uptr = RNode::RNode_uptr;

// define out-of-bounds error string
#define E_OOB_NODE "Index out of bounds"

// Construct internal node by concatenating the given nodes
RNode::RNode(RNode_uptr l, RNode_uptr r)
    : _substr("")
{
    this->_left   = move(l);
    this->_right  = move(r);
    this->_weight = this->_left->length();
}

// Construct leaf node from the given string
RNode::RNode(const std::string& str)
	: _weight(str.length()), _left(nullptr), _right(nullptr), _substr(str) {}

// Copy constructor
RNode::RNode(const RNode& another)
	: _weight(another._weight), _substr(another._substr)
{
	RNode *tmp_left  = another._left.get();
	RNode *tmp_right = another._right.get();

	this->_left  = (tmp_left  == nullptr) ? (nullptr) : (std::make_unique<RNode>(*tmp_left));
	this->_right = (tmp_right == nullptr) ? (nullptr) : (std::make_unique<RNode>(*tmp_right));
}

char RNode::operator[](const std::size_t index) const { return _get_char_by_index(index); }

// Determine whether a node is a leaf
bool RNode::_is_leaf(void) const { return (this->_left == nullptr) && (this->_right == nullptr); }

// Get string length by adding the weight of the root and all nodes in
//   path to rightmost child
std::size_t RNode::length() const
{
	if (this->_is_leaf())
		return this->_weight;
	
	return this->_weight +
	       ((this->_right == nullptr) ? (0) : (this->_right->length()));
}

// Get the character at the given index
char RNode::_get_char_by_index(std::size_t index) const
{
	size_t curr_weight = this->_weight;

  	// if node is a leaf, return the character at the specified index
  	if (this->_is_leaf()) {
    	if (index >= this->_weight)
      		throw std::invalid_argument(E_OOB_NODE);
    	else
     		return this->_substr[index];
     		
  	// else search the appropriate node
  	} else {
    	if (index < curr_weight)
      		return this->_left->_get_char_by_index(index);
    	else
      		return this->_right->_get_char_by_index(index - curr_weight);
  	}
}

// Get the substring of (len) chars beginning at index (start)
std::string RNode::substr(std::size_t start, std::size_t len) const
{
	std::size_t curr_weight = this->_weight;
  	
  	if (this->_is_leaf()) {
    	if(len < curr_weight)
      		return this->_substr.substr(start, len);
    	else
      		return this->_substr;

	} else {
		// check if start index in left subtree
    	if (start < curr_weight) {
    		std::string l_result = (this->_left == nullptr) ? ("") : (this->_left->substr(start,len));
    	
    		if (start + len > curr_weight) {
        		// get number of characters in left subtree
        		size_t tmp =  curr_weight - start;
        		std::string r_result = (this->_right == nullptr) ? ("") : (this->_right->substr(curr_weight, len-tmp));
        		return l_result.append(r_result);
      		} else
        		return l_result;

    	// if start index is in the right subtree...
    	} else {
    		return (this->_right == nullptr) ? ("") : (this->_right->substr(start - curr_weight, len));
		}
	}
}

// Get string contained in current node and its children
std::string RNode::to_string(void) const
{
	if (this->_is_leaf())
		return this->_substr;

	std::string l_result = (this->_left == nullptr) ? ("") : (this->_left->to_string());
	std::string r_result = (this->_right == nullptr) ? ("") : (this->_right->to_string());
  
	return l_result.append(r_result);
}

// Split the represented std::string at the specified index
std::pair<RNode_uptr, RNode_uptr> split_at(RNode_uptr node, std::size_t index)
{
  std::size_t curr_weight = node->_weight;
  
	// if the given node is a leaf, split the leaf
	if (node->_is_leaf()) {
		return std::pair<RNode_uptr,RNode_uptr>{
    		std::make_unique<RNode>(node->_substr.substr(0, index)),
      		std::make_unique<RNode>(node->_substr.substr(index, curr_weight - index))
    	};
	}

	// if the given node is a concat (internal) node, compare index to weight
	// and RNode_uptr accordingly
	RNode_uptr old_right = std::move(node->_right);
	
	if (index < curr_weight) {
		node->_right = nullptr;
		node->_weight = index;
		
		std::pair<RNode_uptr, RNode_uptr> split_left_result = split_at(std::move(node->_left), index);
		node->_left = move(split_left_result.first);
    	
    	return std::pair<RNode_uptr,RNode_uptr>
    	{
      		   move(node),
      		   std::make_unique<RNode>(move(split_left_result.second), move(old_right))
    	};
	} else if (curr_weight < index) {
		std::pair<RNode_uptr, RNode_uptr> split_right_result = split_at(move(old_right), index - curr_weight);
		node->_right = move(split_right_result.first);
    	return std::pair<RNode_uptr,RNode_uptr>
    	{
               std::move(node),
               std::move(split_right_result.second)
    	};
	} else {
		return std::pair<RNode_uptr,RNode_uptr>{
      		   std::move(node->_left),
      		   std::move(old_right)
    	};
	}
}

// Get the maximum depth of the rope, where the depth of a leaf is 0 and the
//   depth of an internal node is 1 plus the max depth of its children
std::size_t RNode::depth(void) const
{
	if(this->_is_leaf())
		return 0;
	
	std::size_t l_result = (this->_left == nullptr)  ? (0) : (this->_left->depth());
	std::size_t r_result = (this->_right == nullptr) ? (0) : (this->_right->depth());
	
	return std::max(++l_result, ++r_result);
}

// Store all leaves in the given vector
void RNode::get_leaves(std::vector<RNode *>& v)
{
	if (this->_is_leaf()) {
		v.push_back(this);
  	} else {
    	RNode *tmp_left = this->_left.get();
    	if (tmp_left != nullptr)
    		tmp_left->get_leaves(v);
    
    	RNode *tmp_right = this->_right.get();
    	if (tmp_right != nullptr)
    		tmp_right->get_leaves(v);
	}
}

