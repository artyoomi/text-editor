#include "Rope/Rope.h"

#include <stdexcept>

// out-of-bounds error constant
#define E_OOB_ROPE "Index out of range"

// Construct a Rope from the given std::string
Rope::Rope(const std::string& str) { this->_root = std::make_unique<RNode>(str); }

// Copy constructor
Rope::Rope(const Rope& r)
{
	RNode new_root = RNode(*r._root);
	this->_root = std::make_unique<RNode>(new_root);
}

// Get the std::string stored in the Rope
std::string Rope::to_string(void) const
{
	if (this->_root == nullptr)
		return "";
	return this->_root->to_string();
}

// Get the length of the stored std::string
std::size_t Rope::length(void) const
{
	if (this->_root == nullptr)
		return 0;
	return this->_root->length();
}

// Get the character at the given position in the represented std::string
char Rope::at(std::size_t index) const
{
	if(this->_root == nullptr)
		throw std::invalid_argument(E_OOB_ROPE);
	return (*(this->_root))[index];
}

// Return the substd::string of length (len) beginning at the specified index
std::string Rope::substr(std::size_t start, std::size_t len) const
{
	std::size_t full_length = this->length();
	
	if (start > full_length || start + len > full_length)
		throw std::invalid_argument(E_OOB_ROPE);
	
	return this->to_string().substr(start, len);
	//return this->_root->substr(start, len);
}

// Insert the given std::string into the Rope, beginning at the specified index (i)
void Rope::insert(std::size_t i, const std::string& str) { this->insert(i, Rope(str)); }

// Insert the given Rope into the Rope, beginning at the specified index (i)
void Rope::insert(size_t i, const Rope& r)
{
	if (this->length() < i) {
		throw std::out_of_range(E_OOB_ROPE);
	} else {
		Rope tmp = Rope(r);
		std::pair<RNode_uptr, RNode_uptr> orig_rope_split = split_at(std::move(this->_root), i);
		RNode_uptr tmp_concat = std::make_unique<RNode>(std::move(orig_rope_split.first),
		                                                std::move(tmp._root));
		this->_root = std::make_unique<RNode>(std::move(tmp_concat),
		                                      std::move(orig_rope_split.second));
	}
}

// Append the argument to the existing Rope
void Rope::append(const std::string& str)
{
	Rope tmp = Rope(str);
	this->_root = std::make_unique<RNode>(std::move(this->_root),
	                                      std::move(tmp._root));
}

// Append the argument to the existing Rope
void Rope::append(const Rope& r)
{
	Rope tmp = Rope(r);
	this->_root = std::make_unique<RNode>(std::move(this->_root),
	                                      std::move(tmp._root));
}

// Delete the substd::string of (len) characters beginning at index (start)
void Rope::rdelete(size_t start, size_t len)
{
	std::size_t full_length = this->length();
	
	if (start > full_length || start + len > full_length) {
		throw std::out_of_range(E_OOB_ROPE);
	} else {
    	std::pair<RNode_uptr, RNode_uptr> first_split = split_at(std::move(this->_root), start);
    	std::pair<RNode_uptr, RNode_uptr> second_split = split_at(move(first_split.second), len);
    	
    	second_split.first.reset();
    	
    	this->_root = std::make_unique<RNode>(std::move(first_split.first),
    	                                      std::move(second_split.second));
  	}
}

// Determine if Rope is balanced
//
// A Rope is balanced if and only if its length is greater than or equal to
//   fib(d+2) where d refers to the depth of the Rope and fib(n) represents
//   the n-th fibonacci number i.e. in the set {1,1,2,3,5,8,etc...}
bool Rope::is_balanced(void) const
{
	if(this->_root == nullptr)
		return true;
  	
  	std::size_t d = this->_root->depth();
  	return this->length() >= fibonacci(d + 2);
}

