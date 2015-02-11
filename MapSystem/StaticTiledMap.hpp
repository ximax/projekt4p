#ifndef STATIC_TILED_MAP_INCLUDED
#define STATIC_TILED_MAP_INCLUDED
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <list>
struct SolidTile
{
	sf::Uint32 x;
	sf::Uint32 y;
	sf::Uint32 gid;
	SolidTile(const sf::Uint32 _x, const sf::Uint32 _y, const sf::Uint32 _gid) 
	: x(_x), y(_y), gid(_gid){};
};
struct BackgroundTile
{
	sf::Uint32 x;
	sf::Uint32 y;
	sf::Uint32 gid;
	BackgroundTile(const sf::Uint32 _x, const sf::Uint32 _y, const sf::Uint32 _gid) 
	: x(_x), y(_y), gid(_gid){};
};
class TileMap : public sf::Drawable, public sf::Transformable
{
	public:
		void 	   setItemManagerHandle(ItemManager* igr);
		
		size_t 	   itemsOnMapCount(){return itemsOnMap.size();}
		size_t solidTilesCount() {return solidTiles.size();}
		// Ładuj z XML
		bool 	   loadFromXML(const std::string name);
		// Czy to item?
		bool	   isItem(const sf::Uint32 x, const sf::Uint32 y, sf::Int32& _id) const;
		// Podnieś item 
		bool	   pickItem(const sf::Uint32 x, const sf::Uint32 y);
		// Wyrzuć item
		bool	   dropItem(const sf::Uint32 id);
		// Czy to solidna kratka
		bool	   isSolidTile(const sf::Uint32 x, const sf::Uint32 y);
		// Tylko do debugowania //
		void printSolidTiles(){for(auto it = solidTiles.begin();it!=solidTiles.end();++it){printf("x:%i, y:%i gid:%i\n",it->x,it->y, it->gid);  }}
		void    printItems1()
		{
			int i=0;
			printf("==========MAPITEMS==========\n");
			if(itemsOnMapCount() == 0)
			{
				printf("No items On map\n");
				printf("===============================\n");
				return ;
			}
			for(auto item : itemsOnMap)
			{
				printf("X:%i Y:%i \tGID:%i \tID:%i \tINDEX:%i\n", item.x,item.y,item.gid,item.id,i);
				i++;
			}
			printf("============================\n");
		}
		///////////////////////////		
	private:
		void appendTile(const sf::Int32, const sf::Int32, const sf::Int32, const bool);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		//Mapka
	    sf::VertexArray m_vertices;
	    sf::VertexArray m_other_vertices;
	    //Tileset
		sf::Texture m_tileset;
		// Szerokość podana w kafelkach
		sf::Uint32 width_in_tiles;
		// Wysokość podana w kafelkach
		sf::Uint32 height_in_tiles;
		// Szerokość kafla
		sf::Uint32 tile_width;
		// Wysokość kafla
		sf::Uint32 tile_height;
		// różne
		std::list<SolidTile> solidTiles;
		std::list<BackgroundTile> backgroundTiles;
		std::list<Item> itemsOnMap;
		// Uchwyt do ItemManager
		ItemManager *imgr = nullptr;	// Jeśli uchwyt nie zostanie ustawiony
										// Itemy nie będa widoczne		
};
#endif //STATIC_TILED_MAP_INCLUDED
