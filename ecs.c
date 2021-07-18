#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*********
* Macros *
*********/
#define xor(a,b)                        (((a) && !(b)) || (!(a) && (b)))
#define min(a,b)                        ((a < b) ? (a) : (b))
#define max(a,b)                        ((a > b) ? (a) : (b))
#define min3(a,b,c)                     (min(min((a), (b)), (c)))
#define max3(a,b,c)                     (max(max((a), (b)), (c)))
#define sign(x)                         (((x) < 0) ? -1 : (((x) > 0) ? 1 : 0))
#define sameSigns(a,b)                  (((long) ((unsigned long)a ^ (unsigned long)b)) >= 0)
#define degToRad(degrees)               ((degrees) * M_PI / 180.0)
#define radToDeg(radians)               ((radians) * 180.0 / M_PI)

#define      setVec2(a,b)               a.x = b.x,          a.y = b.y
#define      addVec2(a,b)               a.x += b.x,         a.y += b.y
#define subtractVec2(a,b)               a.x -= b.x,         a.y -= b.y
#define multiplyVec2(a,b)               a.x *= b.x,         a.y *= b.y
#define   divideVec2(a,b)               a.x /= b.x,         a.y /= b.y
#define    scaleVec2(v,scale)           v.x *= scale,       v.y *= scale
#define    crossVec2(a,b,ab)            ab.x = (a.x)*(b.y), ab.y = -(a.y)*(b.x)
#define setVec2Angle(v, angle)          v.x = cos(angle),   v.y = -sin(angle)
#define getVec2Angle(v)                 atan2(v.y, v.x)
#define getVec2Length(v)                sqrt(pow(v.x,2) + pow(v.y,2))
#define getVec2LengthSquared(v)              pow(v.x,2) + pow(v.y,2)
#define getVec2Distance(v1,v2)          sqrt(pow(v1.x-v2.x,2) + pow(v1.y-v2.y,2))
#define getVec2DistanceSquared(v1,v2)        pow(v1.x-v2.x,2) + pow(v1.y-v2.y,2)

#define tileAt(board,x,y)               board->tileMap[(y) * board->w + (x)]
#define setTile(board,x,y,tile)         tileAt(board,(x),(y)) = tile

#define setColor(dst,src)               dst[0] = src[0], dst[1] = src[1], dst[2] = src[2], dst[3] = src[3]
#define colorArg3(array)                array[0], array[1], array[2]
#define colorArg4(array)                array[0], array[1], array[2], array[3]

/************
* Constants *
************/
// Basics
#define BUFFER_SIZE                     64

// Logic & physics
#define MAX_ENTITIES                    10

#define TILE_SIZE                       16
#define HALF_TILE                       (TILE_SIZE/2)
#define QUARTER_TILE                    (TILE_SIZE/4)

#define PHYS_SCALE                      1
#define DEFAULT_ACCELERATION            (TILE_SIZE/64.0)
#define DEFAULT_MAX_FORCE               (TILE_SIZE/64.0)
#define DEFAULT_MAX_VELOCITY            (TILE_SIZE/8.0)
#define DEFAULT_WALK_SPEED              DEFAULT_MAX_VELOCITY
#define DEFAULT_RUN_SPEED               (DEFAULT_WALK_SPEED * 1.75)
#define DEFAULT_ANGULAR_ACCELERATION    degToRad(0.5)
#define DEFAULT_MAX_ANGULAR_VELOCITY    degToRad(4.0)

#define FIRE_COOLDOWN_TIME              3
#define EXPLOSION_MAGNITUDE             30
#define INACCURACY                      0.1

// Colors
#define RGBA_BLACK                      (uint8_t[]) {0,     0,      0,      0   }
#define RGBA_GRAY                       (uint8_t[]) {128,   128,    128,    0   }
#define RGBA_DARK_GRAY                  (uint8_t[]) {48,    48,     48,     0   }
#define RGBA_WHITE                      (uint8_t[]) {255,   255,    255,    0   }
#define RGBA_RED                        (uint8_t[]) {255,   0,      0,      0   }
#define RGBA_GREEN                      (uint8_t[]) {0,     255,    0,      0   }
#define RGBA_DARK_GREEN                 (uint8_t[]) {16,    64,     32,     0   }
#define RGBA_BLUE                       (uint8_t[]) {0,     0,      255,    0   }
#define RGBA_LIGHT_BLUE                 (uint8_t[]) {64,    128,    255,    0   }
#define RGBA_YELLOW                     (uint8_t[]) {255,   255,    0,      0   }
#define RGBA_PURPLE                     (uint8_t[]) {255,   0,      255,    0   }
#define RGBA_BROWN                      (uint8_t[]) {32,    24,     16,     0   }
#define RGBA_DARK_BLUE                  (uint8_t[]) {0,     0,      64,     0   }

// Rendering
#define FULL_SCREEN                     0
#define BORDERLESS                      0
#define RES_SCALE                       1
#define SCREEN_WIDTH                    320
#define SCREEN_HEIGHT                   240
#define SCREEN_RATIO                    ((float)SCREEN_HEIGHT / SCREEN_WIDTH)
// fov
#define DEFAULT_FOV                     1.0
#define DEFAULT_H_FOV                   DEFAULT_FOV
#define DEFAULT_V_FOV                   DEFAULT_FOV
// textures
#define FLOOR_ENABLE                    1
#define TEX_ENABLE                      1
#define TEX_SIZE                        64
// light
#define LIGHT_ENABLE                    1
#define MIN_LIGHT                       64
#define MAX_LIGHT                       255
// fog
#define FOG_ENABLE                      1
#define WALL_FOG                        FOG_ENABLE
#define FLOOR_FOG                       FOG_ENABLE
#define CEILING_FOG                     FOG_ENABLE
#define FOG_DISTANCE                    DRAW_DISTANCE
#define FOG_COLOR                       RGBA_BLACK
// misc rendering
#define DRAW_DISTANCE                   (TILE_SIZE * 20)
#define BACK_CLIP_PLANE                 1
#define WALL_COLOR                      RGBA_GRAY
#define FLOOR_COLOR                     RGBA_DARK_GRAY
#define CEILING_COLOR                   RGBA_DARK_BLUE
#define BACKGROUND_TOP                  1
#define BACKGROUND_BOTTOM               1
// head bop
#define BOP_SPEED                       12
#define BOP_HEIGHT                      1.3
#define BOP_Z_INC                       0.075
// water
#define UNDERWATER                      0
#define WAVE_HEIGHT                     2
#define WAVE_WIDTH                      4
#define WAVE_SPEED                      1.5
#define LIQUID_WAVE_HEIGHT              (TEX_SIZE / 16)
#define LIQUID_WAVE_WIDTH               (360 / TEX_SIZE)
#define LIQUID_WAVE_SPEED               5
// particle effects etc
#define FIRE_COLOR1                     RGBA_YELLOW
#define FIRE_COLOR2                     RGBA_RED

int quit;
int entityCount = 0;

int texSize       = TEX_SIZE;
int tileSize      = TILE_SIZE;
int halfTile      = HALF_TILE;
int quarterTile   = QUARTER_TILE;
int floorEnable   = FLOOR_ENABLE;
int wallTex       = TEX_ENABLE;
int floorTex      = TEX_ENABLE;
int ceilingTex    = TEX_ENABLE;
int wallFog       = WALL_FOG;
int floorFog      = FLOOR_FOG;
int ceilingFog    = CEILING_FOG;
int lightEnable   = LIGHT_ENABLE;
int minLight      = MIN_LIGHT;
int maxLight      = MAX_LIGHT;
int drawDistance  = DRAW_DISTANCE;
int backClipPlane = BACK_CLIP_PLANE;

int playerId;
int cameraId;
int mouseX,     mouseY;
int camera2D_X, camera2D_Y;
int crosshairX, crosshairY;

long long tick;

const int screenWidth   = SCREEN_WIDTH;
const int screenHeight  = SCREEN_HEIGHT;
int backgroundTop    = 0;
int backgroundBottom = 0;
int backgroundWidth;
int backgroundHeight;
SDL_Rect BackgroundSrcRect;
SDL_Rect BackgroundDstRect;

SDL_Window*     Window2D;
SDL_Window*     Window3D;
SDL_Renderer*   Renderer2D;
SDL_Renderer*   Renderer3D;
SDL_Texture*    Texture2D;
SDL_Texture*    Texture3D;
SDL_Texture*    OffScreen3D;
SDL_Texture*    BackgroundTexture;

SDL_BlendMode   FogBlendMode;

enum TILE_TYPES
{
    TILE_OBSTACLE    = (1 << 0),
    TILE_PARTIAL_OBS = (1 << 1),
    TILE_OCCLUSION   = (1 << 2),
    TILE_PARTIAL_OCC = (1 << 3),
    TILE_LIQUID      = (1 << 4),
    TILE_TOGGLE      = (1 << 5),
    TILE_UNUSED_FLAG = (1 << 6),
    TILE_LIT         = (1 << 7),
    TILE_FLAGS       = 8,
};

enum COMMAND_TYPES
{
    COMMAND_MOVE_UP     = 1 << 0,
    COMMAND_MOVE_DOWN   = 1 << 1,
    COMMAND_MOVE_LEFT   = 1 << 2,
    COMMAND_MOVE_RIGHT  = 1 << 3,
    COMMAND_TURN_UP     = 1 << 4,
    COMMAND_TURN_DOWN   = 1 << 5,
    COMMAND_TURN_LEFT   = 1 << 6,
    COMMAND_TURN_RIGHT  = 1 << 7,
    COMMAND_MOVE_RUN    = 1 << 8,
    MOVE_COMMANDS       = 0b111111111,
    COMMAND_FIRE        = 1 << 9
};

#define NUM_INPUT_CHANNELS 1

