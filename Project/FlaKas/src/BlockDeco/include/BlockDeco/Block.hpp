#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <set>

namespace BlockDecoBlock {

struct Block {
  std::size_t size() const {return VariableIndices.size(); }
  std::set<unsigned> VariableIndices;
  std::set<unsigned> EquationIndices;
};

}

#endif // BLOCK_HPP
