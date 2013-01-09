#ifndef __TERRAINGRID_HPP__
#define __TERRAINGRID_HPP__

#include <stdint.h>
#include <glm/glm.hpp>

namespace imacraft{

	struct TerrainGrid{
		static const uint16_t TERRAIN_WIDTH = 128;
		static const uint16_t TERRAIN_HEIGHT = 128;
		//STATIC METHOD
		static glm::ivec3 getCubeIntegerPosition(const glm::vec3 position);
		
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
