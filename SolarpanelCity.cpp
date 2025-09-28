#include <windows.h> 
#include <GL/glut.h> 
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define NUM_CLOUDS 3
//Sun initialization
float sunX = 0.0f;
float sunY = 370.0f;
//moon initialization
float moonX = 400.0f;
float moonY = 370.0f;

//Clouds initialization
float cloudX[NUM_CLOUDS];
float cloudY[NUM_CLOUDS];
float cloudSpeed[NUM_CLOUDS];

// Vehicle positions
float carBottomX = -100.0f;
float carTopX = 500.0f;
// Airplane
float planeX = -200.0f;
float planeY = 320.0f;
bool showPlane = false;  

// Day/Night
bool isNight = false;
//solar
bool showSolarPanel = false;
//black out
bool isBlackout = false;

// Rain
bool isRaining = false;
const int NUM_RAIN = 400;
float rainX[NUM_RAIN];
float rainY[NUM_RAIN];
//Lightning
bool lightning = false;
int lightningTimer = 0;

// Bird positions
float birdX[5] = {-10, -20, -30, -40, -50};
float birdY[5] = {345, 350, 340, 355, 343};
float birdSpeed = 0.4f;

// Train variables
float trainX = -100.0f;
float trainY = 165.0f;
float trainSpeed = 1.0f;