enum OBJECT_TYPES
{
    OBJECT_PLAYER,
    OBJECT_LIGHT
};

struct Object
{
    char type;
    int x, y;

    union
    {
        struct {float angle;};
        struct {int brightness, range;};
    };
};

struct Board
{
    int             w, h, size, numObjects;
    int             lightEnable, wallTex, floorTex, ceilingTex, wallFog, floorFog, ceilingFog, backgroundTop, backgroundBottom;
    int             fogDistance, drawDistance, backClipPlane;
    int             tileSize, texSize, minLight, maxLight;
    int             wallColor   [3];
    int             floorColor  [3];
    int             ceilingColor[3];
    int             fogColor    [3];
    char            textureFile [BUFFER_SIZE];
    char            bgFile      [BUFFER_SIZE];
    uint16_t*       tileMap;
    uint8_t*        lightMap;
    struct Object*  objects;
};

int tileAtPos(struct Board* board_, int x, int y)
{
    return board_->tileMap[((y/tileSize) * board_->w) + (x/tileSize)];
}

float randomFloat(float min, float max)
{
    return min + ((float)rand() / (float)RAND_MAX) * (max-min);
}

void addToColor(uint8_t target[], int color[])
{
    int tempColor[4];

    for (int i = 0; i < 4; i++)
    {
        tempColor[i] = (int)(target[i]) + color[i];

        if (tempColor[i] < 0)
            target[i] = 0;
        else if (tempColor[i] > 255)
            target[i] = 255;
        else
            target[i] = (uint8_t)(tempColor[i]);
    }
}

uint32_t blendColors(uint8_t color1[], uint8_t color2[])
{
    union Color
    {
        uint8_t values[4];
        uint32_t bytes;
    } color;

    for (int i = 0; i < 4; i++)
        color.values[i] = color1[i]/2 + color2[i]/2;

    return color.bytes;
}

uint32_t randomColor()
{
    union Color
    {
        uint8_t values[4];
        uint32_t bytes;
    } color;

    for (int i = 0; i < 4; i++)
        color.values[i] = rand() % 255;

    return color.bytes;
}

struct Vec3
{
    float x, y, z;
}
ZeroVec3 = {0, 0, 0};

struct Vec2
{
    float x, y;
}
ZeroVec2 = {0, 0};

struct Vec2 rotateVec2(const struct Vec2 v, float angle)
{
    struct Vec2 rotVec;
    rotVec.x = v.x*cos(angle) - v.y*sin(angle);
    rotVec.y = v.x*sin(angle) + v.y*cos(angle);

    return rotVec;
}

struct Vec2 randomVec2(float min, float max)
{
    struct Vec2 v = {randomFloat(min, max), 0};
    v = rotateVec2(v, randomFloat(0, 360));

    return v;
}

struct Vec2 randomVec2Rect(float x, float y)
{
    struct Vec2 v;
    v.x = randomFloat(0, x);
    v.y = randomFloat(0, y);

    return v;
}

struct Vec2 newVec2(float length, float angle)
{
    return rotateVec2((struct Vec2){length, 0}, angle);
}

struct Particle
{
    struct Vec2 origin;
    struct Vec2 velocity;
    struct Vec2 velChange;
    uint8_t color1[4];
    uint8_t color2[4];
    int lifeTime;
    int lifeLeft;
};

int numParticles;
struct Particle* ParticleArray;

#define maxParticles    1000

int initParticleArray()
{
    numParticles = 0;
    ParticleArray = calloc(maxParticles, sizeof(struct Particle));

    if (ParticleArray == NULL)
    {
        printf("calloc() failed for ParticleArray\n");
        return 1;
    }

    return 0;
}

int makeParticle(struct Vec2 origin_, struct Vec2 velocity_, struct Vec2 velChange_, uint8_t color1_[], uint8_t color2_[], int lifeTime_)
{
    int i;

    if (numParticles >= maxParticles)
        i = 0;
    else
        i = numParticles++;

    ParticleArray[i] = (struct Particle)
    {
        .origin    = origin_,
        .velocity  = velocity_,
        .velChange = velChange_,
        .lifeTime  = lifeTime_,
        .lifeLeft  = lifeTime_
    };

    memcpy(ParticleArray[i].color1, color1_, 4);
    memcpy(ParticleArray[i].color2, color2_, 4);

    return i;
}

void killParticle(int index)
{
    if (index < numParticles)
    {
        ParticleArray[index] = ParticleArray[numParticles-1];
        numParticles--;
    }
}

void renderParticles(SDL_Renderer* renderer, struct Board* board_)
{
    uint8_t newColor[3];
    struct Particle* p;

    for (int i = 0; i < numParticles; i++)
    {
        p = &(ParticleArray[i]);

        if (p->lifeLeft-- > 0)
        {
            if (tileAtPos(board_, p->origin.x, p->origin.y) & TILE_OBSTACLE)
            {
                killParticle(i);
                continue;
            }

            addVec2(p->velocity, p->velChange);
            addVec2(p->origin, p->velocity);

            for (int c = 0; c < 3; c++)
                newColor[c] = (p->lifeLeft * p->color1[c] + (p->lifeTime - p->lifeLeft) * p->color2[c]) / p->lifeTime;

            SDL_SetRenderDrawColor(renderer, colorArg3(newColor), 255);
            SDL_RenderDrawPoint(renderer, (int)(p->origin.x) + camera2D_X, (int)(p->origin.y) + camera2D_Y);
        }
        else
            killParticle(i);
    }
}

void spawnFlame(struct Vec2 origin, struct Vec2 moveVector, float scale, float randomness, int life)
{
    scaleVec2(moveVector, scale);
    struct Vec2 particleVelocity = moveVector;
    addVec2(particleVelocity, randomVec2(0, randomness));
    makeParticle(origin, particleVelocity, ZeroVec2, FIRE_COLOR1, FIRE_COLOR2, life);
}

void spawnExplosion(struct Vec2 origin, struct Vec2 moveVector, int magnitude)
{
    struct Vec2 particleVelocity;

    for (int i = 0; i < magnitude; i++)
    {
        particleVelocity = moveVector;
        addVec2(particleVelocity, randomVec2(magnitude/100.0, 0));
        makeParticle(origin, particleVelocity, ZeroVec2, FIRE_COLOR1, FIRE_COLOR2, rand() % magnitude);
    }
}

enum DIRECTIONS
{
    NORTH   = 1 << 0,
    SOUTH   = 1 << 1,
    WEST    = 1 << 2,
    EAST    = 1 << 3
};

struct Velocity
{
    float x, y;
    float maxVelSquared;
    float friction;
}
VelocityDefault = {0, 0, pow(DEFAULT_MAX_VELOCITY, 2), DEFAULT_ACCELERATION/2};

struct Force
{
    float x, y;
    float maxX, maxY;
    float maxForceSquared;
}
ForceDefault = {0, 0, DEFAULT_MAX_FORCE, DEFAULT_MAX_FORCE, pow(DEFAULT_MAX_FORCE,2)};

struct Rotation
{
    float x, y;
    float angle;
}
RotationDefault = {0, -1, degToRad(90)};

struct Torque
{
    float angVel;
    float maxAngVel;
}
TorqueDefault = {0, DEFAULT_MAX_ANGULAR_VELOCITY};

void typeSanityCheck()
{
    short       shortType;
    int         intType;
    long        longType;
    long long   longLongType;
    float       floatType;
    double      doubleType;
    long double longDoubleType;

    // sizeof evaluates the size of a variable
    printf("Size of short:\t\t%d bits\n",        8 * sizeof(shortType));
    printf("Size of int:\t\t%d bits\n",          8 * sizeof(intType));
    printf("Size of long:\t\t%d bits\n",         8 * sizeof(longType));
    printf("Size of long long:\t%d bits\n",      8 * sizeof(longLongType));
    printf("Size of float:\t\t%d bits\n",        8 * sizeof(floatType));
    printf("Size of double:\t\t%d bits\n",       8 * sizeof(doubleType));
    printf("Size of long double:\t%d bits\n",    8 * sizeof(longDoubleType));
}

enum COLLIDABLE_TYPES
{
    COLLIDABLE_TILE,
    COLLIDABLE_POINT,
    COLLIDABLE_RECT,
    COLLIDABLE_CIRCLE
};

struct Collidable
{
    uint8_t type;
    uint8_t collision;
    float w, h;
};

enum CONTROL_TYPES
{
    CONTROL_AI          = 1 << 0,
    CONTROL_KEYBOARD    = 1 << 1,
    CONTROL_MOUSELOOK   = 1 << 2,
    CONTROL_DIRECTIONAL = 1 << 3,
    CONTROL_ROTATIONAL  = 1 << 4
};

struct Control
{
    uint8_t type, inputChannel;
    uint16_t commands;
};

struct AI
{
    uint8_t state;
    uint16_t commands;
};

enum VISIBLE_TYPES
{
    VISIBLE_HITBOX       = 1 << 0,  // enable to see hitbox
    VISIBLE_FILLED       = 1 << 1,  // filled flag for primitive shapes
    VISIBLE_DOT          = 1 << 2,  // .
    VISIBLE_LINE         = 2 << 2,  // _
    VISIBLE_TRIANGLE     = 3 << 2,  // |>
    VISIBLE_RECTANGLE    = 4 << 2,  // []
    VISIBLE_CIRCLE       = 5 << 2,  // O
    VISIBLE_POLYGON      = 6 << 2,  // <>
    VISIBLE_SPRITE       = 7 << 2,  // #
    VISIBLE_MESH         = 8 << 2   // <_>
};

struct Visible
{
    uint8_t type;
    uint8_t color[4];
    uint16_t animation;
    uint16_t frame;
};

