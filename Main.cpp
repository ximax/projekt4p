#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "tinyxml/tinyxml.h"
#include "MapSystem/ItemManager.cpp"
#include "MapSystem/StaticTiledMap.cpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tilemap");
    sf::View cam=window.getDefaultView();
    ItemManager imgr;
    TileMap map;
    
    map.setItemManagerHandle(&imgr);
	map.loadFromXML("r.tmx");
	
	sf::RectangleShape r(sf::Vector2f(10,10));
	int x,y;x=y=0;
	float jmp = 5;
	r.setPosition(sf::Vector2f(300,300));
	sf::Vector2f aPos;
	sf::Int32 itemId = 0;
	unsigned int iid = 0;
	bool isit,sol;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			isit = map.isItem(x,y,itemId);
			sol = map.isSolidTile(x,y);
			printf("X:%i Y:%i, solid:%i item:%i itemId:%i playerit:%i items:%i iid:%i\n"
			,x,y, map.isSolidTile(x,y), 
			isit, itemId, imgr.playerItemCount(), map.itemsOnMapCount(),iid);
            if(event.type == sf::Event::Closed)
                window.close();
                

			aPos = r.getPosition();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
			{
					r.move(-jmp,0.f);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
				if(isit == false && sol == false)
				{
					if(map.dropItem(iid))
						printf("Dropped!\n");
					else
						printf("No Way\n");
				}
				else
					printf("Stoisz na itemie albo solidnej kratce\n");
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				
				if(map.pickItem(x,y))
					printf("Picked!\n");
				else 
					printf("No Item\n");
				
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			{
				map.printItems1();
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::B))
			{
				imgr.printItems();
			}	
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Right )
				{
					++iid;
				}
				if(event.mouseButton.button == sf::Mouse::Left && iid > 0)
				{
					--iid;
				}
			}
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
            {
                r.move(jmp,0.f);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            {
				if(iid < imgr.playerItemCount())
					iid++;
				else iid = 0;
			}
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            {
				if(iid != 0)
					iid--;
			}
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
                r.move(0.f,-jmp);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
                r.move(0.0,jmp);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) )
				return 1;
			x = (int)r.getPosition().x;
			y = (int)r.getPosition().y;
			x = x/32;
			y = y/32; 
			

        }
		imgr.updateItemsData(x,y);
        // draw the map
        window.setView(cam);
        window.clear();
        window.draw(map);
        window.draw(r);
        window.display();
    }
	
    return 0;
}
