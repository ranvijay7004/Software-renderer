#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

const int WIDTH = 1920;
const int HEIGHT = 1080;

struct Vec2{
    float x,y;

    Vec2 operator+(const Vec2& other) const {
        Vec2 result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    Vec2 operator-(const Vec2& other) const {
        Vec2 result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    Vec2 operator*(float scale) const {
        Vec2 result;
        result.x = x * scale;
        result.y = y * scale;
        return result;
    }
};

float dot(Vec2 a , Vec2 b) {
    return a.x*b.x + a.y*b.y;
}

struct Vec3{
    float x,y,z;

    Vec3 operator+(const Vec3& other) const {
        Vec3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result;
    }

    Vec3 operator-(const Vec3& other) const {
        Vec3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }

    Vec3 operator*(float scale) const {
        Vec3 result;
        result.x = x * scale;
        result.y = y * scale;
        result.z = z * scale;
        return result;

    }
};

float length(Vec3& a){
    return std::sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vec3 normalise(Vec3& a){
    float len = length(a);
    if(len == 0) return a;
    return {a.x/len , a.y/len , a.z/len};
}

float dot(Vec3 a , Vec3 b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 cross(Vec3 a , Vec3 b){
    Vec3 result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

struct Vec4{
    float x,y,z,w;
};

float dot(Vec4& a , Vec4& b){
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

struct mat4{

    Vec4 row0 , row1 , row2 , row3;
    
    Vec4 column0(){return {row0.x , row1.x , row2.x , row3.x};}
    Vec4 column1(){return {row0.y , row1.y , row2.y , row3.y};}
    Vec4 column2(){return {row0.z , row1.z , row2.z , row3.z};}
    Vec4 column3(){return {row0.w , row1.w , row2.w , row3.w};}

    Vec4 multiply(Vec4& a){
        Vec4 result;
        result.x = dot(row0,a);
        result.y = dot(row1,a);
        result.z = dot(row2,a);
        result.w = dot(row3,a);
        return result;
    }

    mat4 multiply(mat4& a){

        Vec4 c0 = a.column0();
        Vec4 c1 = a.column1();
        Vec4 c2 = a.column2();
        Vec4 c3 = a.column3();

        mat4 result;
        result.row0 = {dot(row0,c0) , dot(row0,c1) , dot(row0,c2) , dot(row0,c3)};
        result.row1 = {dot(row1,c0) , dot(row1,c1) , dot(row1,c2) , dot(row1,c3)};
        result.row2 = {dot(row2,c0) , dot(row2,c1) , dot(row2,c2) , dot(row2,c3)};
        result.row3 = {dot(row3,c0) , dot(row3,c1) , dot(row3,c2) , dot(row3,c3)};
        return result; 
    }

    static mat4 transaltion(float dx , float dy , float dz){
        mat4 result;
        result.row0 = {1,0,0,dx};
        result.row1 = {0,1,0,dy};
        result.row2 = {0,0,1,dz};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 scale(float dx , float dy , float dz){
        mat4 result;
        result.row0 = {dx,0,0,0};
        result.row1 = {0,dy,0,0};
        result.row2 = {0,0,dz,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 rotationZ(float angledegree){

        float angleradians = angledegree * (3.14159265f / 180.0f);
        float c = cos(angleradians);
        float s = sin(angleradians);

        mat4 result;
        result.row0 = {c,-s,0,0};
        result.row1 = {s,c,0,0};
        result.row2 = {0,0,1,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 rotationY(float angledegree){

        float angleradians = angledegree * (3.14159265f / 180.0f);
        float c = cos(angleradians);
        float s = sin(angleradians);

        mat4 result;
        result.row0 = {c,0,-s,0};
        result.row1 = {0,1,0,0};
        result.row2 = {s,0,c,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 rotationX(float angledegree){

        float angleradians = angledegree * (3.14159265f / 180.0f);
        float c = cos(angleradians);
        float s = sin(angleradians);

        mat4 result;
        result.row0 = {1,0,0,0};
        result.row1 = {0,c,s,0};
        result.row2 = {0,-s,c,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 perspective(float p){
        mat4 result;
        result.row0 = {p,0,0,0};
        result.row1 = {0,p,0,0};
        result.row2 = {0,0,1,0};
        result.row3 = {0,0,1,0};
        return result; 
    }
};

struct triangle{
    int x , y , z;
};

bool loadobj(const std::string& path , std::vector<Vec3>& Vertices , std::vector<triangle>& triangles){
    std::ifstream file(path);
    if(!file.is_open()) return false;
    
    std::string line;
    
    while(std::getline(file,line)){
        std::istringstream ss(line);
        std::string tag;
        ss>>tag;
        
        if(tag == "v"){
            Vec3 v;
            ss>>v.x>>v.y>>v.z;
            Vertices.push_back(v);
        }

        if(tag == "f"){
            std::vector<int> corners;
            std::string tokens;

            while(ss>>tokens){
                corners.push_back(std::stoi(tokens) - 1);
            }
            
            for(int k = 1 ; k+1 < corners.size() ; k++){
                triangles.push_back({corners[0] , corners[k] , corners[k+1]});
            }
        } 
    }
    return true;
}

void setpixel(std::vector<uint8_t>& framebuffer , int x , int y , uint8_t r , uint8_t g , uint8_t b , uint8_t a ){
    
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

    int index = (y * WIDTH + x) * 4;

    framebuffer[index + 0] = r;
    framebuffer[index + 1] = g;
    framebuffer[index + 2] = b;
    framebuffer[index + 3] = a;
}

void drawline(std::vector<uint8_t>& framebuffer , int x1 , int y1 , 
    int x2 , int y2 , uint8_t r , uint8_t g , uint8_t b , uint8_t a){

        bool steep = std::abs(y2-y1) > std::abs(x2-x1);

        if(steep){
            std::swap(x1,y1);
            std::swap(x2,y2);
        }

        int dx , dy , cx , cy;
        
        if(x2>x1) cx = +1;
        else cx = -1;

        if(y2>y1) cy = +1;
        else cy = -1;

        dx = std::abs(x2-x1);
        dy = std::abs(y2-y1);

        int x = x1;
        int y = y1;

        int error = 0;
        
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

void drawtriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2 
    ,int x3 , int y3 , uint8_t r , uint8_t g , uint8_t b , uint8_t a){

        drawline(framebuffer,x1,y1,x2,y2,r,g,b,a);
        drawline(framebuffer,x2,y2,x3,y3,r,g,b,a);
        drawline(framebuffer,x3,y3,x1,y1,r,g,b,a);
}


int checkinside(int ax , int ay ,int bx ,int by , int px , int py){
    return (bx-ax)*(py-ay) - (by-ay)*(px-ax);
}

bool istopleft(int ax , int ay , int bx , int by){

    bool istop = ay == by && bx > ax;
    bool isleft = ay > by;

    return istop || isleft; 
}

void filltriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2 
        ,int x3 , int y3 , uint8_t r , uint8_t g , uint8_t b , uint8_t a){

    int minX = std::min({x1,x2,x3});
    int maxX = std::max({x1,x2,x3});
    int minY = std::min({y1,y2,y3});
    int maxY = std::max({y1,y2,y3});

    int bias1 = istopleft(x1,y1,x2,y2);
    int bias2 = istopleft(x2,y2,x3,y3);
    int bias3 = istopleft(x3,y3,x1,y1);

    for(int x = minX ; x < maxX ; x++){
        for(int y = minY ; y < maxY ; y++){

            int w1 = checkinside(x1,y1,x2,y2,x,y);
            int w2 = checkinside(x2,y2,x3,y3,x,y);
            int w3 = checkinside(x3,y3,x1,y1,x,y);
                
            bool inside = (w1 + bias1 > 0 && w2 + bias2 > 0 && w3 + bias3 > 0);

            if(inside){
                float A = w1 + w2 + w3;
                if(A == 0) continue;
                float l1 = (float)w2/A;
                float l2 = (float)w3/A;
                float l3 = (float)w1/A;
                setpixel(framebuffer,x,y,(uint8_t)(l1*255),(uint8_t)(l2*255),(uint8_t)(l3*255),a);
            }
        }
    }
}

bool toscreen(mat4& x , Vec3& p , Vec2& out){
    Vec4 k = {p.x , p.y , p.z , 1};
    k = x.multiply(k);

    if(k.w <= 0.5f) return false;
    out = {k.x/k.w + WIDTH/2 , k.y/k.w + HEIGHT/2};
    return true;
}

void drawmesh(std::vector<uint8_t>& framebuffer , std::vector<Vec3>& Vertices , std::vector<triangle>& triangles
    ,float angle1 , float angle2 , float angle3){

    float camX = 0 , camY = 0  , camZ = -20;

    mat4 rotateY = mat4::rotationY(angle1);
    mat4 rotateX = mat4::rotationX(angle2);
    mat4 rotateZ = mat4::rotationZ(angle3);
    mat4 view = mat4::transaltion(-camX , -camY , -camZ);
    mat4 pers = mat4::perspective(800.0f);
    
    mat4 x = pers.multiply(view)
            .multiply(rotateY)
            .multiply(rotateX)
            .multiply(rotateZ);
    
    std::vector<Vec2> sp(Vertices.size());
    std::vector<bool> visible(Vertices.size());
    
    for(int i = 0 ; i < Vertices.size() ; i++){
        visible[i] = toscreen(x,Vertices[i],sp[i]);
    }

    int colors[6][4] = {
        {255,107,107,255} , {255,217,61,255},
        {107,203,119,255} , {77,150,255,255},
        {199,125,255,255} , {255,159,67,255}
    };
 
    for(int i = 0 ; i < triangles.size() ; i++){
        triangle t = triangles[i];
        int c = i/2 % 6;
        if(visible[t.x] && visible[t.y] && visible[t.z]){
            filltriangle(framebuffer, sp[t.x].x , sp[t.x].y , 
            sp[t.y].x , sp[t.y].y , sp[t.z].x , sp[t.z].y ,
            colors[c][0] , colors[c][1] , colors[c][2] , colors[c][3]);
        }
    }  
}

Vec3 facenormal(Vec3 a , Vec3 b , Vec3 c){
    Vec3 e1 = b-a;
    Vec3 e2 = c-a;
    Vec3 n = cross(e2,e1);
    return normalise(n);
}

int main(){

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT) , "renderer");
    window.setFramerateLimit(60);

    std::ifstream file("files/cube.obj");

    if(!file.is_open()){
        std::cerr<<"could not open cube.obj"<<std::endl;
        return 1;
    }
    
    std::vector<triangle> Triangles;
    std::vector<Vec3> Vertices;

    if(!loadobj("files/sphere.obj", Vertices , Triangles)){
        std::cerr<<"couldn't load model"<<std::endl;
        return 1;
    }
    
    int outwards = 0;
    for(int i = 0 ; i < Triangles.size() ; i++){
        triangle k = Triangles[0];
        Vec3 n = facenormal(Vertices[k.x] , Vertices[k.y] , Vertices[k.z]);
        
        if(dot(n,Vertices[k.x]) > 0) outwards++;
    }

    std::cerr<<outwards<<" of "<<Triangles.size()<<" points outwards"<<std::endl; 

    float angle1 = 0;
    float angle2 = 35.264;
    float angle3 = 45;
    

    sf::Texture pixeltexture;
    pixeltexture.create(WIDTH,HEIGHT);
    sf::Sprite pixelsprite;
    pixelsprite.setTexture(pixeltexture);

    sf::Event event;

    while(window.isOpen()){
        while(window.pollEvent(event)){

            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
   
        drawmesh(framebuffer, Vertices , Triangles , angle1 , angle2 , angle3);
        angle1 += 1.0f;

        pixeltexture.update(framebuffer.data());
        std::fill(framebuffer.begin() , framebuffer.end() , 0);

        window.clear();
        window.draw(pixelsprite);
        window.display();
    }
    return 0;
}