enum COMPONENT_TYPES
{
    TYPE_POSITION   = 1 << 1,
    TYPE_TRANSFORM  = 1 << 2,
    TYPE_VELOCITY   = 1 << 3,
    TYPE_ROTATION   = 1 << 4,
    TYPE_FORCE      = 1 << 5,
    TYPE_TORQUE     = 1 << 6,
    TYPE_COLLIDABLE = 1 << 7,
    TYPE_CONTROL    = 1 << 8,
    TYPE_AI         = 1 << 9,
    TYPE_VISIBLE    = 1 << 10,
};

uint64_t*           EntityArray;
struct Vec2*        PositionArray;
struct Vec2*        TransformArray;
struct Velocity*    VelocityArray;
struct Rotation*    RotationArray;
struct Force*       ForceArray;
struct Torque*      TorqueArray;
struct Collidable*  CollidableArray;
struct Control*     ControlArray;
struct AI*          AIArray;
struct Visible*     VisibleArray;
struct Camera*      CameraArray;

void initArrays()
{
    EntityArray     = calloc(MAX_ENTITIES, sizeof(uint16_t));
    PositionArray   = calloc(MAX_ENTITIES, sizeof(struct Vec2));
    TransformArray  = calloc(MAX_ENTITIES, sizeof(struct Vec2));
    VelocityArray   = calloc(MAX_ENTITIES, sizeof(struct Velocity));
    RotationArray   = calloc(MAX_ENTITIES, sizeof(struct Rotation));
    ForceArray      = calloc(MAX_ENTITIES, sizeof(struct Force));
    TorqueArray     = calloc(MAX_ENTITIES, sizeof(struct Torque));
    CollidableArray = calloc(MAX_ENTITIES, sizeof(struct Collidable));
    ControlArray    = calloc(MAX_ENTITIES, sizeof(struct Control));
    AIArray         = calloc(MAX_ENTITIES, sizeof(struct AI));
    VisibleArray    = calloc(MAX_ENTITIES, sizeof(struct Visible));

    initParticleArray();
}

void renderVisible(SDL_Renderer* renderer_)
{
    int componentMask = TYPE_POSITION | TYPE_VISIBLE;
    int i, length = 10;

    for (i = 0; i < entityCount; i++)
    {
        if ((EntityArray[i] & componentMask) == componentMask)
        {
            if (VisibleArray[i].type == VISIBLE_HITBOX && EntityArray[i] & TYPE_COLLIDABLE)
            {
                if (CollidableArray[i].type == COLLIDABLE_TILE)
                {
                    length = tileSize;

                    SDL_Rect TempRect =
                    {
                        PositionArray[i].x - halfTile + camera2D_X,
                        PositionArray[i].y - halfTile + camera2D_Y,
                        tileSize, tileSize
                    };

                    SDL_SetRenderDrawColor(renderer_, VisibleArray[i].color[0], VisibleArray[i].color[1], VisibleArray[i].color[2], VisibleArray[i].color[3]);
                    SDL_RenderDrawRect(renderer_, &TempRect);
                }

                if (EntityArray[i] & TYPE_ROTATION)
                {
                    // drawLine(from A(a,x) to B(length,rotation)
                    SDL_RenderDrawLine(renderer_,
                    PositionArray[i].x + camera2D_X,
                    PositionArray[i].y + camera2D_Y,
                    PositionArray[i].x + RotationArray[i].x*length + camera2D_X,
                    PositionArray[i].y + RotationArray[i].y*length + camera2D_Y);
                }
                else if (EntityArray[i] & TYPE_CONTROL)
                {
                    ;
                    // draw arrow to where it's trying to move to
                }
            }
        }
    }
}

void renderCrosshair(SDL_Renderer* renderer_, int i)
{
    int gap      = 8 - (tick%32) / 4;
    int length   = 6;
    int distance = 50;

    if (ControlArray[i].type & CONTROL_MOUSELOOK)
    {
        crosshairX = mouseX/RES_SCALE;
        crosshairY = mouseY/RES_SCALE;
    }
    else
    {
        crosshairX = PositionArray[i].x + RotationArray[i].x*distance + camera2D_X;
        crosshairY = PositionArray[i].y + RotationArray[i].y*distance + camera2D_Y;
    }

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer_, crosshairX-gap, crosshairY, crosshairX-length-gap, crosshairY);
    SDL_RenderDrawLine(renderer_, crosshairX+gap, crosshairY, crosshairX+length+gap, crosshairY);
    SDL_RenderDrawLine(renderer_, crosshairX, crosshairY-gap, crosshairX, crosshairY-length-gap);
    SDL_RenderDrawLine(renderer_, crosshairX, crosshairY+gap, crosshairX, crosshairY+length+gap);
}

void doVelocity()
{
    int i;
    float angle, excessVel, excessVelSquared;
    struct Vec2 friction = ZeroVec2;

    for (i = 0; i < entityCount; i++)
    {
        if (EntityArray[i] & TYPE_VELOCITY)
        {
            if (EntityArray[i] & TYPE_CONTROL && ControlArray[i].type & CONTROL_ROTATIONAL)
            {
                VelocityArray[i].x += RotationArray[i].x * -(ForceArray[i].y) + RotationArray[i].y * -(ForceArray[i].x);
                VelocityArray[i].y += RotationArray[i].y * -(ForceArray[i].y) + RotationArray[i].x * ForceArray[i].x;
            }
            else
            {
                VelocityArray[i].x += ForceArray[i].x;
                VelocityArray[i].y += ForceArray[i].y;
            }

            if (VelocityArray[i].x || VelocityArray[i].y)
            {
                angle            = getVec2Angle(VelocityArray[i]);
                friction         = newVec2(VelocityArray[i].friction, angle);
                excessVelSquared = pow(VelocityArray[i].x, 2) + pow(VelocityArray[i].y, 2) - VelocityArray[i].maxVelSquared;

                if (excessVelSquared > 0)
                {
                    excessVel = sqrt(excessVelSquared);
                    addVec2(friction, newVec2(excessVel, angle));
                }

                subtractVec2(VelocityArray[i], friction);

                if (fabs(VelocityArray[i].x) < fabs(friction.x))
                    VelocityArray[i].x = 0;

                if (fabs(VelocityArray[i].y) < fabs(friction.y))
                    VelocityArray[i].y = 0;
            }
        }
    }
}

void doRotationAndTorque()
{
    int i;

    for (i = 0; i < entityCount; i++)
    {
        if (EntityArray[i] & TYPE_ROTATION)
        {
            if (ControlArray[i].type & CONTROL_MOUSELOOK && i == playerId)   // were not even checking if the entity has control tho, also reading mouse input shouldnt be here
            {
                RotationArray[i].angle = getVec2Angle(((struct Vec2)
                {
                    mouseX/RES_SCALE - camera2D_X - PositionArray[i].x,
                    -(mouseY/RES_SCALE - camera2D_Y - PositionArray[i].y)
                }));
            }
            else if (EntityArray[i] & TYPE_TORQUE)
                RotationArray[i].angle += TorqueArray[i].angVel;

            //RotationArray[i].x      =  cos(RotationArray[i].angle);
            //RotationArray[i].y      = -sin(RotationArray[i].angle);
            setVec2Angle(RotationArray[i], RotationArray[i].angle);
        }
    }
}

uint8_t collisionTestTileX(struct Board* board_, float x, float y, uint8_t dirX)
{
    uint16_t tileCheck;
    int step = (dirX == WEST) ? 0 : tileSize-1;

    tileCheck  = tileAtPos(board_, x+step, y);
    tileCheck |= tileAtPos(board_, x+step, y+tileSize-1);

    if (tileCheck & TILE_OBSTACLE)
        return dirX;
    else
        return 0;
}

uint8_t collisionTestTileY(struct Board* board_, float x, float y, uint8_t dirY)
{
    uint16_t tileCheck;
    int step = (dirY == NORTH) ? 0 : tileSize-1;

    tileCheck  = tileAtPos(board_, x, y+step);
    tileCheck |= tileAtPos(board_, x+tileSize-1, y+step);

    if (tileCheck & TILE_OBSTACLE)
        return dirY;
    else
        return 0;
}

void resolveCollisionTile(int i, char collision_)
{
    if (collision_ & WEST)
        TransformArray[i].x = -(int)(PositionArray[i].x + halfTile) % tileSize;
    else if (collision_ & EAST)
        TransformArray[i].x = tileSize-(int)(PositionArray[i].x + halfTile-1) % tileSize-1;

    if (collision_ & NORTH)
        TransformArray[i].y = -(int)(PositionArray[i].y + halfTile) % tileSize;
    else if (collision_ & SOUTH)
        TransformArray[i].y = tileSize-(int)(PositionArray[i].y + halfTile-1) % tileSize-1;
}

uint8_t collideTile(struct Board* board_, int i)
{
    float x = PositionArray[i].x + TransformArray[i].x - halfTile;
    float y = PositionArray[i].y + TransformArray[i].y - halfTile;

    uint8_t dirX = 0;
    uint8_t dirY = 0;
    uint8_t collision = 0;

    if (TransformArray[i].x)
    {
        dirX = (TransformArray[i].x < 0) ? WEST : EAST;
        collision |= collisionTestTileX(board_, x, y, dirX);
    }

    if (TransformArray[i].y)
    {
        dirY = (TransformArray[i].y < 0) ? NORTH : SOUTH;
        collision |= collisionTestTileY(board_, x, y, dirY);
    }

    if (collision & (WEST | EAST) && collision & (NORTH | SOUTH))
    {
        y = PositionArray[i].y - halfTile;
        collision = collisionTestTileX(board_, x, y, dirX);

        if (collision) // if there was x collision
        {
            x = PositionArray[i].x - halfTile;
            y = PositionArray[i].y + TransformArray[i].y - halfTile;
            collision = collisionTestTileY(board_, x, y, dirY);

            if (collision) // if there was y collision
                collision = dirX | dirY;
            else
                collision = dirX;
        }
        else
            collision = dirY;
    }

    return collision;
}

