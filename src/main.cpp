#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

enum GameState {Game, MainMenu, Settings};
int SCREEN_HEIGHT = 1080;
int SCREEN_WIDTH = 1920;
sf::Color Brown = sf::Color(103, 71, 71);
sf::Color Red = sf::Color(249, 102, 102);
sf::Color Green = sf::Color(130, 148, 96);
sf::Color Grey = sf::Color(238, 238, 238);

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

class Ennemie{
    public:
        std::string name;
        int speed;
        int health;
        int damage;
        Ennemie(std::string n,int s, int h, int d) {
            name = n;
            speed = s;
            health = h;
            damage = d;
        };
};

class Wave{
    public:
        std::vector<Ennemie> ennemi_list;
        Wave(std::vector<Ennemie> ennemi_list_oc) {
            ennemi_list = ennemi_list_oc;
        };

        void print_wave(){
            for(int i =0; i<ennemi_list.size(); i++){
                std::cout << ennemi_list[i].name;
            }
        };
};

class Level{
    public:
        int height;
        int width;
        int difficulty;
        std::vector<std::vector<int>> grid;
        std::vector<Wave> wave_list;
};

class Map{
    public:
        int height;
        int width;
        Level level;
        void generate_level(Level level_oc){
            level = level_oc;
            
        };
        void visualise(){};

};

class GameManager{
    public:
        //all global stats here
        GameState game_state;
        void To_MainMenu(){
            game_state = MainMenu;
        };
        void To_Game(){
            game_state = Game;
        };
        void To_Settings(){
            game_state = Settings;
        };
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Stop the Goblins");
    GameManager GM; GM.To_MainMenu(); 

    sf::RectangleShape Menu_background(sf::Vector2f(SCREEN_WIDTH*1/2, SCREEN_HEIGHT*1/2));
        Menu_background.setPosition(sf::Vector2f(SCREEN_WIDTH/4, SCREEN_HEIGHT/3.5));
        Menu_background.setFillColor(Brown);

    const int level[] =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };

    TileMap map;
    if (!map.load("asset/GRASS+.png", sf::Vector2u(16, 16), level, 16, 8))
        return -1;

    Ennemie goblin("goblin", 2, 1, 2);
    Ennemie troll("troll", 1, 3, 3);
    std::vector<Ennemie> ennemie_list = {goblin, goblin, goblin, troll, troll};
    Wave wave0(ennemie_list);
    wave0.print_wave();



    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(Menu_background);
        window.draw(map);
        window.display();
    }

    return 0;
}