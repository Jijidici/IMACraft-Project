#include "imacraft/TerrainGrid.hpp"

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <glm/glm.hpp>

namespace imacraft{
	
	TerrainGrid::TerrainGrid(){
		m_data = NULL;
		m_northPosition = 0;
		m_eastPosition = 0;
		m_northRelativePosition = 0;
		m_eastRelativePosition = 0;
	}
	
	TerrainGrid::~TerrainGrid(){
		if(NULL != m_data){ delete[] m_data; } 
	}
	
	uint8_t TerrainGrid::operator[](size_t idx) const{
		return m_data[idx].type;
	}
	
	uint8_t& TerrainGrid::operator[](size_t idx){
		return m_data[idx].type;
	}
	
	bool TerrainGrid::hidden(size_t idx) const{
		return m_data[idx].hidden;
	}
	
	void TerrainGrid::unhideArroundCubes(size_t idx){
		int k = idx/(TERRAIN_HEIGHT*TERRAIN_WIDTH);
		int reste = idx%(TERRAIN_HEIGHT*TERRAIN_WIDTH);
		int j = reste/TERRAIN_WIDTH;
		int i = reste%TERRAIN_WIDTH;
		if(k+1 < 32){ m_data[(k+1)*TERRAIN_HEIGHT*TERRAIN_WIDTH+j*TERRAIN_WIDTH+i].hidden = false; }
		if(k-1 >= 0){ m_data[(k-1)*TERRAIN_HEIGHT*TERRAIN_WIDTH+j*TERRAIN_WIDTH+i].hidden = false; }
		if(j+1 < 32){ m_data[k*TERRAIN_HEIGHT*TERRAIN_WIDTH+(j+1)*TERRAIN_WIDTH+i].hidden = false; }
		if(j-1 >= 0){ m_data[k*TERRAIN_HEIGHT*TERRAIN_WIDTH+(j-1)*TERRAIN_WIDTH+i].hidden = false; }
		if(i+1 < 32){ m_data[k*TERRAIN_HEIGHT*TERRAIN_WIDTH+j*TERRAIN_WIDTH+i+1].hidden = false; }
		if(i-1 >= 0){ m_data[k*TERRAIN_HEIGHT*TERRAIN_WIDTH+j*TERRAIN_WIDTH+i-1].hidden = false; }
	}
	
	bool TerrainGrid::readFile(const char* fileName){
		FILE *rDataFile = NULL;
		char path[80] = "terrain_data/";
		strcat(path,fileName);

		std::string stringToCompare = fileName;
		size_t findNorth = 0;
		int northCount = -1;
		size_t findSouth = 0;
		int southCount = -1;
		size_t findEast = 0;
		int eastCount = -1;
		size_t findWest = 0;
		int westCount = -1;
		
		while(findNorth != std::string::npos){
			findNorth = stringToCompare.find("N", findNorth+1);
			++northCount;
		}
		m_northPosition = northCount;
		
		while(findSouth != std::string::npos){
			findSouth = stringToCompare.find("S", findSouth+1);
			++southCount;
		}
		m_northPosition -= southCount;
		
		while(findEast != std::string::npos){
			findEast = stringToCompare.find("E", findEast+1);
			++eastCount;
		}
		m_eastPosition = eastCount;
		
		while(findWest != std::string::npos){
			findWest = stringToCompare.find("W", findWest+1);
			++westCount;
		}
		m_eastPosition -= westCount;
		
		rDataFile = fopen(path, "rb");
		if(NULL == rDataFile){
			std::cout << "Created : " << path << std::endl;
			delete[] m_data;
			
			m_data = randomGen();
			
		}else{
			size_t test_fic = 0;
			
			uint16_t tempFAKE;
			
			test_fic = fread(&tempFAKE, sizeof(uint16_t), 1, rDataFile);
			
			delete[] m_data;
			m_data = new Bloc[TERRAIN_WIDTH*TERRAIN_WIDTH*TERRAIN_HEIGHT];
			test_fic = fread(m_data, TERRAIN_WIDTH*TERRAIN_WIDTH*TERRAIN_HEIGHT*sizeof(Bloc), 1, rDataFile);
		
			fclose(rDataFile);
			
			std::cout << "Read : " << path << std::endl;
			
			if(!test_fic) return false;
			else return true;
		}
		
		return true;
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
		
		strcat(path,".data");

		wDataFile = fopen(path, "wb");
		if(NULL == wDataFile){
			std::cout << "[!] > Unable to write into the terrain_data file" << std::endl;
			return EXIT_FAILURE;
		}

		size_t test_fic = 0;
		

		uint16_t tempFAKE = TERRAIN_WIDTH;

		test_fic = fwrite(&tempFAKE, sizeof(uint16_t), 1, wDataFile);
		
		test_fic = fwrite(m_data, TERRAIN_WIDTH*TERRAIN_WIDTH*TERRAIN_HEIGHT*sizeof(Bloc), 1, wDataFile);
		
		fclose(wDataFile);

		if(!test_fic) return false;
		else return true;
	}
	