void drawCircle(GLfloat x, GLfloat y, GLfloat radius, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i <= segments; i++) {
        GLfloat angle = i * 2.0f * M_PI / segments;
        GLfloat dx = radius * cosf(angle);
        GLfloat dy = radius * sinf(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

// ====Draw train "Galpa"====
void drawTrain(float xBase, float yBase) {
    // ===== Wagon 1====
    glColor3ub(150, 200, 45); // green body
    glBegin(GL_QUADS);
        glVertex2f(xBase + 0,  yBase + 0);
        glVertex2f(xBase + 60, yBase + 0);
        glVertex2f(xBase + 60, yBase + 25);
        glVertex2f(xBase + 0,  yBase + 25);
    glEnd();

    // Windows
    glColor3ub(180, 220, 250); // light blue
    glBegin(GL_QUADS);
        glVertex2f(xBase + 10, yBase + 8);
        glVertex2f(xBase + 25, yBase + 8);
        glVertex2f(xBase + 25, yBase + 20);
        glVertex2f(xBase + 10, yBase + 20);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(xBase + 35, yBase + 8);
        glVertex2f(xBase + 50, yBase + 8);
        glVertex2f(xBase + 50, yBase + 20);
        glVertex2f(xBase + 35, yBase + 20);
    glEnd();

    // Wheels
    glColor3ub(0, 0, 0);
    drawCircle(xBase + 10, yBase + 0, 5, 20);
    drawCircle(xBase + 50, yBase + 0, 5, 20);

    // ===== Wagon 2 =====
    glColor3ub(0, 0, 255); // blue body
    glBegin(GL_QUADS);
        glVertex2f(xBase + 65, yBase + 0);
        glVertex2f(xBase + 120, yBase + 0);
        glVertex2f(xBase + 120, yBase + 25);
        glVertex2f(xBase + 65, yBase + 25);
    glEnd();

    // Windows
    glColor3ub(180, 220, 250);
    glBegin(GL_QUADS);
        glVertex2f(xBase + 75, yBase + 8);
        glVertex2f(xBase + 90, yBase + 8);
        glVertex2f(xBase + 90, yBase + 20);
        glVertex2f(xBase + 75, yBase + 20);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(xBase + 95, yBase + 8);
        glVertex2f(xBase + 110, yBase + 8);
        glVertex2f(xBase + 110, yBase + 20);
        glVertex2f(xBase + 95, yBase + 20);
    glEnd();

    // Wheels
    glColor3ub(0, 0, 0);
    drawCircle(xBase + 70,  yBase + 0, 5, 20);
    drawCircle(xBase + 110, yBase + 0, 5, 20);

    // ===== Locomotive =====
    glColor3ub(255, 0, 0); // red body
    glBegin(GL_QUADS);
        glVertex2f(xBase + 125, yBase + 0);
        glVertex2f(xBase + 180, yBase + 0);
        glVertex2f(xBase + 180, yBase + 30);
        glVertex2f(xBase + 125, yBase + 30);
    glEnd();

    // Windows 
    glColor3ub(180, 220, 250);
    glBegin(GL_QUADS);
        glVertex2f(xBase + 135, yBase + 10);
        glVertex2f(xBase + 150, yBase + 10);
        glVertex2f(xBase + 150, yBase + 22);
        glVertex2f(xBase + 135, yBase + 22);
    glEnd();

    // Cabin
    glColor3ub(200, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(xBase + 160, yBase + 30);
        glVertex2f(xBase + 180, yBase + 30);
        glVertex2f(xBase + 180, yBase + 45);
        glVertex2f(xBase + 160, yBase + 45);
    glEnd();

    // Cabin window
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
        glVertex2f(xBase + 165, yBase + 33);
        glVertex2f(xBase + 175, yBase + 33);
        glVertex2f(xBase + 175, yBase + 42);
        glVertex2f(xBase + 165, yBase + 42);
    glEnd();

    // Wheels
    glColor3ub(0, 0, 0);
    drawCircle(xBase + 130, yBase + 0, 5, 20);
    drawCircle(xBase + 150, yBase + 0, 5, 20);
    drawCircle(xBase + 170, yBase + 0, 5, 20);

        // ===== Headlight =====
    glColor3ub(255, 255, 100); // bright yellow
    drawCircle(xBase + 180, yBase + 20, 4, 30); // circle light at front side

        if (isNight) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 0.6f, 0.25f);
    glBegin(GL_TRIANGLES);
        glVertex2f(xBase + 182, yBase + 20);      // start at lamp
        glVertex2f(xBase + 220, yBase + 40);      // upper spread
        glVertex2f(xBase + 220, yBase + 0);       // lower spread
    glEnd();

        glDisable(GL_BLEND);
    }
}

//plane ==Galpa===
void drawAirplane(GLfloat x, GLfloat y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.5f, 0.5f, 1.0f);

    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(60, 0);
        glVertex2f(60, 10);
        glVertex2f(0, 10);
    glEnd();

    // Nose
    glColor3ub(200, 0, 0);
    float radius = 5.0f;
    float cx = 60.0f, cy = 5.0f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = -90; i <= 90; i++) {
            float rad = i * M_PI / 180.0f;
            glVertex2f(cx + cos(rad) * radius, cy + sin(rad) * radius);
        }
    glEnd();

    // Tail
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 5.0f);
        for (int i = 90; i <= 270; i++) {
            float rad = i * M_PI / 180.0f;
            glVertex2f(0 + cos(rad) * radius, 5.0f + sin(rad) * radius);
        }
    glEnd();

    // Cockpit window
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
        glVertex2f(55, 10);
        glVertex2f(60, 10);
        glVertex2f(60, 6);
        glVertex2f(55, 6);
    glEnd();

    // Passenger windows
    glColor3ub(0, 150, 255);
    for (int i = 8; i < 50; i += 10) {
        float wx = i, wy = 7;
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j++) {
            float rad = j * M_PI / 180.0f;
            glVertex2f(wx + cos(rad) * 1.5f, wy + sin(rad) * 1.5f);
        }
        glEnd();
    }

    // Wings
    glColor3ub(200, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(20, 10);
        glVertex2f(40, 10);
        glVertex2f(30, 25);
        glVertex2f(10, 25);

        glVertex2f(20, 0);
        glVertex2f(40, 0);
        glVertex2f(30, -15);
        glVertex2f(10, -15);
    glEnd();

    // Tail fin
    glBegin(GL_TRIANGLES);
        glVertex2f(0, 10);
        glVertex2f(-10, 25);
        glVertex2f(8, 10);
    glEnd();

    glPopMatrix();
}