void doCollidable(struct Board* board_)
{
    uint16_t componentMask = TYPE_COLLIDABLE | TYPE_TRANSFORM;
    int i, collision;

    for (i = 0; i < entityCount; i++)
    {
        if ((EntityArray[i] & componentMask) == componentMask && (TransformArray[i].x || TransformArray[i].y))
        {
            CollidableArray[i].collision = 0;

            if (CollidableArray[i].type == COLLIDABLE_TILE)
            {
                collision = collideTile(board_, i);
                resolveCollisionTile(i, collision);
                CollidableArray[i].collision = collision;
            }

            if (CollidableArray[i].collision & (WEST | EAST))
                VelocityArray[i].x = 0;

            if (CollidableArray[i].collision & (NORTH | SOUTH))
                VelocityArray[i].y = 0;
        }
    }
}

void doTransform()
{
    uint16_t componentMask = TYPE_TRANSFORM | TYPE_VELOCITY;
    int i;

    for (i = 0; i < entityCount; i++)
    {
        if ((EntityArray[i] & componentMask) == componentMask)
        {
            if (PHYS_SCALE == 1)
                setVec2(TransformArray[i], VelocityArray[i]);
            else
                scaleVec2(TransformArray[i], PHYS_SCALE);
        }
    }
}

void doPosition()
{
    uint16_t componentMask = TYPE_POSITION | TYPE_TRANSFORM;
    int i;

    for (i = 0; i < entityCount; i++)
    {
        if ((EntityArray[i] & componentMask) == componentMask)
            addVec2(PositionArray[i], TransformArray[i]);
    }
}

struct TileType
{
    char symbol;
    uint16_t data;
};

struct TileTypeArray
{
    int numTypes;
    struct TileType* TileTypes;
};

void loadTileTypes(FILE* mapData_, struct TileTypeArray* TileTypeArray_)
{
    long typeDataOffset;
    int i, tileGfxId = 0;
    char buffer[BUFFER_SIZE];
    char tileFlags[TILE_FLAGS];
    uint8_t flagBits = 0;
    char* c;

    fgetc(mapData_);
    typeDataOffset = ftell(mapData_);
    TileTypeArray_->numTypes = 0;

    while (fgets(buffer, BUFFER_SIZE, mapData_) != NULL)
    {
        if (buffer[0] == '\n')
            break;
        else
            TileTypeArray_->numTypes++;
    }

    fseek(mapData_, typeDataOffset, SEEK_SET);
    TileTypeArray_->TileTypes = calloc(TileTypeArray_->numTypes, sizeof(struct TileType));

    for (i = 0; i < TileTypeArray_->numTypes; i++)
    {
        flagBits = 0;
        memset(tileFlags, 0, TILE_FLAGS);
        fscanf(mapData_, "%c %d %s\n", &(TileTypeArray_->TileTypes[i].symbol), &tileGfxId, tileFlags);

        for (c = tileFlags; *c != '\0'; c++)
        {
            switch (*c)
            {
            case 'w':
                flagBits |= TILE_OBSTACLE;
                break;
            case 'o':
                flagBits |= TILE_OCCLUSION;
                break;
            case 'l':
                flagBits |= TILE_LIQUID;
                break;
            default:
                break;
            }
        }

        TileTypeArray_->TileTypes[i].data = (tileGfxId << TILE_FLAGS) + flagBits;

        printf("Symbol: %c\t", TileTypeArray_->TileTypes[i].symbol);
        printf("Graphic: %d\t", TileTypeArray_->TileTypes[i].data >> TILE_FLAGS);
        printf("Flags: %d\n", (TileTypeArray_->TileTypes[i].data & ((1 << TILE_FLAGS)-1)));
    }
}

uint16_t getTileType(char c, struct TileTypeArray* TileTypeArray_)
{
    int i;
    uint16_t tileType = 0;

    for (i = 0; i < TileTypeArray_->numTypes; i++)
    {
        if (TileTypeArray_->TileTypes[i].symbol == c)
            tileType = TileTypeArray_->TileTypes[i].data;
    }

    return tileType;
}

void loadTileMap(FILE* mapData_, struct TileTypeArray* TileTypeArray_, struct Board* board_)
{
    int i;
    char c;
    uint16_t tileType = 0;

    fgetc(mapData_);

    for (i = 0; i < board_->size; i++)
    {
        c = fgetc(mapData_);

        if (c == '\n')
        {
            putchar('\n');
            i--;
        }
        else
        {
            putchar(c);
            tileType = getTileType(c, TileTypeArray_);
            board_->tileMap[i] = tileType;
        }
    }
    putchar('\n');
}

void loadObjects(FILE* mapData_, struct Board* board_)
{
    int i;
    char cPrev, c;
    long objectDataOffset;
    char buffer[BUFFER_SIZE];

    cPrev = fgetc(mapData_);
    objectDataOffset = ftell(mapData_);
    board_->numObjects = 0;

    while (cPrev != EOF)
    {
        c = fgetc(mapData_);

        if (cPrev == '\n')
        {
            if (c == '.')
                board_->numObjects++;
            else
                break;
        }
        cPrev = c;
    }

    printf("Objects: %d\n", board_->numObjects);
    board_->objects = calloc(board_->numObjects, sizeof(struct Object));
    fseek(mapData_, objectDataOffset, SEEK_SET);
    i = 0;

    while ((c = fgetc(mapData_)) != EOF)
    {
        if (c == '.')
        {
            fscanf(mapData_, "%s", buffer);

            if (!strcmp(buffer, "player"))
            {
                board_->objects[i].type = OBJECT_PLAYER;
                fscanf(mapData_, "%d %d %f", &(board_->objects[i].x), &(board_->objects[i].y), &(board_->objects[i].angle));
                printf("player %d %d %f\n", (board_->objects[i].x), (board_->objects[i].y), (board_->objects[i].angle));
            }
            else if (!strcmp(buffer, "light"))
            {
                board_->objects[i].type = OBJECT_LIGHT;
                fscanf(mapData_, "%d %d %d %d", &(board_->objects[i].x), &(board_->objects[i].y), &(board_->objects[i].brightness), &(board_->objects[i].range));
                printf("light %d %d %d %d\n", (board_->objects[i].x), (board_->objects[i].y), (board_->objects[i].brightness), (board_->objects[i].range));
            }

            if (++i >= board_->numObjects)
                break;
        }
    }
}

struct Board* loadMap(char* filename)
{
    FILE* MapData                           = fopen(filename, "r");
    struct Board* newBoard                  = calloc(1, sizeof(struct Board));
    struct TileTypeArray* newTileTypeArray  = calloc(1, sizeof(struct TileTypeArray));
    char buffer[BUFFER_SIZE];
    char c;

    setColor(newBoard->wallColor,       WALL_COLOR);
    setColor(newBoard->floorColor,      FLOOR_COLOR);
    setColor(newBoard->ceilingColor,    CEILING_COLOR);
    setColor(newBoard->fogColor,        FOG_COLOR);
    newBoard->wallFog                 = WALL_FOG;
    newBoard->floorFog                = FLOOR_FOG;
    newBoard->ceilingFog              = CEILING_FOG;
    newBoard->wallTex                 = TEX_ENABLE;
    newBoard->floorTex                = TEX_ENABLE;
    newBoard->ceilingTex              = TEX_ENABLE;
    newBoard->lightEnable             = LIGHT_ENABLE;
    newBoard->minLight                = MIN_LIGHT;
    newBoard->maxLight                = MAX_LIGHT;
    newBoard->fogDistance             = FOG_DISTANCE;
    newBoard->drawDistance            = DRAW_DISTANCE;
    newBoard->backClipPlane           = BACK_CLIP_PLANE;
    newBoard->backgroundTop           = BACKGROUND_TOP;
    newBoard->backgroundBottom        = BACKGROUND_BOTTOM;
    newBoard->tileSize                = TILE_SIZE;
    newBoard->texSize                 = TEX_SIZE;

