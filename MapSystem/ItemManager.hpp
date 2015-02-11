#ifndef ITEM_MANAGER_INCLUDED
#define ITEM_MANAGER_INCLUDED
#include <list>
struct Item
{
	sf::Uint32 x;
	sf::Uint32 y;
	sf::Uint32 gid;
	sf::Int32  id;
	bool	   valid;
	Item():x(0),y(0),gid(0),id(0),valid(0){};
	Item(const sf::Uint32 _x, const sf::Uint32 _y, const sf::Uint32 _gid, const sf::Int32 _id, bool _v)
		: x(_x), y(_y), gid(_gid),id(_id), valid(_v) {};
	//ctor
	Item(const Item& tmp) : x(tmp.x), y(tmp.y), gid(tmp.gid), id(tmp.id), valid(tmp.valid){};
	//assigment
	Item operator=(const Item& rhs) {Item tmp(rhs);return tmp;}
};
class ItemManager
{
public:
	ItemManager(){};
	~ItemManager(){};
	size_t  playerItemCount();
	void    printItems()
	{
		int index=0;
		printf("==========PLAYERITEMS==========\n");
		if(playerItemCount() == 0)
		{
			printf("No items in vct\n");
			printf("===============================\n");
			return;
		}
		for(auto item : playerItems)
		{
			printf("X:%i Y:%i \tGID:%i \tID:%i \tINDEX:%i\n", item.x,item.y,item.gid,item.id,index);
			index++;
		}
		printf("===============================\n");
	}
	void 	updateItemsData(const sf::Int32, const sf::Int32);
	void 	addItem(const Item it);
	bool	deleteItem(const sf::Int32 id);
	Item	getItem(const sf::Int32 id) const;
private:
	std::list<Item> playerItems;
};

#endif //ITEM_MANAGER_INCLUDED

