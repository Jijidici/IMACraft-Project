#ifndef __TERRAINGRID_HPP__
#define __TERRAINGRID_HPP__

namespace imacraft{

	struct TerrainGrid{
		static const uint16_t TERRAIN_HEIGHT = 128;
		
		private:
			uint8_t *m_data;
			uint16_t m_width;
			int m_northPosition;
			int m_eastPosition;
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
	};

}

#endif
