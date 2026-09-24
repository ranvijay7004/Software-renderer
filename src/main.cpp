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


void drawline(std::vector<uint8_t>& framebuffer , int x0 , int y0 , int x1,
    int y1 , uint8_t r , uint8_t g , uint8_t b , uint8_t a ){
       
        bool steep = std::abs(y1-y0) > std::abs(x1-x0);
        if (steep){
            std::swap(x0,y0);
            std::swap(x1,y1);
        }

        int error = 0;
        int y = y0;
        int x = x0;
        int cx;
        int cy;
        int dx,dy;

        dx = std::abs(x1-x0);
        dy = std::abs(y1-y0);

        if(x0 < x1) cx = +1;
        else cx = -1;

        if(y0 < y1) cy = +1;
        else cy = -1;
        
        for(int i = 0 ; i <= dx ; i++){

            if(steep){setpixel(framebuffer,y,x,r,g,b,a);}
            else{setpixel(framebuffer,x,y,r,g,b,a);}

            error += dy;
            if(error >= dx){
                y += cy;
                error -= dx;
            }
            x += cx;
        }
    }


int main(){

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"Window");

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);
    
    for(int i = 0 ; i < 600 ; i++){
    setpixel(framebuffer,50,i,225,0,0,225);
    }

    drawline(framebuffer,100,100,150,599,0,0,255,255); 

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