// Draw a single bird as a "V" "Ramia"===
void drawBird(float x, float y) {
    glColor3ub(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x - 5, y + 5);
        glVertex2f(x, y);
        glVertex2f(x + 5, y + 5);
    glEnd();
}

void updateBirds() {
    // move all birds together
    for (int i = 0; i < 5; i++) {
        birdX[i] += birdSpeed;
    }
    float maxX = birdX[4];
    if (maxX > 400) {
        for (int i = 0; i < 5; i++) {
            birdX[i] -= (maxX + 200);
        }
    }
}

// Initialize clouds==Ramia===
void initClouds() {
    for (int i = 0; i < NUM_CLOUDS; i++) {
        cloudX[i] = rand() % 400;
        cloudY[i] = 350 + rand() % 100;
        cloudSpeed[i] = 0.5f;
    }
}
//Cloud ==Ramia==

void drawCloud(GLfloat x, GLfloat y) {
    if (isRaining) {
        glColor3ub(169, 169, 169); // ash gray for rain clouds
    } else {
        glColor3f(1.0f, 1.0f, 1.0f); // normal white clouds
    }
    drawCircle(x, y, 15, 40);
    drawCircle(x + 15, y + 5, 15, 40);
    drawCircle(x + 30, y, 15, 40);
    drawCircle(x + 15, y - 5, 15, 40);
}

void updateClouds() {
    for (int i = 0; i < NUM_CLOUDS; i++) {
        cloudX[i] -= cloudSpeed[i];
        if (cloudX[i] < -40) {
            cloudX[i] = 400 + rand() % 100;
            cloudY[i] = 350 + rand() % 100;
        }
    }
}


//Rain "Ramia"===
void initRain() {
// initialize rain positions
for (int i = 0; i < NUM_RAIN; i++) {
    rainX[i] = rand() % 400;
    rainY[i] = 200 + rand() % 200; // from mid sky down
}
}
//Rain
void drawRain() {
    if (!isRaining) return;

    glColor3ub(173, 216, 230); // light blue rain
    glLineWidth(1.5);
    glBegin(GL_LINES);
    for (int i = 0; i < NUM_RAIN; i++) {
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i], rainY[i] - 8); // length of raindrop
    }
    glEnd();
}
//Ramia==Tree
void drawTree(float x, float y) {
    // ===== Trunk =====
    glColor3ub(101, 67, 33); // brown
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 5, y);
        glVertex2f(x + 5, y + 25);
        glVertex2f(x, y + 25);
    glEnd();

    // ===== Leaves / Canopy =====
    glColor3ub(34, 139, 34); // dark green

    // Central circle
    drawCircle(x + 2.5, y + 35, 10, 30);

    // Surrounding circles for fullness
    drawCircle(x - 5, y + 32, 7, 30);
    drawCircle(x + 10, y + 32, 7, 30);
    drawCircle(x - 3, y + 40, 8, 30);
    drawCircle(x + 7, y + 40, 8, 30);
    drawCircle(x + 2.5, y + 45, 9, 30);
    drawCircle(x - 4, y + 45, 6, 30);
    drawCircle(x + 9, y + 45, 6, 30);
}

void initGL() {
    glClearColor(0.0f, .8f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 400, 0, 400);
    initClouds();
    initRain();
    glMatrixMode(GL_MODELVIEW);
}
//Sky Nadim
void sky() {
    if (isNight) {
        // Night sky
        glBegin(GL_QUADS);
            glColor3ub(10, 10, 40);   // dark blue night
            glVertex2f(0.0f, 200.0f);
            glVertex2f(400.0f, 200.0f);
            glVertex2f(400.0f, 400.0f);
            glVertex2f(0.0f, 400.0f);
        glEnd();
    } else {
        // Day sky
        glBegin(GL_QUADS);
            if (isRaining) {
                glColor3ub(180, 180, 200);  // light ash gray sky
            } else {
                glColor3ub(130, 202, 255);  // normal blue sky
            }
            glVertex2f(0.0f, 200.0f);
            glVertex2f(400.0f, 200.0f);
            glVertex2f(400.0f, 400.0f);
            glVertex2f(0.0f, 400.0f);
        glEnd();
    }
}