	uint32_t TerrainGrid::length() const{
		return TERRAIN_WIDTH*TERRAIN_WIDTH*TERRAIN_HEIGHT;
	}
	
	uint16_t TerrainGrid::width() const{
		return TERRAIN_WIDTH;
	}
	
	uint16_t TerrainGrid::height() const{
		return TERRAIN_HEIGHT;
	}
	
	int TerrainGrid::getNorthPos() const{
		return m_northRelativePosition;
	}
	
	int TerrainGrid::getEastPos() const{
		return m_eastRelativePosition;
	}
	
	void TerrainGrid::setGridRelativePosition(int north, int east){
		m_northRelativePosition = north;
		m_eastRelativePosition = east;
	}
	
	void TerrainGrid::removeCube(glm::vec3 fPos){
		glm::ivec3 iPos = getCubeIntegerPosition(fPos);
		m_data[iPos.z*TERRAIN_HEIGHT*TERRAIN_WIDTH + iPos.y*TERRAIN_WIDTH + iPos.x].type = 0;
	}
	
	void TerrainGrid::addCube(glm::vec3 fPos, uint8_t cubeType){
		glm::ivec3 iPos = getCubeIntegerPosition(fPos);
		m_data[iPos.z*TERRAIN_HEIGHT*TERRAIN_WIDTH + iPos.y*TERRAIN_WIDTH + iPos.x].type = cubeType;
	}
	
	
	//STATIC METHOD
	glm::ivec3 TerrainGrid::getCubeIntegerPosition(const glm::vec3 position){
		int i = (position.x+1.f)/2.f * TERRAIN_WIDTH;
		int j = (position.y+1.f)/2.f * TERRAIN_HEIGHT;
		int k = (position.z+1.f)/2.f * TERRAIN_WIDTH;
		return glm::ivec3(i, j, k);
	}
	
	glm::vec3 TerrainGrid::getCubeFloatPosition(const glm::ivec3 cube){
		float x = (cube.x*CUBE_SIZE)-1.;
		float y = (cube.y*CUBE_SIZE)-1.;
		float z = (cube.z*CUBE_SIZE)-1.;
		return glm::vec3(x, y, z);
	}
	
	Bloc* TerrainGrid::randomGen(){
		int i,j,k,l,lift = 0;
		// TERRAIN_WIDTH, TERRAIN_HEIGHT
		uint16_t w = 32;
		uint16_t h = 32;
		Bloc* data = new Bloc[w*w*h];
		srand(time(NULL));
		
		// Generate the cube and randomly move it up or down
		j=15;
		for(k=0;k<=31;++k){
			for(i=0;i<=31;++i){
				lift = rand() %100;
				if(lift>98 && j<31){
					++j;
				}

				if(lift<=1 && j>0){
					--j;
				}
				data[k*w*h + j*w + i].type=1;
			}
		}
		
		// Fill lower cubes
		for(k=0;k<=31;++k){
			for(i=0;i<=31;++i){
				for(j=31;j>=0;--j){
					if(data[k*w*h + j*w + i].type != 0){
						// Fill the two following cubes with type 1
						for(l=j;l>=j-2;--l){
							data[k*w*h + l*w + i].type=1;
						}
						// Fill the following cubes with type 2
						for(l=j-3;l>=0;--l){
							if (l<=1){
								data[k*w*h + l*w + i].type=4;
							}else{
								data[k*w*h + l*w + i].type=2;
							}
						}
						break;
					}
				}
			}
		}
	
		//set the hidden property
		for(k=1;k<31;++k){
			for(j=1;j<31;++j){
				for(i=1;i<31;++i){
					if(data[(k+1)*w*h + j*w + i].type !=0 &&
					   data[(k-1)*w*h + j*w + i].type !=0 &&
					   data[k*w*h + (j+1)*w + i].type !=0 &&
					   data[k*w*h + (j-1)*w + i].type !=0 &&
					   data[k*w*h + j*w + i+1].type !=0 &&
					   data[k*w*h + j*w + i-1].type !=0){
						data[k*w*h + j*w + i].hidden = true;
					}
				}
			}
		}
		return data;
	}
}