    while ((c = fgetc(MapData)) != EOF)
    {
        if (c == '$')
        {
            fscanf(MapData, "%s", buffer);
            // texture settings
            if (!strcmp(buffer, "walltex"))
                fscanf(MapData, "%d", &(newBoard->wallTex));
            else if (!strcmp(buffer, "floortex"))
                fscanf(MapData, "%d", &(newBoard->floorTex));
            else if (!strcmp(buffer, "ceilingtex"))
                fscanf(MapData, "%d", &(newBoard->ceilingTex));
            else if (!strcmp(buffer, "texturesize"))
                fscanf(MapData, "%d", &(newBoard->texSize));
            else if (!strcmp(buffer, "texturefile"))
                fscanf(MapData, "%s", newBoard->textureFile);
            // background settings
            else if (!strcmp(buffer, "bgbottom"))
                fscanf(MapData, "%d", &(newBoard->backgroundBottom));
            else if (!strcmp(buffer, "bgtop"))
                fscanf(MapData, "%d", &(newBoard->backgroundTop));
            else if (!strcmp(buffer, "bgfile"))
                fscanf(MapData, "%s", newBoard->bgFile);
            // colors & distances
            else if (!strcmp(buffer, "wallcolor"))
                fscanf(MapData, "%d %d %d", &(newBoard->wallColor[0]), &(newBoard->wallColor[1]), &(newBoard->wallColor[2]));
            else if (!strcmp(buffer, "floorcolor"))
                fscanf(MapData, "%d %d %d", &(newBoard->floorColor[0]), &(newBoard->floorColor[1]), &(newBoard->floorColor[2]));
            else if (!strcmp(buffer, "ceilingcolor"))
                fscanf(MapData, "%d %d %d", &(newBoard->ceilingColor[0]), &(newBoard->ceilingColor[1]), &(newBoard->ceilingColor[2]));
            else if (!strcmp(buffer, "drawdistance"))
                fscanf(MapData, "%d", &(newBoard->drawDistance));
            else if (!strcmp(buffer, "backclipplane"))
                fscanf(MapData, "%d", &(newBoard->backClipPlane));
            // fog settings
            else if (!strcmp(buffer, "wallfog"))
                fscanf(MapData, "%d", &(newBoard->wallFog));
            else if (!strcmp(buffer, "floorfog"))
                fscanf(MapData, "%d", &(newBoard->floorFog));
            else if (!strcmp(buffer, "ceilingfog"))
                fscanf(MapData, "%d", &(newBoard->ceilingFog));
            else if (!strcmp(buffer, "fogdistance"))
                fscanf(MapData, "%d", &(newBoard->fogDistance));
            else if (!strcmp(buffer, "fogcolor"))
                fscanf(MapData, "%d %d %d", &(newBoard->fogColor[0]), &(newBoard->fogColor[1]), &(newBoard->fogColor[2]));
            // light settings
            else if (!strcmp(buffer, "lightenable"))
                fscanf(MapData, "%d", &(newBoard->lightEnable));
            else if (!strcmp(buffer, "minlight"))
                fscanf(MapData, "%d", &(newBoard->minLight));
            else if (!strcmp(buffer, "maxlight"))
                fscanf(MapData, "%d", &(newBoard->maxLight));
            // tile and object data
            else if (!strcmp(buffer, "tilesize"))
                fscanf(MapData, "%d", &(newBoard->tileSize));
            else if (!strcmp(buffer, "tiletypes"))
                loadTileTypes(MapData, newTileTypeArray);
            else if (!strcmp(buffer, "mapsize") && newBoard->size == 0)
            {
                fscanf(MapData, "%d %d", &(newBoard->w), &(newBoard->h));
                newBoard->size = newBoard->w * newBoard->h;
                newBoard->tileMap = malloc(sizeof(uint16_t) * newBoard->size);
                newBoard->lightMap = malloc(sizeof(uint8_t) * newBoard->size);
            }
            else if (!strcmp(buffer, "tilemap"))
                loadTileMap(MapData, newTileTypeArray, newBoard);
            else if (!strcmp(buffer, "objects"))
                loadObjects(MapData, newBoard);
        }
    }

    free(newTileTypeArray->TileTypes);
    free(newTileTypeArray);
    //fclose(filename);

    return newBoard;
}

int lightTile(struct Board* board_, int x, int y, int brightness, float dist, float range)
{
    int newBrightness;
    uint16_t* tile;

    tile = &(tileAt(board_, x, y));

    if (((*tile) & TILE_LIT) == 0)
    {
        newBrightness = board_->lightMap[y*board_->w + x] + ((range-dist) / range) * brightness;

        if (newBrightness > maxLight)
            newBrightness = maxLight;

        board_->lightMap[y*board_->w + x] = newBrightness;
        (*tile) |= TILE_LIT;
    }

    if ((*tile) & TILE_OCCLUSION)
        return 1;

    return 0;
}

void lightRay(struct Board* board_, int ax, int ay, int bx, int by, int brightness, float range)
{
	int i, x, y, px, py;
	float dist = 0;

	const int dx = bx - ax;
	const int dy = by - ay;
	const int dx_abs = abs(dx);
	const int dy_abs = abs(dy);
	const int x_sign = sign(dx);
	const int y_sign = sign(dy);

	x = dy_abs/2;
	y = dx_abs/2;
	px = ax;
	py = ay;

	if (dx_abs > dy_abs)
	{
		for (i = 0; i <= dx_abs; i++)
		{
            if ((dist = sqrt(pow(ax-px, 2) + pow(ay-py, 2))) > range)
                break;
            if (lightTile(board_, px, py, brightness, dist, range))
                break;
            if (((tileAt(board_, px+x_sign, py)) & TILE_OCCLUSION) && ((tileAt(board_, px, py+y_sign)) & TILE_OCCLUSION))
                break;

			y += dy_abs;

			if (y >= dx_abs)
			{
				y -= dx_abs;
				py += y_sign;
			}
			px += x_sign;
		}
	}
    else
	{
		for (i = 0; i <= dy_abs; i++)
		{
            if ((dist = sqrt(pow(ax-px, 2) + pow(ay-py, 2))) > range)
                break;
            if (lightTile(board_, px, py, brightness, dist, range))
                break;
            if (((tileAt(board_, px+x_sign, py)) & TILE_OCCLUSION) && ((tileAt(board_, px, py+y_sign)) & TILE_OCCLUSION))
                break;

			x += dx_abs;

			if (x >= dy_abs)
			{
				x -= dy_abs;
				px += x_sign;
			}
			py += y_sign;
		}
	}
}

void lightSpot(struct Board* board_, const int x, const int y, const int brightness, const float range)
{
    int ix, iy, minX, maxX, minY, maxY;

    if ((minX = x-range) < 0)           minX = 0;
    if ((minY = y-range) < 0)           minY = 0;
    if ((maxX = x+range) > board_->w)   maxX = board_->w;
    if ((maxY = y+range) > board_->h)   maxY = board_->h;

    for (iy = minY; iy < maxY; iy++)
    {
        for (ix = minX; ix < maxX; ix++)
        {
            board_->tileMap[iy*board_->w + ix] &= (~TILE_LIT);
        }
    }

    //lightRay(board_, x, y, minX, minY, brightness, range);
    //lightRay(board_, x, y, maxX, minY, brightness, range);
    //lightRay(board_, x, y, minX, maxY, brightness, range);
    //lightRay(board_, x, y, maxX, maxY, brightness, range);

    for (ix = minX, iy = minY; ix < maxX; ix++)
        lightRay(board_, x, y, ix, iy, brightness, range);
    for (; iy < maxY; iy++)
        lightRay(board_, x, y, ix, iy, brightness, range);
    for (; ix > minX; ix--)
        lightRay(board_, x, y, ix, iy, brightness, range);
    for (; iy > minY; iy--)
        lightRay(board_, x, y, ix, iy, brightness, range);
}

void lightBoard(struct Board* board_)
{
    int i;

    memset(board_->lightMap, minLight, board_->w*board_->h);

    if (lightEnable)
    {
        for (i = 0; i < board_->numObjects; i++)
        {
            if (board_->objects[i].type == OBJECT_LIGHT)
                lightSpot(board_, board_->objects[i].x, board_->objects[i].y, board_->objects[i].brightness, board_->objects[i].range);
        }
    }
}

uint16_t InputChannelArray[NUM_INPUT_CHANNELS];

void getInput(uint8_t channel, uint8_t controlType)
{
    SDL_Event event;
    const uint8_t* keyState;

    uint8_t inputLeft   = 0;
    uint8_t inputRight  = 0;
    uint8_t inputUp     = 0;
    uint8_t inputDown   = 0;
    uint8_t inputRun    = 0;
    uint8_t inputStrafe = 0;
    uint8_t inputFire   = 0;

    static uint8_t mouseLeftDown  = 0;
    static uint8_t mouseRightDown = 0;

    InputChannelArray[channel] = 0;
    keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_ESCAPE])
    {
        quit = 1;
        return;
    }

    // temporary camera/control switching function
/*
    if (keyState[SDL_SCANCODE_1])
    {
        ControlArray[0].inputChannel = 0;
        ControlArray[1].inputChannel = -1;

        CameraArray[0].entityId = 0;
        playerId = 0;
    }
    else if (keyState[SDL_SCANCODE_2])
    {
        ControlArray[0].inputChannel = -1;
        ControlArray[1].inputChannel = 0;

        CameraArray[0].entityId = 1;
        playerId = 1;
    }
*/
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseLeftDown = 1;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseRightDown = 1;
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseLeftDown = 0;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseRightDown = 0;
        }
    }

    if (controlType & CONTROL_MOUSELOOK)
        SDL_GetMouseState(&mouseX, &mouseY);

    if (controlType & CONTROL_KEYBOARD)
    {
        inputLeft   |= keyState[SDL_SCANCODE_LEFT]   | keyState[SDL_SCANCODE_A];
        inputRight  |= keyState[SDL_SCANCODE_RIGHT]  | keyState[SDL_SCANCODE_D];
        inputUp     |= keyState[SDL_SCANCODE_UP]     | keyState[SDL_SCANCODE_W];
        inputDown   |= keyState[SDL_SCANCODE_DOWN]   | keyState[SDL_SCANCODE_S];
        inputRun    |= keyState[SDL_SCANCODE_RSHIFT] | keyState[SDL_SCANCODE_LSHIFT];
        inputStrafe |= keyState[SDL_SCANCODE_RALT]   | keyState[SDL_SCANCODE_LALT]  | mouseRightDown;
        inputFire   |= keyState[SDL_SCANCODE_RCTRL]  | keyState[SDL_SCANCODE_LCTRL] | mouseLeftDown;
    }

    if (inputUp)        InputChannelArray[channel] |= COMMAND_MOVE_UP;
    if (inputDown)      InputChannelArray[channel] |= COMMAND_MOVE_DOWN;
    if (inputRun)       InputChannelArray[channel] |= COMMAND_MOVE_RUN;

    if ((controlType & CONTROL_ROTATIONAL) && (controlType & CONTROL_MOUSELOOK) == 0 && inputStrafe == 0)
    {
        if (inputLeft)  InputChannelArray[channel] |= COMMAND_TURN_LEFT;
        if (inputRight) InputChannelArray[channel] |= COMMAND_TURN_RIGHT;
    }
    else
    {
        if (inputLeft)  InputChannelArray[channel] |= COMMAND_MOVE_LEFT;
        if (inputRight) InputChannelArray[channel] |= COMMAND_MOVE_RIGHT;
    }

    if (inputFire)      InputChannelArray[channel] |= COMMAND_FIRE;
}