// Draw two-lane road===Nadim==
void road() {
    glBegin(GL_QUADS);
        glColor3ub(50, 50, 50);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(400.0f, 0.0f);
        glVertex2f(400.0f, 100.0f);
        glVertex2f(0.0f, 100.0f);
    glEnd();

    // Lane dividers
    glLineWidth(4);
    glColor3ub(255, 255, 0);
    glBegin(GL_LINES);
        for (int i = 0; i < 400; i += 40) {
            glVertex2f(i, 50.0f);
            glVertex2f(i + 20, 50.0f);
        }
    glEnd();
}


// Draw Car===Nadim===
void drawCar(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, bool faceLeft) {
    glPushMatrix();

    // ===== Night Light Triangle along road =====

    if (isNight) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.6f, 0.3f); // semi-transparent yellow

        float beamHeight = 15.0f;
        float beamWidth = 8.0f;

        if (!faceLeft) {
            // Bottom lane, car moving right
            glBegin(GL_TRIANGLES);
                glVertex2f(x + 58, y + 5);
                glVertex2f(x + 58 - 4, y + 5 - beamHeight);
                glVertex2f(x + 58 + beamWidth, y + 5 - beamHeight);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f(x + 58, y + 15);
                glVertex2f(x + 58 - 4, y + 15 - beamHeight);
                glVertex2f(x + 58 + beamWidth, y + 15 - beamHeight);
            glEnd();
        } else {
            // Top lane, car moving left
            glBegin(GL_TRIANGLES);
                glVertex2f(x + 2, y + 5);
                glVertex2f(x + 2 - beamWidth, y + 5 - beamHeight);
                glVertex2f(x + 2 + 4, y + 5 - beamHeight);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f(x + 2, y + 15);
                glVertex2f(x + 2 - beamWidth, y + 15 - beamHeight);
                glVertex2f(x + 2 + 4, y + 15 - beamHeight);
            glEnd();
        }

        glDisable(GL_BLEND);
    }

    if (faceLeft) {
        glTranslatef(x + 60, y, 0);
        glScalef(-1, 1, 1);
        x = 0; y = 0;
    }

    // ===== Car Body =====
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 60, y);
        glVertex2f(x + 60, y + 20);
        glVertex2f(x, y + 20);
    glEnd();

    // Roof
    glBegin(GL_QUADS);
        glVertex2f(x + 10, y + 20);
        glVertex2f(x + 50, y + 20);
        glVertex2f(x + 40, y + 35);
        glVertex2f(x + 20, y + 35);
    glEnd();

    // Windows
    glColor3ub(173, 216, 230);
    glBegin(GL_QUADS);
        glVertex2f(x + 22, y + 23);
        glVertex2f(x + 38, y + 23);
        glVertex2f(x + 33, y + 32);
        glVertex2f(x + 27, y + 32);
    glEnd();

    // Wheels
    glColor3ub(0, 0, 0);
    drawCircle(x + 15, y, 5, 20);
    drawCircle(x + 45, y, 5, 20);

    // Headlights
    glColor3ub(255, 255, 150);
    drawCircle(x + 58, y + 5, 3, 20);
    drawCircle(x + 58, y + 15, 3, 20);

    glPopMatrix();
}

