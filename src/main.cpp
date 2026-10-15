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

Vec3 facenormal(Vec3 a , Vec3 b , Vec3 c){
    Vec3 e1 = b-a;
    Vec3 e2 = c-a;
    Vec3 n = cross(e2,e1);
    return normalise(n);
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
    int tx ,ty ,tz;
};

struct texture{
  int w ,h;

  std::vector<uint8_t> data;
};

struct color{
    uint8_t r,g,b;
};

color sampletexture(texture& tex , float u  , float v){
    
    u = u - std::floor(u);

    int x = (int)(u*tex.w);
    int y = (int)(v*tex.h);

    if(x > tex.w -1) x = tex.w - 1;
    if(y > tex.h -1) y = tex.h - 1;

    int indx = (y * tex.w + x) *3;

    return {tex.data[indx] , tex.data[indx+1] , tex.data[indx+2]};
}

void parsecorner(const std::string& token , int& v , int& t){

    v = std::stoi(token) -1;
    t = -1;

    size_t slash = token.find('/');
    
    if(slash != std::string::npos && slash +1 < token.size() && token[slash+1] != '/'){
        t = std::stoi(token.substr(slash + 1)) -1;
    }
}

bool loadobj(const std::string& path , std::vector<Vec3>& Vertices , std::vector<Vec2>& uvs , std::vector<triangle>& triangles){

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

        else if(tag == "vt"){
            Vec2 uv;
            ss>>uv.x>>uv.y;
            uv.y = 1.0f - uv.y;
            uvs.push_back(uv);
        }

        else if(tag == "f"){

            std::vector<int> vs , ts;
            std::string tokens;

            while(ss>>tokens){
                int v , t;
                parsecorner(tokens , v , t);
                vs.push_back(v);
                ts.push_back(t);
            }
            
            for(int k = 1 ; k+1 < vs.size() ; k++){
                triangle tri;

                tri.x = vs[0] , tri.y = vs[k] , tri.z = vs[k+1];
                tri.tx = ts[0] , tri.ty = ts[k] , tri.tz = ts[k+1];

                triangles.push_back(tri);
            }
        } 
    }
    return true;
}

bool loadppm(const std::string& path , texture& tex){

  std::ifstream file(path,std::ios::binary);
  
  if(!file.is_open()) return false;

  std::string magic;
  int maxval;
  
  file>>magic>>tex.w>>tex.h>>maxval;

  if(magic != "P6" || maxval != 255) return false;

  file.get();
  tex.data.resize(tex.w * tex.h * 3);
  file.read((char*)tex.data.data() , tex.data.size());
  return !file.fail();
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

void filltriangle(std::vector<uint8_t>& framebuffer , std::vector<float>& zbuffer , int x1 , int y1 , int x2 , int y2 ,
    int x3 , int y3 ,float z1 , float z2 , float z3 , float s1 , float s2 , float s3 ,float u1 , float v1 ,
    float u2 , float v2 , float u3 , float v3 , texture& tex , uint8_t r , uint8_t g , uint8_t b , uint8_t a){


    int minX = std::min({x1,x2,x3});
    int maxX = std::max({x1,x2,x3});
    int minY = std::min({y1,y2,y3});
    int maxY = std::max({y1,y2,y3});

    int bias1 = istopleft(x1,y1,x2,y2);
    int bias2 = istopleft(x2,y2,x3,y3);
    int bias3 = istopleft(x3,y3,x1,y1);

    for(int x = minX ; x < maxX ; x++){
        for(int y = minY ; y < maxY ; y++){

            if(x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) continue;

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

                float depth = l1/z1 + l2/z2 + l3/z3;

                int idx = (y * WIDTH) + x;
                if(depth > zbuffer[idx]){
                    zbuffer[idx] = depth;

                    float shade = l1*s1 + l2*s2 + l3*s3;
                    if(shade < 0) shade = 0 ;
                    if(shade > 1) shade = 1;

                    float u = (l1*u1/z1 + l2*u2/z2 + l3*u3/z3) / depth;
                    float v = (l1*v1/z1 + l2*v2/z2 + l3*v3/z3) / depth;

                    color c = sampletexture(tex , u , v);

                    setpixel(framebuffer,x,y,(uint8_t)(c.r*shade),(uint8_t)(c.g*shade),(uint8_t)(c.b*shade),a);
                }
            }
        }
    }
}

bool toscreen(mat4& x , Vec3& p , Vec2& out , float& depth){
    Vec4 k = {p.x , p.y , p.z , 1};
    k = x.multiply(k);

    if(k.w <= 0.5f) return false;
    depth = k.w;
    out = {k.x/k.w + WIDTH/2 , k.y/k.w + HEIGHT/2};
    return true;
}

