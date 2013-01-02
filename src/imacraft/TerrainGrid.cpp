#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include"imacraft/TerrainGrid.hpp"

namespace imacraft{
	
	TerrainGrid::TerrainGrid(){
		m_data = NULL;
		m_width = 0;
		m_northPosition = 0;
		m_eastPosition = 0;
	}
	
	TerrainGrid::~TerrainGrid(){
		if(NULL != m_data){ delete[] m_data; } 
	}
	
	uint8_t TerrainGrid::operator[](size_t idx) const{
		return m_data[idx];
	}
	
	uint8_t& TerrainGrid::operator[](size_t idx){
		return m_data[idx];
	}
	
	bool TerrainGrid::readFile(const char* fileName){
		FILE *rDataFile = NULL;
		char path[80] = "terrain_data/";
		strcat(path,fileName);

		std::string stringToCompare = fileName;
		size_t findNorth = 0;
		size_t northCount = -1;
		size_t findSouth = 0;
		size_t southCount = -1;
		size_t findEast = 0;
		size_t eastCount = -1;
		size_t findWest = 0;
		size_t westCount = -1;
		
		while(findNorth != std::string::npos){
			findNorth = stringToCompare.find("N", findNorth+1);
			++northCount;
			//~ if(findNorth != std::string::npos){ // if found
				//~ std::cout << "position N : " << findNorth << std::endl;
			//~ }
		}
		m_northPosition += northCount;
		//~ std::cout << "northCount : " << northCount << std::endl;
		
		while(findSouth != std::string::npos){
			findSouth = stringToCompare.find("S", findSouth+1);
			++southCount;
		}
		m_northPosition -= southCount;
		
		while(findEast != std::string::npos){
			findEast = stringToCompare.find("E", findEast+1);
			++eastCount;
		}
		m_eastPosition += eastCount;
		
		while(findWest != std::string::npos){
			findWest = stringToCompare.find("W", findWest+1);
			++westCount;
		}
		m_eastPosition -= westCount;
		
		rDataFile = fopen(path, "rb");
		if(NULL == rDataFile){
			std::cout << "[!] > Unable to read the terrain_data file" << std::endl;
			return EXIT_FAILURE;
		}

		size_t test_fic = 0;
		
		test_fic = fread(&m_width, sizeof(uint16_t), 1, rDataFile);
		
		m_data = new uint8_t[m_width*m_width*TERRAIN_HEIGHT];
		test_fic = fread(m_data, m_width*m_width*TERRAIN_HEIGHT*sizeof(uint8_t), 1, rDataFile);
	
		fclose(rDataFile);

		if(!test_fic) return false;
		else return true;
	}

	bool TerrainGrid::writeFile(const char* fileName){

		FILE *wDataFile = NULL;
		char path[80] = "terrain_data/";
		strcat(path,fileName);
		for(int i = 0; i < m_northPosition; ++i){
			strcat(path,"N");
		}
		for(int i = 0; i < -m_northPosition; ++i){
			strcat(path,"S");
		}
		for(int i = 0; i < m_eastPosition; ++i){
			strcat(path,"E");
		}
		for(int i = 0; i < -m_eastPosition; ++i){
			strcat(path,"W");
		}

		wDataFile = fopen(path, "wb");
		if(NULL == wDataFile){
			std::cout << "[!] > Unable to write into the terrain_data file" << std::endl;
			return EXIT_FAILURE;
		}

		size_t test_fic = 0;

		test_fic = fwrite(&m_width, sizeof(uint16_t), 1, wDataFile);
		test_fic = fwrite(m_data, m_width*sizeof(uint8_t), 1, wDataFile);

		fclose(wDataFile);

		if(!test_fic) return false;
		else return true;
	}
	
	uint32_t TerrainGrid::length() const{
		return m_width*m_width*TERRAIN_HEIGHT;
	}
	
	uint16_t TerrainGrid::width() const{
		return m_width;
	}
	
	uint16_t TerrainGrid::height() const{
		return TERRAIN_HEIGHT;
	}
	
	uint16_t TerrainGrid::getNorthPos() const{
		return m_northPosition;
	}
	
	uint16_t TerrainGrid::getEastPos() const{
		return m_eastPosition;
	}

}
