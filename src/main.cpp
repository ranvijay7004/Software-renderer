#include <SFML/Graphics.hpp>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

struct Vec2{
    float x,y;

    Vec2 operator+(const Vec2& other) const{
        Vec2 result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    Vec2 operator-(const Vec2& other) const{
        Vec2 result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    Vec2 operator*(float scalar)const{
        Vec2 result;
        result.x = x * scalar;
        result.y = y * scalar;
        return result;
    }
};

float dot(Vec2 a , Vec2 b){
    return a.x*b.x + a.y*b.y;
}

struct Vec3{
    float x,y,z;

    Vec3 operator+(const Vec3& other) const{
        Vec3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result; 
    }

    Vec3 operator-(const Vec3& other) const{
        Vec3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }

    Vec3 operator*(float scalar) const{
        Vec3 result;
        result.x = x * scalar;
        result.y = y * scalar;
        result.z = z * scalar;
        return result;
    }

    Vec2 screenpoint(){
    
        Vec2 result;

        float scale = 200.00f;
        float centerX = WIDTH / 2.00f;
        float centerY = HEIGHT / 2.00f;

        result.x = (x/z) * scale + centerX;
        result.y = (y/z) * scale + centerY;
        
        return result;
    }
};

float dot(Vec3 a , Vec3 b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 cross(Vec3 a , Vec3 b){
    Vec3 result;
    result.x = a.y * b.z - b.y * a.z;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result; 
}

struct Vec4{
    float x,y,z,w;
};

float dot(Vec4 a , Vec4 b){
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

struct mat4{
    Vec4 row0 , row1 , row2 , row3;

    Vec4 multiply(Vec4 a) const {
        Vec4 result;
        result.x = dot(row0 , a);
        result.y = dot(row1 , a);
        result.z = dot(row2 , a);
        result.w = dot(row3 , a);
        return result;
    }

    static mat4 translation(float dx , float dy , float dz){
        mat4 result;
        result.row0 = {1,0,0,dx};
        result.row1 = {0,1,0,dy};
        result.row2 = {0,0,1,dz};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 scaler(float dx , float dy , float dz){
        mat4 result;
        result.row0 = {dx,0,0,0};
        result.row1 = {0,dy,0,0};
        result.row2 = {0,0,dz,0};
        result.row3 = {0,0,0,1};
        return result;
    }
};

void setpixel(std::vector<uint8_t>& framebuffer , int x , int y , uint8_t r , 
    uint8_t g , uint8_t b , uint8_t a){

        int index = (y * WIDTH + x) * 4;

        framebuffer [index + 0] = r;
        framebuffer [index + 1] = g;
        framebuffer [index + 2] = b;
        framebuffer [index + 3] = a;
}

void drawline(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2 , 
    uint8_t r ,  uint8_t g , uint8_t b , uint8_t a){

    bool steep  = std::abs(y2 - y1) > std::abs(x2 - x1);
    if(steep){
        std::swap(x1,y1);
        std::swap(x2,y2);
    }

    int dx = std::abs(x2-x1);
    int dy = std::abs(y2-y1);
    
    int cx;
    int cy;
    int x = x1;
    int y = y1;
    int error = 0;

    if(x2 > x1) cx = +1;
    else cx = -1;

    if(y2>y1) cy = +1;
    else cy = -1;

    for(int i = 0 ; i <= dx ; i++){
        
        if(steep) setpixel(framebuffer,y,x,r,g,b,a);
        else setpixel(framebuffer,x,y,r,g,b,a);

        error += dy;
        if(error >= dx){
            y += cy;
            error -= dx;
        }
        x += cx;
    }
}      

void drawtriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2 , int x3 , int y3 ,
    uint8_t r ,  uint8_t g , uint8_t b , uint8_t a){

    drawline(framebuffer, x1 , y1 , x2 , y2 , r,g,b,a);    
    drawline(framebuffer, x2 , y2 , x3 , y3 , r,g,b,a);
    drawline(framebuffer, x3 , y3 , x1 , y1 , r,g,b,a);
}

int checkinside(int ax , int ay , int bx , int by , int px , int py){
    return (bx-ax)*(py-ay) - (by-ay)*(px-ax);
}

void filltriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2 , int x3 , int y3 ,
    uint8_t r ,  uint8_t g , uint8_t b , uint8_t a){

        int maxX = std::max({x1,x2,x3});
        int minX = std::min({x1,x2,x3});
        int maxY = std::max({y1,y2,y3});
        int minY = std::min({y1,y2,y3});

        for(int x = minX ; x <= maxX ; x++){
            for(int y = minY ; y <= maxY ; y++){
                int w1 = checkinside(x1,y1,x2,y2,x,y);
                int w2 = checkinside(x2,y2,x3,y3,x,y);
                int w3 = checkinside(x3,y3,x1,y1,x,y);

                bool inside = (w1 >= 0 && w2 >= 0 && w3 >= 0);
                if(inside){
                    setpixel(framebuffer,x,y,r,g,b,a);
                }
            }
        }
}

void drawcube(std::vector<uint8_t>& framebuffer , Vec3 vert[8] ,
    uint8_t r ,  uint8_t g , uint8_t b , uint8_t a){
        
        Vec2 sp[8];

        for(int i  = 0 ; i < 8 ; i++){
            sp[i] = vert[i].screenpoint(); 
        }

        int edges[12][2] = {
            {0,1} , {1,2} , {2,3} , {3,0}, //front face
            {4,5} , {5,6} , {6,7} , {7,4}, //back face
            {0,4} , {1,5} , {2,6} , {3,7}, //connecting them
        };

        for(int i = 0 ; i < 12 ; i++){
            int indxa = edges[i][0];
            int indxb = edges[i][1];
            drawline(framebuffer,(int)sp[indxa].x , (int)sp[indxa].y ,
            (int)sp[indxb].x , (int)sp[indxb].y , r , g , b , a);
        }
    }


int main(){

    std::vector<uint8_t> framebuffer(WIDTH * HEIGHT * 4);

    Vec3 cubeverts[8] = {
        {-3,-3,5} , {3,-3,5} , {3,3,5} , {-3,3,5}, //front face
        {-3,-3,8} , {3,-3,8} , {3,3,8} , {-3,3,8} //back face
    };

    mat4 move = mat4::translation(5 , 0 , 0);
    Vec4 point = {10,3,2,1};
    Vec4 moved = move.multiply(point);
    std::cerr<<moved.x<<std::endl;
    std::cerr<<moved.y<<std::endl;
    std::cerr<<moved.z<<std::endl;

    mat4 scale = mat4::scaler(3,3,3);
    Vec4 scaled = scale.multiply(point);
    std::cerr<<scaled.x<<std::endl;
    std::cerr<<scaled.y<<std::endl;
    std::cerr<<scaled.z<<std::endl;

    drawcube(framebuffer,cubeverts,255,0,0,255);

    sf::Texture pixeltexture;
    pixeltexture.create(WIDTH,HEIGHT);
    pixeltexture.update(framebuffer.data());

    sf::Sprite pixelsprite;
    pixelsprite.setTexture(pixeltexture);

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"Window");

    while(window.isOpen()){
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
            window.close();
        }

        window.clear();
        window.draw(pixelsprite);
        window.display();
    }

    return 0;
}