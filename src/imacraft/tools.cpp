#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdint.h>

#include "imacraft/tools.hpp"

#define CENTER 0
#define NORTH 1
#define EAST 3
#define SOUTH 2
#define WEST 4
#define NORTH_EAST 5
#define SOUTH_EAST 6
#define SOUTH_WEST 7
#define NORTH_WEST 8

// load correct grids NESW...
bool loadGrids(int northPos, int eastPos, std::vector<imacraft::TerrainGrid*> &vecGrid){
	char basePath[80] = "terrain_imacraft_";
	char centerPath[80] = "terrain_imacraft_";
	char northPath[80] = "terrain_imacraft_";
	char southPath[80] = "terrain_imacraft_";
	char eastPath[80] = "terrain_imacraft_";
	char westPath[80] = "terrain_imacraft_";
	char northEastPath[80] = "terrain_imacraft_";
	char southEastPath[80] = "terrain_imacraft_";
	char southWestPath[80] = "terrain_imacraft_";
	char northWestPath[80] = "terrain_imacraft_";
	
	//center
	for(int i = 0; i < northPos; ++i)
		strcat(centerPath,"N");
	for(int i = 0; i < -northPos; ++i)
		strcat(centerPath,"S");
	for(int i = 0; i < eastPos; ++i)
		strcat(centerPath,"E");
	for(int i = 0; i < -eastPos; ++i)
		strcat(centerPath,"W");

	strcat(centerPath,".data");
	
	//north
	for(int i = 0; i < northPos+1; ++i)
		strcat(northPath,"N");
	for(int i = 0; i < -(northPos+1); ++i)
		strcat(northPath,"S");
	for(int i = 0; i < eastPos; ++i)
		strcat(northPath,"E");
	for(int i = 0; i < -eastPos; ++i)
		strcat(northPath,"W");

	strcat(northPath,".data");
	
	//south
	for(int i = 0; i < northPos-1; ++i)
		strcat(southPath,"N");
	for(int i = 0; i < -(northPos-1); ++i)
		strcat(southPath,"S");
	for(int i = 0; i < eastPos; ++i)
		strcat(southPath,"E");
	for(int i = 0; i < -eastPos; ++i)
		strcat(southPath,"W");

	strcat(southPath,".data");
	
	//east
	for(int i = 0; i < northPos; ++i)
		strcat(eastPath,"N");
	for(int i = 0; i < -(northPos); ++i)
		strcat(eastPath,"S");
	for(int i = 0; i < eastPos+1; ++i)
		strcat(eastPath,"E");
	for(int i = 0; i < -(eastPos+1); ++i)
		strcat(eastPath,"W");

	strcat(eastPath,".data");
	
	//west
	for(int i = 0; i < northPos; ++i)
		strcat(westPath,"N");
	for(int i = 0; i < -(northPos); ++i)
		strcat(westPath,"S");
	for(int i = 0; i < eastPos-1; ++i)
		strcat(westPath,"E");
	for(int i = 0; i < -(eastPos-1); ++i)
		strcat(westPath,"W");

	strcat(westPath,".data");
	
	//northEast
	for(int i = 0; i < northPos+1; ++i)
		strcat(northEastPath,"N");
	for(int i = 0; i < -(northPos+1); ++i)
		strcat(northEastPath,"S");
	for(int i = 0; i < eastPos+1; ++i)
		strcat(northEastPath,"E");
	for(int i = 0; i < -(eastPos+1); ++i)
		strcat(northEastPath,"W");

	strcat(northEastPath,".data");
	
	//southEast
	for(int i = 0; i < northPos-1; ++i)
		strcat(southEastPath,"N");
	for(int i = 0; i < -(northPos-1); ++i)
		strcat(southEastPath,"S");
	for(int i = 0; i < eastPos+1; ++i)
		strcat(southEastPath,"E");
	for(int i = 0; i < -(eastPos+1); ++i)
		strcat(southEastPath,"W");

	strcat(southEastPath,".data");
	
	//southWest
	for(int i = 0; i < northPos-1; ++i)
		strcat(southWestPath,"N");
	for(int i = 0; i < -(northPos-1); ++i)
		strcat(southWestPath,"S");
	for(int i = 0; i < eastPos-1; ++i)
		strcat(southWestPath,"E");
	for(int i = 0; i < -(eastPos-1); ++i)
		strcat(southWestPath,"W");

	strcat(southWestPath,".data");
	
	//northWest
	for(int i = 0; i < northPos+1; ++i)
		strcat(northWestPath,"N");
	for(int i = 0; i < -(northPos+1); ++i)
		strcat(northWestPath,"S");
	for(int i = 0; i < eastPos-1; ++i)
		strcat(northWestPath,"E");
	for(int i = 0; i < -(eastPos-1); ++i)
		strcat(northWestPath,"W");

	strcat(northWestPath,".data");
	
	std::cout << centerPath << std::endl;
	std::cout << northPath << std::endl;
	std::cout << southPath << std::endl;
	std::cout << eastPath << std::endl;
	std::cout << westPath << std::endl;
	std::cout << northEastPath << std::endl;
	std::cout << southEastPath << std::endl;
	std::cout << southWestPath << std::endl;
	std::cout << northWestPath << std::endl;
	
	if(!(*vecGrid[CENTER]).readFile(centerPath)) return false;
	if(!(*vecGrid[NORTH]).readFile(northPath)) return false;
	if(!(*vecGrid[SOUTH]).readFile(southPath)) return false;
	if(!(*vecGrid[EAST]).readFile(eastPath)) return false;
	if(!(*vecGrid[WEST]).readFile(westPath)) return false;
	if(!(*vecGrid[NORTH_EAST]).readFile(northEastPath)) return false;
	if(!(*vecGrid[SOUTH_EAST]).readFile(southEastPath)) return false;
	if(!(*vecGrid[SOUTH_WEST]).readFile(southWestPath)) return false;
	if(!(*vecGrid[NORTH_WEST]).readFile(northWestPath)) return false;

	(*vecGrid[CENTER]).setGridRelativePosition(0,0);
	(*vecGrid[NORTH]).setGridRelativePosition(1,0);
	(*vecGrid[SOUTH]).setGridRelativePosition(-1,0);
	(*vecGrid[EAST]).setGridRelativePosition(0,1);
	(*vecGrid[WEST]).setGridRelativePosition(0,-1);
	(*vecGrid[NORTH_EAST]).setGridRelativePosition(1,1);
	(*vecGrid[SOUTH_EAST]).setGridRelativePosition(-1,1);
	(*vecGrid[SOUTH_WEST]).setGridRelativePosition(-1,-1);
	(*vecGrid[NORTH_WEST]).setGridRelativePosition(1,-1);
	
	return true;
}