void drawmesh(std::vector<uint8_t>& framebuffer , std::vector<float>& zbuffer ,  std::vector<Vec3>& Vertices , std::vector<triangle>& triangles
    ,std::vector<Vec3>& vnormal , texture& tex , std::vector<Vec2>& uvs , float angle1 , float angle2 , float angle3 , int color =-1){

    float camX = 0 , camY = 0  , camZ = -20;
    Vec3 light = {-0.6f , -0.6f , -0.7f};
    light = normalise(light);
    float ambient = 0.12f;

    mat4 rotateY = mat4::rotationY(angle1);
    mat4 rotateX = mat4::rotationX(angle2);
    mat4 rotateZ = mat4::rotationZ(angle3);
    mat4 view = mat4::transaltion(-camX , -camY , -camZ);
    mat4 pers = mat4::perspective(1200.0f);

    mat4 model = rotateY.multiply(rotateX).multiply(rotateZ);
    
    mat4 x = pers.multiply(view).multiply(model);
    
    std::vector<Vec2> sp(Vertices.size());
    std::vector<bool> visible(Vertices.size());
    std::vector<float> depth(Vertices.size());
    std::vector<float> vbright(Vertices.size());
    
    for(int i = 0 ; i < Vertices.size() ; i++){
        visible[i] = toscreen(x,Vertices[i],sp[i],depth[i]);
    }

    int colors[6][4] = {
        {255,107,107,255} , {255,217,61,255},
        {107,203,119,255} , {77,150,255,255},
        {199,125,255,255} , {255,159,67,255}
    };

    for(int i = 0 ; i < Vertices.size() ; i++){

        Vec4 k = {vnormal[i].x , vnormal[i].y , vnormal[i].z , 0};
        k = model.multiply(k);
        Vec3 nr = {k.x , k.y , k.z};

        float d = dot(nr,light);
        if(d < 0) d = 0;

        float b = ambient + (1 - ambient) * d;
        if(b > 1) b =1;

        vbright[i] = b;
    }

    for(int i = 0 ; i < triangles.size() ; i++){
        triangle t = triangles[i];
        
        Vec2 uv1 = {0,0} , uv2 = {0,0} , uv3 = {0,0};

        if(t.tx >= 0){
            uv1 = uvs[t.tx];
            uv2 = uvs[t.ty];
            uv3 = uvs[t.tz];
        }

        int c = (color >= 0) ? color : i/2 % 6;

        if(visible[t.x] && visible[t.y] && visible[t.z]){
            filltriangle(framebuffer, zbuffer ,  sp[t.x].x , sp[t.x].y , 
            sp[t.y].x , sp[t.y].y , sp[t.z].x , sp[t.z].y ,
            depth[t.x] , depth[t.y] , depth[t.z] ,
            vbright[t.x] , vbright[t.y] , vbright[t.z],
            uv1.x , uv1.y , uv2.x , uv2.y , uv3.x , uv3.y , tex,
            (int)(colors[c][0]) , (int)(colors[c][1]) , (int)(colors[c][2]) , 255);
        }
    }  
}

std::vector<Vec3> vertexnormals(std::vector<Vec3>& Vertices , std::vector<triangle>& Triangles){
    
        std::vector<Vec3> sum(Vertices.size(), Vec3({0,0,0}));
        for(int i = 0 ; i < Triangles.size() ; i++){
            triangle t = Triangles[i];

            Vec3 n = facenormal(Vertices[t.x] , Vertices[t.y] , Vertices[t.z]);
            sum[t.x] = sum[t.x] + n;
            sum[t.y] = sum[t.y] + n;
            sum[t.z] = sum[t.z] + n;
        }

        for(int i = 0 ; i < sum.size() ; i++){
            sum[i] = normalise(sum[i]);
        }

        return sum;
    };

int main(){

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);
    std::vector<float> zbuffer(WIDTH*HEIGHT , 0.0f);

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT) , "renderer");
    window.setFramerateLimit(60);
    
    std::vector<triangle> Triangles;
    std::vector<Vec3> Vertices;
    std::vector<Vec2> uvs;
    texture tex;

    if(!loadobj("models/spiky_uv.obj", Vertices , uvs , Triangles)){
        std::cerr<<"couldn't load model"<<std::endl;
        return 1;
    }
    
    if(!loadppm("models/checker.ppm" , tex)){
      std::cerr<<"couldn't load texture"<<std::endl;
      return 1;
    }

    std::vector<Vec3> vnormals = vertexnormals(Vertices,Triangles);

    triangle t = Triangles[0];
    std::cerr<<uvs.size()<<" uvs size "<<t.x<<" "<<t.y<<" "<<t.z<<" "<<t.tx<<" "<<t.ty<<" "<<t.tz<<std::endl;
    
    float angle1 = 0;
    float angle2 = 35.264;
    float angle3 = 45;
    

    sf::Texture pixeltexture;
    pixeltexture.create(WIDTH,HEIGHT);
    sf::Sprite pixelsprite;
    pixelsprite.setTexture(pixeltexture);

    sf::Clock clock;

    sf::Event event;

    while(window.isOpen()){
        while(window.pollEvent(event)){

            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
        
        float dt = clock.restart().asSeconds();
        window.setTitle("renderer  " + std::to_string((int)(1/dt)) + " fps  " + std::to_string((int)(1000*dt)) + " ms");

        drawmesh(framebuffer, zbuffer , Vertices , Triangles ,  vnormals , tex , uvs , angle1 , angle2 , angle3,3);
        angle1 += 1.0f;

        pixeltexture.update(framebuffer.data());
        std::fill(framebuffer.begin() , framebuffer.end() , 0);
        std::fill(zbuffer.begin() , zbuffer.end() , 0);

        window.clear();
        window.draw(pixelsprite);
        window.display();
    }
    return 0;
}