// Balance a Rope
void Rope::balance(void)
{
	// initiate rebalancing only if Rope is unbalanced
	if (this->is_balanced())
		return;
	
	// build vector representation of Fibonacci intervals
	std::vector<std::size_t> intervals = build_fibonacci_vec(this->length());
	std::vector<RNode_uptr> nodes(intervals.size());

	// get leaf nodes
	std::vector<RNode*> leaves;
	this->_root->get_leaves(leaves);

	size_t i;
    size_t max_i = intervals.size()-1;
    size_t currMaxInterval = 0;
    RNode_uptr acc = nullptr;
    RNode_uptr tmp = nullptr;

    // attempt to insert each leaf into nodes vector based on length
    for (auto& leaf : leaves) {
    	std::size_t len = leaf->length();
    	bool inserted = false;
  
    	// ignore empty leaf nodes
    	if (len > 0) {
    		acc = std::make_unique<RNode>(*leaf);
    		i = 0;

        	while(!inserted) {
          		// find appropriate slot for the acc node to be inserted,
          		// concatenating with nodes encountered along the way
          		while(i < max_i && len >= intervals[i+1]) {
            		if(nodes[i].get() != nullptr) {
              			// concatenate encountered entries with node to be inserted
              			tmp = std::make_unique<RNode>(*nodes[i].get());
              			acc = std::make_unique<RNode>(*acc.get());
              			acc = std::make_unique<RNode>(move(tmp),move(acc));
          
              			// update len
              			len = acc->length();
          
              			// if new length is sufficiently great that the node must be
              			//   moved to a new slot, we clear out the existing entry
              			if(len >= intervals[i+1]) nodes[i] = nullptr;
            		}
            	
            		i++;
          		}
      
          		// target slot found -- check if occupied
          		if (nodes[i].get() == nullptr) {
            		nodes[i].swap(acc);
            		inserted = true;
            		
            		// update currMaxInterval if necessary
            		if(i > currMaxInterval)
            			currMaxInterval = i;
          		} else {
            		// concatenate encountered entries with node to be inserted
		            tmp = std::make_unique<RNode>(*nodes[i].get());
		            acc = std::make_unique<RNode>(*acc.get());
		            acc = std::make_unique<RNode>(move(tmp),move(acc));
        
		            // update len
		            len = acc->length();
        
		            // if new length is sufficiently great that the node must be
		            //   moved to a new slot, we clear out the existing entry
		            if(len >= intervals[i + 1])
		            	nodes[i] = nullptr;
      			}
    		}
  		}
	}

	// concatenate remaining entries to produce balanced Rope
	acc = move(nodes[currMaxInterval]);
	for(int idx = currMaxInterval; idx >= 0; idx--) {
	  	if(nodes[idx] != nullptr) {
	    	tmp = std::make_unique<RNode>(*nodes[idx].get());
	    	acc = std::make_unique<RNode>(move(acc),move(tmp));
	  	}
	}
	
	this->_root = move(acc); // reset root
}

// Assignment operator
Rope& Rope::operator=(const Rope& rhs)
{
	// check for self-assignment
	if(this == &rhs)
  		return *this;
  	
  	// delete existing Rope to recover memory
  	this->_root.reset();
  	
  	// invoke copy constructor
  	this->_root = std::make_unique<RNode>(*(rhs._root.get()));
  	
  	return *this;
}

// Determine if two Ropes contain identical std::strings
bool Rope::operator ==(const Rope& rhs) const
{
  return this->to_string() == rhs.to_string();
}

// Determine if two Ropes contain identical std::strings
bool Rope::operator !=(const Rope& rhs) const
{
  return !(*this == rhs);
}

// Print the Rope
std::ostream& operator<<(std::ostream& out, const Rope& r)
{
  return out << r.to_string();
}


// Compute the nth Fibonacci number, in O(n) time
std::size_t fibonacci(std::size_t n)
{
	// initialize first two numbers in sequence
  	int a = 0, b = 1, next;
  	
  	if(n == 0)
  		return a;
  	
  	for (std::size_t i = 2; i <= n; i++) {
    	next = a + b;
    	a = b;
    	b = next;
  	}
  	
  	return b;
};

// Construct a vector where the n-th entry represents the interval between the
// Fibonacci numbers F(n + 2) and F(n + 3), and the final entry includes the number
// specified in the length parameter
// e.g. buildFibList(0) -> {}
//      buildFibList(1) -> {[1,2)}
//      buildFibList(8) -> {[1,2),[2,3),[3,5),[5,8),[8,13)}
std::vector<std::size_t> build_fibonacci_vec(std::size_t len)
{
	// initialize a and b to the first and second fib numbers respectively
  	int a = 0, b = 1, next;
  	std::vector<std::size_t> intervals = std::vector<std::size_t>();
  	
  	while (a <= len) {
    	if (a > 0) {
      		intervals.push_back(b);
    	}
    	
    	next = a + b;
    	a = b;
    	b = next;
  	}
  	
  	return intervals;
}

// Find all occurences of s in t
std::vector<int> rabin_karp(std::string const& substr, std::string const& text)
{
    const int p = 31;           // prime digit for calculations
    const int m = 1e9 + 9;
    int S = substr.size(), T = text.size();

	// calculate pows vector
    std::vector<long long> p_pow(std::max(S, T)); 
    p_pow[0] = 1; 
    for (std::size_t i = 1; i < (int)p_pow.size(); i++) 
        p_pow[i] = (p_pow[i-1] * p) % m;

    std::vector<long long> h(T + 1, 0); 
    for (int i = 0; i < T; i++)
        h[i+1] = (h[i] + (text[i] - 'a' + 1) * p_pow[i]) % m; 
    long long h_s = 0; 
    for (int i = 0; i < S; i++) 
        h_s = (h_s + (substr[i] - 'a' + 1) * p_pow[i]) % m; 

    std::vector<int> occurrences;
    for (int i = 0; i + S - 1 < T; i++) {
        long long cur_h = (h[i+S] + m - h[i]) % m;
        if (cur_h == h_s * p_pow[i] % m)
            occurrences.push_back(i);
    }
    return occurrences;
}