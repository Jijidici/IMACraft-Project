#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <cstdlib>
#include <vector>

#include "imacraft/TerrainGrid.hpp"

bool loadGrids(int northPos, int eastPos, std::vector<imacraft::TerrainGrid*> &vecGrid);

#endif //_TOOLS_HPP_