enum AI_STATE
{
    AI_IDLE,
    AI_PATROL
};

void doAI(struct Board* board_) // refactor this whole function, the nesting is totally fucked
{
    int i;

    for (i = 0; i < entityCount; i++)
    {
        if (EntityArray[i] & TYPE_AI)
        {
            if (ControlArray[i].type & CONTROL_DIRECTIONAL)
            {
                if (AIArray[i].state == AI_IDLE)
                {
                    if ((tileAtPos(board_, PositionArray[i].x, PositionArray[i].y) & TILE_LIT) == 0)
                    {
                        AIArray[i].state = AI_PATROL;
                        AIArray[i].commands |= COMMAND_MOVE_LEFT;
                        // ^ should be a random direction or start heading toward the player
                        // refactor into "start moving"
                    }
                }
                else if (AIArray[i].state == AI_PATROL)
                {
                    VisibleArray[i].color[0] += 128;

                    if (CollidableArray[i].collision & (WEST | EAST))
                    {
                        AIArray[i].commands ^= (COMMAND_MOVE_LEFT | COMMAND_MOVE_RIGHT);
                    }
                }
            }
        }
    }
}

void controlEntity(const int i)
{
    const float accX    = DEFAULT_ACCELERATION;
    const float accY    = DEFAULT_ACCELERATION;
    const float angAcc  = DEFAULT_ANGULAR_ACCELERATION;
    uint16_t* commands  = &(ControlArray[i].commands);

    if (EntityArray[i] & TYPE_FORCE)
    {
        /*
        if (ControlArray[i].commands & COMMAND_MOVE_RUN)
            VelocityArray[i].maxVelSquared = pow(DEFAULT_RUN_SPEED, 2);
        else if (VelocityArray[i].maxVelSquared > pow(DEFAULT_WALK_SPEED, 2))
            VelocityArray[i].maxVelSquared -= 0.1;
        */ // poor implementation

        if (xor((*commands & COMMAND_MOVE_LEFT), (*commands & COMMAND_MOVE_RIGHT)))
            ForceArray[i].x = (*commands & COMMAND_MOVE_LEFT) ? -accX : accX;
        else
            ForceArray[i].x = 0;

        if (xor((*commands & COMMAND_MOVE_UP), (*commands & COMMAND_MOVE_DOWN)))
            ForceArray[i].y = (*commands & COMMAND_MOVE_UP) ? -accY : accY;
        else
            ForceArray[i].y = 0;
    }

    if (EntityArray[i] & TYPE_TORQUE)
    {
        if (xor(((*commands) & COMMAND_TURN_LEFT), ((*commands) & COMMAND_TURN_RIGHT)))
        {
            TorqueArray[i].angVel += ((*commands) & COMMAND_TURN_LEFT) ? angAcc : -angAcc;

            if (TorqueArray[i].angVel > TorqueArray[i].maxAngVel)
                TorqueArray[i].angVel = TorqueArray[i].maxAngVel;
            else if (TorqueArray[i].angVel < -(TorqueArray[i].maxAngVel))
                TorqueArray[i].angVel = -(TorqueArray[i].maxAngVel);
        }
        else if (TorqueArray[i].angVel >= angAcc)
            TorqueArray[i].angVel -= angAcc;
        else if (TorqueArray[i].angVel <= -angAcc)
            TorqueArray[i].angVel += angAcc;
        else
            TorqueArray[i].angVel = 0;
    }

    (*commands) &= (~MOVE_COMMANDS);
}

void doControl()
{
    int i;
    uint8_t channel;

    for (i = 0; i < entityCount; i++)
    {
        if (EntityArray[i] & TYPE_CONTROL)
        {
            channel = ControlArray[i].inputChannel;

            if (ControlArray[i].type & CONTROL_AI)
                ControlArray[i].commands = AIArray[i].commands;
            else if (ControlArray[i].type & CONTROL_KEYBOARD)
                ControlArray[i].commands = InputChannelArray[channel];

            controlEntity(i);
        }
    }
}

struct Vec2 shootRay(struct Board* board_, struct Vec2 origin, struct Vec2 direction)
{
    struct Vec2 ray = origin;

    while (1)   // This will CRASH if it goes into infinity
    {
        if (tileAtPos(board_, ray.x, ray.y) & TILE_OBSTACLE)
            return ray;

        else
            addVec2(ray, direction);
    }
}

