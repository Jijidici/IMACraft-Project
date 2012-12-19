#ifndef __TERRAINGRID_HPP__
#define __TERRAINGRID_HPP__

namespace imacraft{

	struct TerrainGrid{
		private:
			uint8_t *m_data;
			uint16_t m_width;
		public:
			bool readFile(const char*);
			bool writeFile(const char*);
	};

}

#endif