//Nadim==electric poles
void epoll(float xBase, float yBase) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(xBase, yBase, 0.0f); // move pole to desired position

    // ===== Pole vertical =====
    glColor3ub(91, 133, 87); // Greenish pole
    glLineWidth(10);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, 60.0f);  // height
    glEnd();

    // ===== Angled top bar =====
    glColor3ub(55, 59, 58); // Dark gray
    glLineWidth(6);
    glBegin(GL_LINES);
        glVertex2f(-8.0f, 60.0f);
        glVertex2f(8.0f, 65.0f);
    glEnd();

    glPopMatrix();
}
//Nadim Electricpole wires
void eware(float yBase) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLineWidth(3);
    glColor3ub(0, 0, 0); // black wires

    glBegin(GL_LINES);
        glVertex2f(0.0f, yBase + 58.0f);
        glVertex2f(400.0f, yBase + 58.0f);

        glVertex2f(0.0f, yBase + 62.0f);
        glVertex2f(400.0f, yBase + 62.0f);

        glVertex2f(0.0f, yBase + 65.0f);
        glVertex2f(400.0f, yBase + 65.0f);
    glEnd();

    glPopMatrix();
}

//Street light Nadim
void drawRoadLight(float xBase, float yBase) {
    glPushMatrix();
    glTranslatef(xBase, yBase, 0.0f);

    // ===== Pole =====
    glColor3ub(80, 80, 80);
    glLineWidth(6);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, 50.0f);
    glEnd();

    // ===== Top horizontal arm =====
    glLineWidth(4);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 50.0f);
        glVertex2f(10.0f, 50.0f);
    glEnd();

    // ===== Lamp =====
if (isNight && !(isBlackout)) {
    // Only draw light if night AND either not blackout or solar active
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 1; i <= 3; i++) {
        float alpha = 0.15f / i;
        float width = 5.0f + i * 5;
        float height = 20.0f + i * 5;

        glColor4f(1.0f, 1.0f, 0.5f, alpha);
        glBegin(GL_TRIANGLES);
            glVertex2f(10.0f, 50.0f);
            glVertex2f(10.0f - width, 50.0f - height);
            glVertex2f(10.0f + width, 50.0f - height);
        glEnd();
    }
    glDisable(GL_BLEND);
}else {
        // Daytime, lamp off
        glColor3ub(150, 150, 150);
    }

    drawCircle(10.0f, 50.0f, 5, 30);

    //black out
        if (isNight && !isBlackout) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // draw lamp light triangles
        glDisable(GL_BLEND);
    } else if (isNight && isBlackout && showSolarPanel) {
        glColor3ub(150, 150, 150); // road lights off, building still powered
    } else if (!isNight) {
        glColor3ub(150, 150, 150); // daytime lamp off
    }

    glPopMatrix();
}