void raycast(struct Board* board_, int camId)
{
    const int      halfScreenH       = screenHeight/2;
    const int      floorTex_         = floorTex;
    const int      ceilingTex_       = ceilingTex;
    const int      wallTex_          = wallTex;                           // same
    const int      lightEnable_      = lightEnable;                         // same
    const int      wallFog_          = wallFog;
    const int      floorFog_         = floorFog;
    const int      ceilingFog_       = ceilingFog;
    const int      backgroundTop_    = backgroundTop;
    const int      backgroundBottom_ = backgroundBottom;
    const int      backClipPlane_    = backClipPlane;
    const int      drawDistance_     = drawDistance;
    const int      underwater        = UNDERWATER;                           // dunno
    const int      hRatio            = (screenWidth*halfTile)/DEFAULT_V_FOV; // /planeVert;    // should be calculated here
    const int      debug2D           = 1;
    const float    planeHorz         = DEFAULT_H_FOV;                        // camera property too
    const float    planeVert         = DEFAULT_V_FOV;                        // same
    const float    xInc              = 2.0/screenWidth;
    const float    distInc           = 0.1;
    const float    liquidWaveHeight  = LIQUID_WAVE_HEIGHT;
    const float    liquidWaveWidth   = LIQUID_WAVE_WIDTH;
    const float    liquidWaveSpeed   = LIQUID_WAVE_SPEED;
    const struct   Vec2 CamPos       = PositionArray[camId];
    const struct   Vec2 CamDir       = {RotationArray[camId].x, RotationArray[camId].y};
    const struct   Vec2 CamPlane     = {-(CamDir.y)*planeHorz, (CamDir.x)*planeHorz};
    const SDL_Rect ScreenRect        = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    int i, y, height, halfHeight, offset, bottom, alpha;
    float x, z, zInc, dist;
    static float zFactor;
    uint16_t tileType;
    struct Vec2 RayPos;
    struct Vec2 RayDir;
    struct Vec3 RayPos2;
    SDL_Rect SrcRect, DstRect;

    // headbop
    if ((ForceArray[playerId].x || ForceArray[playerId].y) && zFactor < 1)
        zFactor += BOP_Z_INC;
    else if (zFactor > 0)
    {
        zFactor -= BOP_Z_INC;

        if (zFactor < BOP_Z_INC)
            zFactor = 0;
    }

    if (zFactor)
        z = zFactor * sin(degToRad((int)(tick * BOP_SPEED) % 360)) * BOP_HEIGHT;
    else
        z = 0;





    // Floors & ceiling
    SDL_SetRenderTarget       (Renderer3D, NULL);
    SDL_SetRenderDrawBlendMode(Renderer3D, SDL_BLENDMODE_NONE);
    setVec2(RayDir, CamDir);

    if (wallFog_)
    {
        SDL_SetRenderDrawColor(Renderer3D, colorArg3(board_->fogColor), 255);
        SDL_RenderClear       (Renderer3D);
    }
    else if (backClipPlane_)
    {
        SDL_SetRenderDrawColor(Renderer3D, colorArg4(RGBA_BLACK));
        SDL_RenderClear       (Renderer3D);
    }



#define CORRECTION -1

    // Background
    if (backgroundTop_ || backgroundBottom_)
    {
        float bgX;
        int bgAngle = radToDeg(RotationArray[camId].angle);
        if (bgAngle < 0) bgAngle + 360;
        bgAngle = bgAngle % 360;

        for (i = 0; i < 4; i++)
        {
            bgX = (((unsigned)(bgAngle + (i*90)) % 360) / 360.0) - 0.25;
            BackgroundSrcRect.x = i * BackgroundSrcRect.w;
            BackgroundDstRect.x = bgX * (screenWidth * 4);

            if (BackgroundDstRect.x >= screenWidth || BackgroundDstRect.x <= -screenWidth)
                continue;

            SDL_RenderCopy(Renderer3D, BackgroundTexture, &BackgroundSrcRect, &BackgroundDstRect);
        }
    }




    // Floor
    setVec2(RayPos2, CamPos);

    for (y = 1; y <= halfScreenH; y++)
    {
        dist        = 0;
        setVec2(RayPos2, CamPos);
        RayPos2.z   = halfTile+z;
        zInc        = 1.5*((float)y/screenHeight);

        while (dist < drawDistance_)
        {
            dist += 1               * 0.1;  // temp
            RayPos2.z -= zInc       * 0.1;
            //addVec2(RayPos2, RayDir);
            RayPos2.x += RayDir.x * 0.1;
            RayPos2.y += RayDir.y * 0.1;

            if (RayPos2.z < 0)
            {
                if (!backgroundBottom_)
                {
                    SDL_SetRenderDrawColor(Renderer3D, colorArg3(board_->floorColor), 255);
                    SDL_RenderDrawLine    (Renderer3D, 0, halfScreenH+(y-1)+CORRECTION, screenWidth-1, halfScreenH+(y-1)+CORRECTION);
                }

                if (floorTex_)
                {
                    x = -1;

                    for (i = 0; i < screenWidth; i++)
                    {
                        RayPos.x = RayPos2.x + (CamPlane.x * dist * x);// * x);
                        RayPos.y = RayPos2.y + (CamPlane.y * dist * x);// * x);

                        tileType = tileAtPos(board_, RayPos.x, RayPos.y);

                        if ((tileType & TILE_OCCLUSION) == 0)
                        {

                            int texX = (int)(RayPos.x * 4.0) % texSize;
                            int texY = (int)(RayPos.y * 4.0) % texSize;

                            SrcRect.x = texX;
                            SrcRect.y = texY + (texSize * (tileType >> TILE_FLAGS));
                            SrcRect.w = 1;
                            SrcRect.h = 1;

                            DstRect.x = i;
                            DstRect.y = halfScreenH+(y-1)+CORRECTION;
                            DstRect.w = 1;
                            DstRect.h = 1;

                            SDL_RenderCopy(Renderer3D, Texture3D, &SrcRect, &DstRect);
                            alpha = 255 - board_->lightMap[board_->w * (int)(RayPos.y/tileSize) + (int)(RayPos.x/tileSize)];
                            SDL_SetRenderDrawColor(Renderer3D, 0, 0, 0, alpha);
                            SDL_RenderDrawPoint(Renderer3D, DstRect.x, DstRect.y);



                            //SDL_SetRenderDrawColor      (Renderer2D, colorArg4(RGBA_GREEN));
                            //SDL_RenderDrawPoint         (Renderer2D, camera2D_X+RayPos.x, camera2D_Y+RayPos.y);
                        }

                        x += xInc;
                    }
                }

                if (floorFog_)
                {
                    if ((alpha = 255 * (dist/board_->fogDistance)) > 255)
                        alpha = 255;

                    SDL_SetRenderDrawBlendMode(Renderer3D, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(Renderer3D, colorArg3(board_->fogColor), alpha);
                    SDL_RenderDrawLine    (Renderer3D, 0, halfScreenH+(y-1)+CORRECTION, screenWidth-1, halfScreenH+(y-1)+CORRECTION);
                }

                break;
            }
        }
    }
    SDL_SetRenderDrawBlendMode(Renderer3D, SDL_BLENDMODE_NONE);

    // Ceiling
    for (y = 1; y < halfScreenH; y++)
    {
        dist        = 0;
        setVec2(RayPos2, CamPos);
        RayPos2.z   = halfTile+z;
        zInc        = (float)y/halfScreenH;

        while (dist < drawDistance_)
        {
            dist += 1;// * 0.1;
            RayPos2.z += zInc;// * 0.1;
            addVec2(RayPos2, RayDir);

            if (RayPos2.z > tileSize)
            {
                if ((alpha = 255 * (dist/board_->fogDistance)) > 255)
                    alpha = 255;

                if (!backgroundTop_)
                {
                    SDL_SetRenderDrawColor(Renderer3D, colorArg3(board_->ceilingColor), 255);
                    SDL_RenderDrawLine    (Renderer3D, 0, (halfScreenH-1)-(y-1)-CORRECTION, screenWidth-1, (halfScreenH-1)-(y-1)-CORRECTION);
                }
/*
                if (ceilingtex_)
                {
                    x = -1;

                    for (i = 0; i < screenWidth; i++)
                    {
                        RayPos.x = RayPos2.x + (CamPlane.x * dist * x);// * x);
                        RayPos.y = RayPos2.y + (CamPlane.y * dist * x);// * x);

                        tileType = tileAtPos(board_, RayPos.x, RayPos.y);
                        int texX = (((int)(RayPos2.x/tileSize) % tileSize) / tileSize) * texSize;
                        int texY = (((int)(RayPos2.y/tileSize) % tileSize) / tileSize) * texSize;

                        SrcRect.x = texX;
                        SrcRect.y = texY + texSize * (tileType >> TILE_FLAGS);
                        SrcRect.w = 1;
                        SrcRect.h = 1;

                        DstRect.x = i;
                        DstRect.y = (halfScreenH-1)-(y-1)-CORRECTION;
                        DstRect.w = 1;
                        DstRect.h = 1;

                        SDL_RenderCopy(Renderer3D, Texture3D, &SrcRect, &DstRect);

                        SDL_SetRenderDrawColor      (Renderer2D, colorArg4(RGBA_GREEN));
                        SDL_RenderDrawPoint         (Renderer2D, camera2D_X+RayPos.x, camera2D_Y+RayPos.y);

                        x += xInc;
                    }
                }
                */

                if (ceilingFog_)
                {
                    SDL_SetRenderDrawBlendMode(Renderer3D, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(Renderer3D, colorArg3(board_->fogColor), alpha);
                    SDL_RenderDrawLine    (Renderer3D, 0, (halfScreenH-1)-(y-1)-CORRECTION, screenWidth-1, (halfScreenH-1)-(y-1)-CORRECTION);
                }

                break;
            }
        }
    }
    SDL_SetRenderDrawBlendMode(Renderer3D, SDL_BLENDMODE_NONE);

    // Walls
    x       = -1;
    SrcRect = (SDL_Rect){.x = 0, .y = 0, .w = 1, .h = texSize};
    DstRect = (SDL_Rect){.x = 0, .y = 0, .w = 1, .h = 0};

    if (wallTex_)
    {
        SDL_SetRenderTarget   (Renderer3D, OffScreen3D);
        SDL_SetRenderDrawColor(Renderer3D, 0, 0, 0, 0);
        SDL_RenderClear       (Renderer3D);
    }

    for (i = 0; i < screenWidth; i++)
    {
        RayPos  = CamPos;
        RayDir  = (struct Vec2){(CamDir.x + x*CamPlane.x)*distInc, (CamDir.y + x*CamPlane.y)*distInc};
        dist    = 0;

        while (dist < drawDistance_)
        {
            addVec2(RayPos, RayDir);
            dist += distInc;

            if ((tileType = tileAtPos(board_, RayPos.x, RayPos.y)) & TILE_OCCLUSION)
            {
                if (debug2D)
                {
                    SDL_SetRenderDrawColor      (Renderer2D, colorArg4(RGBA_GREEN));
                    SDL_RenderDrawPoint         (Renderer2D, camera2D_X+RayPos.x, camera2D_Y+RayPos.y);
                }

                height     = (int)(hRatio/dist) & ~1;
                halfHeight = height/2;
                offset     = z * (((hRatio)/dist)/tileSize);

                if (underwater)
                    offset += sin(degToRad((int)(i + tick*WAVE_SPEED + RotationArray[camId].angle) * WAVE_WIDTH % 360)) * WAVE_HEIGHT;

                DstRect.x  = i;
                DstRect.y  = halfScreenH - halfHeight + offset;
                DstRect.h  = height;
                bottom     = DstRect.y + DstRect.h;

                if (wallTex_)
                {
                    SrcRect.x = (int)((((float)(RayPos.x + RayPos.y)) / tileSize) * texSize) % texSize;
                    SrcRect.y = texSize * (tileType >> TILE_FLAGS);

                    if (tileType & TILE_LIQUID)
                    {
                        SrcRect.y += sin(degToRad((int)(tick * liquidWaveSpeed + SrcRect.x * liquidWaveWidth) % 360)) * liquidWaveHeight + liquidWaveHeight;
                        SrcRect.h = texSize - (liquidWaveHeight * 2);
                    }
                    else
                        SrcRect.h = texSize;

                    SDL_RenderCopy(Renderer3D, Texture3D, &SrcRect, &DstRect);
                }
                else
                {
                    SDL_SetRenderDrawColor(Renderer3D, colorArg3(board_->wallColor), 255);
                    SDL_RenderDrawLine    (Renderer3D, i, DstRect.y, i, bottom-1);
                }

                if (lightEnable_)
                {
                    subtractVec2(RayPos, RayDir);
                    alpha = board_->lightMap[(int)RayPos.y/tileSize * board_->w + (int)RayPos.x/tileSize];

                    SDL_SetRenderDrawBlendMode(Renderer3D, SDL_BLENDMODE_MOD);
                    SDL_SetRenderDrawColor    (Renderer3D, alpha, alpha, alpha, 255);
                    SDL_RenderDrawLine        (Renderer3D, i, DstRect.y, i, bottom-1);
                }

                if (wallFog_)
                {
                    if ((alpha = 255 * (dist/board_->fogDistance)) > 255)
                        alpha = 255;

                    SDL_SetRenderDrawBlendMode(Renderer3D, FogBlendMode);
                    SDL_SetRenderDrawColor    (Renderer3D, colorArg3(board_->fogColor), alpha);
                    SDL_RenderDrawLine        (Renderer3D, i, DstRect.y, i, bottom-1);
                }
                break;
            }
        }
        x += xInc;
        SDL_SetRenderDrawBlendMode(Renderer3D, SDL_BLENDMODE_NONE);
    }

    if (wallTex_)
    {
        SDL_SetRenderTarget(Renderer3D, NULL);
        SDL_RenderCopy     (Renderer3D, OffScreen3D, &ScreenRect, &ScreenRect);
    }

    SDL_RenderPresent(Renderer3D);
}

void doFire(SDL_Renderer* renderer_, struct Board* board_, int i)
{
    struct Vec2 hit, direction;
    static int cooldown = 0;

    if (cooldown > 0)
        cooldown--;

    else if (ControlArray[i].commands & COMMAND_FIRE)
    {
        setVec2(direction, RotationArray[i]);
        addVec2(direction, randomVec2(0, INACCURACY));
        hit = shootRay(board_, PositionArray[i], direction);
        subtractVec2(hit, direction);

        SDL_SetRenderDrawColor(renderer_, colorArg4(FIRE_COLOR1));
        SDL_RenderDrawLine
        (
           renderer_,
           PositionArray[i].x+camera2D_X,
           PositionArray[i].y+camera2D_Y,
           hit.x+camera2D_X,
           hit.y+camera2D_Y
        );
        spawnExplosion(hit, ZeroVec2, EXPLOSION_MAGNITUDE);
        cooldown = FIRE_COOLDOWN_TIME;
    }
}

void renderBoard(struct Board* board_)
{
    int x, y, minY, maxY, minX, maxX;
    uint16_t tileType;

    SDL_Rect SrcRect = (SDL_Rect){.x = 0, .y = 0, .w = texSize, .h = texSize};
    SDL_Rect DstRect = (SDL_Rect){.x = 0, .y = 0, .w = tileSize, .h = tileSize};

    minX = -camera2D_X/tileSize;
    maxX = (-camera2D_X + screenWidth)/tileSize+1;
    minY = -camera2D_Y/tileSize;
    maxY = (-camera2D_Y + screenHeight)/tileSize+1;

    if (minX < 0)
        minX = 0;
    if (minY < 0)
        minY = 0;
    if (maxX > board_->w)
        maxX = board_->w;
    if (maxY > board_->h)
        maxY = board_->h;

    for (y = minY; y < maxY; y++)
    {
        DstRect.y = y*tileSize + camera2D_Y;

        for (x = minX; x < maxX; x++)
        {
            tileType = tileAt(board_, x, y);
            DstRect.x = x*tileSize + camera2D_X;
            SrcRect.x = 0;
            SrcRect.y = texSize * (tileType >> TILE_FLAGS);
            SDL_RenderCopy(Renderer2D, Texture2D, &SrcRect, &DstRect);

            if (lightEnable)
            {
                SDL_SetRenderDrawBlendMode(Renderer2D, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(Renderer2D, 0, 0, 0, 255 - board_->lightMap[y*board_->w + x]);
                SDL_RenderFillRect(Renderer2D, &DstRect);
                SDL_SetRenderDrawBlendMode(Renderer2D, SDL_BLENDMODE_NONE);
            }
        }
    }
}

void centerCamera(int cameraId)
{
    camera2D_X = screenWidth/2 -  PositionArray[cameraId].x;
    camera2D_Y = screenHeight/2 - PositionArray[cameraId].y;
}

void spawnPlayer(int x, int y, float angle_, uint8_t color_[])
{
    EntityArray[entityCount] =
      TYPE_POSITION
    | TYPE_TRANSFORM
    | TYPE_VELOCITY
    | TYPE_FORCE
    | TYPE_ROTATION
    | TYPE_TORQUE
    | TYPE_COLLIDABLE
    | TYPE_CONTROL
    | TYPE_VISIBLE;

    PositionArray             [entityCount]              = (struct Vec2)       {.x = halfTile + tileSize*x, .y = halfTile + tileSize*x};
    TransformArray            [entityCount]              = ZeroVec2;
    VelocityArray             [entityCount]              = VelocityDefault;
    ForceArray                [entityCount]              = ForceDefault;
    RotationArray             [entityCount].angle        = degToRad(angle_);
    setVec2Angle(RotationArray[entityCount], angle_);
    TorqueArray               [entityCount]              = TorqueDefault;
    CollidableArray           [entityCount]              = (struct Collidable) {.w = tileSize, .h = tileSize};
    VisibleArray              [entityCount]              = (struct Visible)    {.type = VISIBLE_HITBOX, .animation = 0, .frame = 0};
    ControlArray              [entityCount].type         = CONTROL_ROTATIONAL | CONTROL_KEYBOARD;// | CONTROL_MOUSELOOK;
    ControlArray              [entityCount].inputChannel = 0;
    setColor(VisibleArray     [entityCount].color, color_);
    playerId = entityCount;

    entityCount++;
    printf("entityCount: %d\n", entityCount);
}

void initRenderer(struct Board* board_)
{
    uint8_t transColor[3] = {152, 0, 136};
    SDL_Surface* TempSurface = IMG_Load(board_->textureFile);
    SDL_SetColorKey(TempSurface, SDL_TRUE, SDL_MapRGB(TempSurface->format, transColor[0], transColor[1], transColor[2]));

    Window2D      = SDL_CreateWindow("lol", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * RES_SCALE, SCREEN_HEIGHT * RES_SCALE, SDL_WINDOW_SHOWN | (BORDERLESS ? SDL_WINDOW_BORDERLESS : 0));
    Window3D      = SDL_CreateWindow("lol3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * RES_SCALE, SCREEN_HEIGHT * RES_SCALE, SDL_WINDOW_SHOWN | (BORDERLESS ? SDL_WINDOW_BORDERLESS : 0) | (FULL_SCREEN ? SDL_WINDOW_FULLSCREEN : 0));
    Renderer2D    = SDL_CreateRenderer(Window2D, -1, SDL_RENDERER_ACCELERATED);
    Renderer3D    = SDL_CreateRenderer(Window3D, -1, SDL_RENDERER_ACCELERATED);
    Texture2D     = SDL_CreateTextureFromSurface(Renderer2D, TempSurface);
    Texture3D     = SDL_CreateTextureFromSurface(Renderer3D, TempSurface);
    OffScreen3D   = SDL_CreateTexture(Renderer3D, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetTextureBlendMode(OffScreen3D, SDL_BLENDMODE_BLEND);
    FogBlendMode  = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDOPERATION_ADD);

    TempSurface         = IMG_Load(board_->bgFile);
    BackgroundTexture   = SDL_CreateTextureFromSurface(Renderer3D, TempSurface);

    BackgroundSrcRect.x = 0;
    BackgroundSrcRect.w = TempSurface->w / 4;
    BackgroundDstRect.x = 0;
    BackgroundDstRect.w = screenWidth;

    if (backgroundTop && backgroundBottom)
    {
        BackgroundSrcRect.y = 0;
        BackgroundSrcRect.h = TempSurface->h;
        BackgroundDstRect.y = 0;
        BackgroundDstRect.h = screenHeight;
    }

    else if (backgroundTop && !backgroundBottom)
    {
        BackgroundSrcRect.y = 0;
        BackgroundSrcRect.h = TempSurface->h/2;
        BackgroundDstRect.y = 0;
        BackgroundDstRect.h = screenHeight/2;
    }
    else if (!backgroundTop && backgroundBottom)
    {
        BackgroundSrcRect.y = TempSurface->h/2;
        BackgroundSrcRect.h = TempSurface->h/2;
        BackgroundDstRect.y = screenHeight/2;
        BackgroundDstRect.h = screenHeight/2;
    }

    SDL_RenderSetScale(Renderer2D, RES_SCALE, RES_SCALE);
    SDL_RenderSetScale(Renderer3D, RES_SCALE, RES_SCALE);
    SDL_FreeSurface(TempSurface);
}

void spawnObjects(struct Board* board_)
{
    int i;

    for (i = 0; i < board_->numObjects; i++)
    {
        switch (board_->objects[i].type)
        {
        case OBJECT_PLAYER:
            spawnPlayer(board_->objects[i].x, board_->objects[i].y, board_->objects[i].angle, RGBA_GREEN);
            break;
        default:
            break;
        }
    }
}

void getSettings(struct Board* board_)
{
    texSize          = board_->texSize;
    tileSize         = board_->tileSize;
    halfTile         = tileSize/2;
    quarterTile      = tileSize/4;
    wallFog          = board_->wallFog;
    floorFog         = board_->floorFog;
    ceilingFog       = board_->ceilingFog;
    wallTex          = board_->wallTex;
    floorTex         = board_->floorTex;
    ceilingTex       = board_->ceilingTex;
    lightEnable      = board_->lightEnable;
    minLight         = board_->minLight;
    maxLight         = board_->maxLight;
    drawDistance     = board_->drawDistance;
    backClipPlane    = board_->backClipPlane;
    backgroundTop    = board_->backgroundTop;
    backgroundBottom = board_->backgroundBottom;

    if (maxLight > 255) maxLight = 255;
}

/*
void replaceColor(SDL_Surface* surface_, SDL_Color DstColor, SDL_Color SrcColor)
{
    int i, j;
    int numColors = surface_->format->palette->ncolors;

    for (i = 0; i < numColors; i++)
    {
        if (surface_->format->palette->ncolors[i].r != DstColor.r) break;
        if (surface_->format->palette->ncolors[i].g != DstColor.g) break;
        if (surface_->format->palette->ncolors[i].b != DstColor.b) break;

        surface_->format->palette->ncolors[i] = SrcColor;
    }
}
*/

int initGame()
{
    struct Board* MainBoard;

    // Initialization
    initArrays();
    MainBoard = loadMap("map2.txt");
    getSettings(MainBoard);
    lightBoard(MainBoard);
    spawnObjects(MainBoard);
    cameraId = playerId;

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    SDL_ShowCursor(SDL_DISABLE);
    initRenderer(MainBoard);

    quit = 0;

    while (!quit)
    {
        // Input and logic
        getInput(0, ControlArray[0].type);

        if (tick % 31 == 0) // solve issue with this not working if it runs too frequently
            doAI(MainBoard);

        doControl();

        // Physics
        doRotationAndTorque();
        doVelocity();
        doTransform();
        doCollidable(MainBoard);
        doPosition();

        // Render
        SDL_SetRenderDrawColor  (Renderer2D, 0, 0, 0, 255);
        SDL_RenderClear         (Renderer2D);
        //SDL_SetRenderDrawColor  (Renderer3D, 0x00, 0x00, 0x00, 0xFF);
        //SDL_RenderClear         (Renderer3D);

        centerCamera            (cameraId);
        renderBoard             (MainBoard);
        raycast                 (MainBoard, cameraId);
        renderVisible           (Renderer2D);
        doFire                  (Renderer2D, MainBoard, playerId);      // should be separated to logic and render
        renderParticles         (Renderer2D, MainBoard);
        renderCrosshair         (Renderer2D, playerId);
        SDL_RenderPresent       (Renderer2D);
        //SDL_RenderPresent       (Renderer3D);

        // End cycle
        SDL_Delay(1000.0/60.0);
        tick++;
    }

    // Quit
    SDL_DestroyTexture  (Texture2D);
    SDL_DestroyRenderer (Renderer2D);
    SDL_DestroyWindow   (Window2D);

    SDL_DestroyTexture  (Texture3D);
    SDL_DestroyRenderer (Renderer3D);
    SDL_DestroyWindow   (Window3D);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
