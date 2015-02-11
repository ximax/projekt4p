#include "StaticTiledMap.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
// Proxima
#define __DEBUG
// Ładuje mape z pliku tmx
// zapisuje pozycje *solidnych* kafli, tj. takich na które nie da
// się wejść do wektora. 
bool TileMap::loadFromXML(const std::string name)
{
	
	TiXmlDocument mapFile(name.c_str());
	if(!mapFile.LoadFile())
	{
		#ifdef __DEBUG
		std::cout << "Błąd, nie wczytano pliku \"" << name << "\"." << std::endl;
		#endif
		return false; 
	}
	// Rozmiary w kaflach oraz rozmiary kafli
	TiXmlElement *map = mapFile.FirstChildElement("map");
	width_in_tiles 	  = atoi(map->Attribute("width"));
	height_in_tiles   = atoi(map->Attribute("height"));
	tile_width		  = atoi(map->Attribute("tilewidth"));
	tile_height       = atoi(map->Attribute("tileheight"));
	//rozmiary i typ
	m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width_in_tiles * height_in_tiles * 4);    
    m_other_vertices.setPrimitiveType(sf::Quads);
    m_other_vertices.resize(width_in_tiles * height_in_tiles * 4);
	// misc
	TiXmlElement *tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	int firstTID = atoi(tilesetElement->Attribute("firstgid"));
	// image data
	TiXmlElement *image;
	image = tilesetElement->FirstChildElement("image");
	std::string imgpath = image->Attribute("source");
	// load check
	if(!m_tileset.loadFromFile(imgpath))
	{
		#ifdef __DEBUG
		std::cout << "Nie udało sie wczytać tilemapki" << std::endl;
		#endif
		return false;
	}
	//int col = m_tileset.getSize().x / tile_width;
	//int row = m_tileset.getSize().y / tile_height;

	TiXmlElement *layer = map->FirstChildElement("layer");
	std::string lName = layer->Attribute("name");
	int numz = 0;
	int layerNum = 0;
	sf::Uint32 x,y;
	x=y=0;
	
	printf("%s\n", lName.c_str());
	
	while(layer)
	{	
		lName = layer->Attribute("name");	// Jaki layer
		TiXmlElement* layerData = layer->FirstChildElement("data"); // znacznik data
		if(layerData == NULL)
		{
			#ifdef __DEBUG
			std::cout << "Nie znaleziono <data>" << std::endl;
			#endif
			return false;
		}
		TiXmlElement *tile = layerData->FirstChildElement("tile");
		if(tile == NULL)
		{
			#ifdef __DEBUG
			std::cout << "Nie znaleziono <tile>" << std::endl;
			#endif
			return false;
		}	
		while(tile)
		{
			int tileNumber = atoi(tile->Attribute("gid"))-firstTID;
			// Dodajemy kafle
			if(lName == "bg")							// Jeśli to tylko tło
			{
				appendTile(x,y,tileNumber, true);
				backgroundTiles.push_back(BackgroundTile(x,y,tileNumber));
			}
			if(lName == "solid" && tileNumber != -1)	// Jeśli obiekt solid to zapisujemy jego 
			{		
														// x i y ( w kaflach )
				appendTile(x,y,tileNumber, false);
				solidTiles.push_back(SolidTile(x,y,tileNumber));
			}
			// Jedziem dalej;	
			tile = tile->NextSiblingElement("tile");    // Ustaw wskaźnik na następny kafel
			x++;
			if (x >= width_in_tiles)
			{
				x = 0;
				y++;
				if (y >= height_in_tiles)
				{
					y = 0;
				}
			}
			#ifdef __DEBUG
			printf("TN:%i GIDNUM:%i LAYER:%i X:%i Y:%i LN:%s\n",tileNumber,numz,layerNum,x,y, lName.c_str());
			#endif
			numz++;
		}
	layer = layer->NextSiblingElement("layer");
	layerNum++;
	}
	// Obiekty ( bronie, zbroje .etc  na mapce )
	sf::Uint32 id,ix, iy, i_gid; // dane obiektu
	TiXmlElement *objectsGroup;
	id = 0;
	if(map->FirstChildElement("objectgroup") != NULL)
	{
		objectsGroup = map->FirstChildElement("objectgroup");
		while(objectsGroup)
		{
			TiXmlElement *object = objectsGroup->FirstChildElement("object");
			while(object)
			{
				//id	  = atoi(object->Attribute("id"));
				i_gid = atoi(object->Attribute("gid")) - firstTID;
				ix    = (atoi(object->Attribute("x"))) / tile_width; 
				iy 	  = (atoi(object->Attribute("y"))) / tile_width;
				// Czy takie rozwiązanie zarządzania itemami jest poprawne?
				// Tj uchwyt do obiektu
				if(imgr != nullptr)
				{
					appendTile(ix,iy,i_gid,0);
					itemsOnMap.push_back(Item(ix,iy,i_gid,id, true));
				}
				#ifdef __DEBUG
				printf("itemek x:%i y:%i, gid:%i\n",ix,iy,i_gid);
				#endif
				object = object->NextSiblingElement("object");
				++id;
			}
			objectsGroup = objectsGroup->NextSiblingElement("objectgroup");			
		}
	}
	return true;
}
// Czy na kratke da sie wejść
bool TileMap::isSolidTile(const sf::Uint32 _x, const sf::Uint32 _y)
{
	for(auto sTile : solidTiles)
	{
		if(_x == sTile.x && _y == sTile.y)
			return true;
	}
	return false;
}
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	// Rysuj tło
	target.draw(m_vertices, states);
	// Itemy rysuj
	target.draw(m_other_vertices, states);
}
bool TileMap::pickItem(const sf::Uint32 x,const sf::Uint32 y)
{
	std::list<Item>::iterator it=itemsOnMap.begin();	
	for(;it!=itemsOnMap.end();++it){
		if(it->x == x && it->y == y){
			// Dodaj item do managera
			imgr->addItem(*it);
			// Wyczyść kratke
			appendTile(it->x,it->y,0,0);
			// Skasuj z listy itemów na mapie
			itemsOnMap.erase(it);
			// Item podniesiony
			return true;
		}
	}
	// Nie da sie podnieśc itema bo nie ma itema na tej kratce
	return false;
}
bool TileMap::dropItem(const sf::Uint32 id)
{
	Item i = imgr->getItem(id);
	if((i.x != 0 && i.y != 0 && i.valid != 0)){
		itemsOnMap.push_back(i);
		appendTile(i.x,i.y,i.gid,0);
		imgr->deleteItem(id);
		return 1;
	}
	return 0;
}
// Dodaj kratke
// bgLayer to warstwa
// Są dwie, tło i itemki
void TileMap::appendTile(const sf::Int32 x,const sf::Int32 y, const sf::Int32 GID, const bool bgLayer)
{
	sf::Int32 tu = GID % (m_tileset.getSize().x / tile_width);
	sf::Int32 tv = GID / (m_tileset.getSize().x / tile_height);
	sf::Vertex* quad;
	// Jeśli to warstwa tła
	if( bgLayer )
		quad = &m_vertices[(x + y * width_in_tiles) * 4];
	// Jeśli to warstwa inna niż layer;
    else
	    quad = &m_other_vertices[(x + y * width_in_tiles) * 4];			
	quad[0].position = sf::Vector2f(x * tile_width, y * tile_height);
	quad[1].position = sf::Vector2f((x + 1) * tile_width, y * tile_height);
	quad[2].position = sf::Vector2f((x + 1) * tile_width, (y + 1) * tile_height);
	quad[3].position = sf::Vector2f(x * tile_width, (y + 1) * tile_height);
			
	quad[0].texCoords = sf::Vector2f(tu * tile_width, tv * tile_height);
	quad[1].texCoords = sf::Vector2f((tu + 1) * tile_width, tv * tile_height);
	quad[2].texCoords = sf::Vector2f((tu + 1) * tile_width, (tv + 1) * tile_height);
	quad[3].texCoords = sf::Vector2f(tu * tile_width, (tv + 1) * tile_height);
}
// Uchwyt do managera itemów gdyż ta klasa korzysta z managera
// Domyślam sie że to raczej rozwiązanie do bani :P
inline void TileMap::setItemManagerHandle(ItemManager*  igr){
	imgr = igr;
}
// Metoda zwraca true jeśli stoimy na itemie ( oraz jeśli na nim stoimy to jego id )
inline bool TileMap::isItem(const sf::Uint32 x, const sf::Uint32 y, sf::Int32& _id) const
{
	for(auto itm : itemsOnMap)
	{
		if(itm.x == x && itm.y == y){ 
			_id = itm.id;
			return true;
		}
			
	}
	_id = -1;
	return false;
}