// ===== Draw Building (smaller + colorful) ===Galpa =====
void drawBuilding(float x, float width, float height, int colorType) {
    switch (colorType % 5) {
        case 0: glColor3ub(255, 99, 71); break;
        case 1: glColor3ub(70, 130, 180); break;
        case 2: glColor3ub(60, 179, 113); break;
        case 3: glColor3ub(238, 130, 238); break;
        case 4: glColor3ub(255, 215, 0); break;
    }

    glBegin(GL_QUADS);
        glVertex2f(x, 200.0f);
        glVertex2f(x + width, 200.0f);
        glVertex2f(x + width, 200.0f + height);
        glVertex2f(x, 200.0f + height);
    glEnd();

 glColor3ub(180, 180, 180);
     glBegin(GL_QUADS);
        glVertex2f(x, 200.0f + height);
        glVertex2f(x + width, 200.0f + height);
        glVertex2f(x + width - 4, 200.0f + height + 6);
        glVertex2f(x + 4, 200.0f + height + 6);
    glEnd();

        //blackout effect
    if (isNight && !isBlackout) {
        glColor3ub(255, 230, 150);   // glowing yellow at night
    } else if (isNight && isBlackout && showSolarPanel) {
        glColor3ub(255, 230, 150);   // solar-powered windows stay on
    } else {
        glColor3ub(0, 0, 0);         // windows off during blackout or day
    }
    float winSize = 5.0f;
    for (float i = x + 4; i < x + width - 4; i += 12) {
        for (float j = 210; j < 200.0f + height - 10; j += 15) {
            glBegin(GL_QUADS);
                glVertex2f(i, j);
                glVertex2f(i + winSize, j);
                glVertex2f(i + winSize, j + winSize);
                glVertex2f(i, j + winSize);
            glEnd();
        }
    }

    if (showSolarPanel) {
    // Solar panel dimensions
    float panelWidth = width - 4;
    float panelHeight = 6;
    float panelY = 200.0f + height + 2;

    // Draw solar panel rectangle
    glColor3ub(0, 0, 100); // dark blue panel
    glBegin(GL_QUADS);
        glVertex2f(x + 2, panelY);
        glVertex2f(x + 2 + panelWidth, panelY);
        glVertex2f(x + 2 + panelWidth, panelY + panelHeight);
        glVertex2f(x + 2, panelY + panelHeight);
    glEnd();

    glColor3ub(50, 50, 150);
    glLineWidth(1);
    glBegin(GL_LINES);
    for (float i = x + 2; i < x + 2 + panelWidth; i += panelWidth / 5) {
        glVertex2f(i, panelY);
        glVertex2f(i, panelY + panelHeight);
    }
    for (float j = panelY; j < panelY + panelHeight; j += panelHeight / 2) {
        glVertex2f(x + 2, j);
        glVertex2f(x + 2 + panelWidth, j);
    }

    glEnd();
}

}

//Railline Galpa
void drawRailLine(float startX, float endX, float yBase) {

    glColor3ub(80, 80, 80);
    glLineWidth(4);
    glBegin(GL_LINES);
        glVertex2f(startX, yBase + 10);
        glVertex2f(endX, yBase + 10);

        glVertex2f(startX, yBase + 20);
        glVertex2f(endX, yBase + 20);
    glEnd();

    glColor3ub(139, 69, 19);
    float tieSpacing = 15.0f;
    for (float x = startX; x <= endX; x += tieSpacing) {
        glBegin(GL_QUADS);
            glVertex2f(x, yBase + 8);
            glVertex2f(x + 5, yBase + 8);
            glVertex2f(x + 5, yBase + 22);
            glVertex2f(x, yBase + 22);
        glEnd();
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //sky
    sky();
    //road
    road();
    //Railline
    drawRailLine(0, 400, 150);
    //birds
    if (!isNight) {
    drawBird(birdX[0], birdY[0]);
    drawBird(birdX[1], birdY[1]);
    drawBird(birdX[2], birdY[2]);
    drawBird(birdX[3], birdY[3]);
    drawBird(birdX[4], birdY[4]);
    }

        //lightning
    if (lightning) {
    glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 0.8f); // white flash
        glVertex2f(0.0f, 0.0f);
        glVertex2f(400.0f, 0.0f);
        glVertex2f(400.0f, 400.0f);
        glVertex2f(0.0f, 400.0f);
    glEnd();
    }
    //sun or moon
    if (!isNight) {
        glPushMatrix();
            glTranslatef(sunX, sunY, 0);
            glColor3ub(255, 255, 0);
            drawCircle(0, 0, 15, 100);
        glPopMatrix();
    } else {
        glPushMatrix();
            glTranslatef(moonX, moonY, 0);
            glColor3ub(245, 245, 220);
            drawCircle(0, 0, 15, 100);
        glPopMatrix();
    }
    //electric polles
    float sp = 400.0f / 5;
    for (int i = 1; i <= 4; i++) {
        float xPos = i * sp;
        epoll(xPos, 200);
    }
    eware(200);
    //Street lights
    int numLights = 5;
    float lightSpacing = 400.0f / (numLights + 1);
    for (int i = 1; i <= numLights; i++) {
        float xPos = i * lightSpacing;
        drawRoadLight(xPos, 100);
    }
    //Tree ddaw near building
    drawTree(370,200);
    drawTree(15,200);
    //nead raod
    drawTree(30,100);
    drawTree(100,100);
    drawTree(170,100);
    drawTree(240,100);
    drawTree(310,100);
    drawTree(370,100);
    //draw cars
    drawCar(carBottomX, 20, 255, 0, 0, false);
    drawCar(carBottomX-80, 20, 255, 130, 45, false);

    drawCar(carTopX, 60, 0, 255, 0, true);
    drawCar(carTopX-80, 80, 234, 0, 100, true);
    //building Draw
    drawBuilding(30, 40, 70, 0);
    drawBuilding(170, 50, 90, 1);
    drawBuilding(100, 50, 80, 3);
    drawBuilding(250, 40, 75, 4);
    drawBuilding(300, 50, 65, 2);
    //Train Draw
    drawTrain(trainX, trainY);
    //clouds Draw
    for (int i = 0; i < NUM_CLOUDS; i++){
    drawCloud(cloudX[i], cloudY[i]);
    }
    //Solar Panel

    if (showPlane) {
        drawAirplane(planeX, planeY);
    }
    //Rain draw
    drawRain();

    glutSwapBuffers();
}

