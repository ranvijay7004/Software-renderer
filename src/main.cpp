#include <SFML/Graphics.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;

void setpixel(std::vector<uint8_t>& framebuffer , int x , int y ,
    uint8_t r, uint8_t g, uint8_t b, uint8_t a ){
       
    int index = (y * WIDTH + x) * 4;

    framebuffer[index + 0] = r;
    framebuffer[index + 1] = g;
    framebuffer[index + 2] = b;
    framebuffer[index + 3] = a;
}


int main(){

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"Window");

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);
    
    for(int i = 0 ; i < 600 ; i++){
    setpixel(framebuffer,50,i,225,0,0,225);
    }

    sf::Texture pixel;
    pixel.create(WIDTH,HEIGHT); 
    pixel.update(framebuffer.data());
    sf::Sprite pixelsprite; 
    pixelsprite.setTexture(pixel);


while(window.isOpen()){

    sf::Event event;

    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            window.close();
        }

        if(event.type == sf::Event::KeyPressed){
            if(event.key.code == sf::Keyboard::Escape){
                window.close();
            }
        }
    }

    window.clear();
    window.draw(pixelsprite);
    window.display();
}
    return 0;
}
