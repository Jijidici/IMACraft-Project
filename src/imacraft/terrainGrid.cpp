#include<iostream>
#include<cstdlib>
#include<string.h>
#include <stdint.h>
#include <stdio.h>

#include"imacraft/terrainGrid.hpp"

namespace imacraft{

	bool TerrainGrid::readFile(const char* fileName){

		FILE *rDataFile = NULL;
		char path[80] = "terrain_data/";
		strcat(path,fileName);

		rDataFile = fopen(path, "rb");
		if(NULL == rDataFile){
			std::cout << "[!] > Unable to read the terrain_data file" << std::endl;
			return EXIT_FAILURE;
		}

		size_t test_fic = 0;
		
		test_fic = fread(&m_width, sizeof(uint16_t), 1, rDataFile);

		m_data = new uint8_t[m_width];
		test_fic = fread(m_data, m_width*sizeof(uint8_t), 1, rDataFile);

		fclose(rDataFile);

		if(!test_fic) return false;
		else return true;
	}

	bool TerrainGrid::writeFile(const char* fileName){

		FILE *wDataFile = NULL;
		char path[80] = "terrain_data/";
		strcat(path,fileName);

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

}