void timer(int v) {
    //sun moon
    if (!isNight) {
        sunX += 0.3f;
        if (sunX > 400) {
            isNight = true;
            sunX = -50;
            moonX = 420;
        }
    } else {
        moonX -= 0.3f;
        if (moonX <-50) {
            isNight = false;
              sunX = -50;
            moonX = 420;
        }
    }

    //Rain
    if (isRaining) {
    for (int i = 0; i < NUM_RAIN; i++) {
        rainY[i] -= 15; // fall speed
        if (rainY[i] < 0) {
            rainY[i] = 400;
            rainX[i] = rand() % 400;
            }
        }
    }
    //lighting
    if (lightning) {
        lightningTimer--;
        if (lightningTimer <= 0)
            lightning = false;
    }

    //car
    carBottomX += 1.0f; if (carBottomX > 450) carBottomX = -100;
    carTopX -= 1.1f; if (carTopX < -100) carTopX = 500;

    //clous
    updateClouds();

    //birds
    if (!isNight) updateBirds();

//aeroplane
    static int counter = 0;
    counter++;
    if (counter > 200) showPlane = true;
    if (showPlane) {
        planeX += 1.0f;
        if (planeX > 450) {
            planeX = -200;
            counter = 0;
            showPlane = false;
        }
    }

    // Update train position inside timer()
    trainX += trainSpeed;
    if (trainX > 450) trainX = -200;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'd':
        case 'D':
            isNight = false;//toggole Day
            sunX = 50;
            break;
        case 'n':
        case 'N':
            isNight = true;//toggol night
            moonX = 420;
            break;
        case 's':
        case 'S':
            showSolarPanel = !showSolarPanel; // toggle solar panels
            break;
        case 'b':
        case 'B':
            isBlackout = !isBlackout; // toggle blackout
            break;
        case 'r':
        case 'R':
            isRaining = !isRaining; // toggle rain
            break;
        case 'l':
        case 'L':
            if(isRaining){
            lightning = true;
            lightningTimer = 5; // lightning lasts 5 frames
            }
            break;


    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
      // Terminal instructions
    cout << "==============================\n";
    cout << "     Importance of Solar Panel Project      \n";
    cout << "==============================\n";
    cout << "Instructions:\n";
    cout << "D: Switch to Day mode\n";
    cout << "N: Switch to Night mode\n";
    cout << "S: Toggle Solar Panels on Buildings\n";
    cout << "B: Toggle Blackout\n";
    cout << "R:For Rain\n";
    cout << "L: Lightning Effect\n";
    cout << "------------------------------\n";

    //openGl window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Solar-Powered City Simulation");
    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
