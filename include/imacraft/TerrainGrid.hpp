#ifndef __TERRAINGRID_HPP__
#define __TERRAINGRID_HPP__

#include <stdint.h>
#include <glm/glm.hpp>

#define CUBE_SIZE 0.015625
#define CENTER 0
#define NORTH 1
#define EAST 3
#define SOUTH 2
#define WEST 4
#define NORTH_EAST 5
#define SOUTH_EAST 6
#define SOUTH_WEST 7
#define NORTH_WEST 8

namespace imacraft{

	struct TerrainGrid{
		static const uint16_t TERRAIN_WIDTH = 128;
		static const uint16_t TERRAIN_HEIGHT = 128;
		//STATIC METHOD
		static glm::ivec3 getCubeIntegerPosition(const glm::vec3 position);
		static glm::vec3 getCubeFloatPosition(const glm::ivec3 cube);
		
		private:
			uint8_t *m_data;
			uint16_t m_width;
			int m_northPosition;
			int m_northRelativePosition;
			int m_eastPosition;
			int m_eastRelativePosition;
		public:
			TerrainGrid();
			~TerrainGrid();
			uint8_t operator[](size_t idx) const;
			uint8_t& operator[](size_t idx);
			bool readFile(const char*);
			bool writeFile(const char*);
			uint32_t length() const;
			uint16_t width() const;
			uint16_t height() const;
			int getNorthPos() const;
			int getEastPos() const;
			void setGridRelativePosition(int north, int east);
	};

}

#endif
