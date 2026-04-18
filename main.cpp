
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Window
int W = 1200, H = 700;

// Animation
float cloudX = 0.0f;
float carX1 = -1.2f, carX2 = 1.5f;
float carX3 = -1.8f, carX4 = 0.8f;
float moonGlow = 0.0f;
float moonDir = 0.005f;
bool nightMode = true;

// Stars
struct Star { float x, y, brightness; };
Star stars[200];

void initStars() {
    srand(42);
    for (int i = 0; i < 200; i++) {
        stars[i].x = ((rand() % 2000) / 1000.0f) - 1.0f;
        stars[i].y = ((rand() % 1000) / 1000.0f) * 0.6f + 0.25f;
        stars[i].brightness = (rand() % 100) / 100.0f;
    }
}

void setColor(float r, float g, float b) { glColor3f(r, g, b); }

void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int segs) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segs; i++) {
        float a = 2.0f * 3.14159f * i / segs;
        glVertex2f(cx + r * cos(a), cy + r * sin(a));
    }
    glEnd();
}

void drawSky() {
    if (nightMode) {
        // Night gradient
        glBegin(GL_QUADS);
        glColor3f(0.02f, 0.02f, 0.12f); glVertex2f(-1, 1);
        glColor3f(0.02f, 0.02f, 0.12f); glVertex2f(1, 1);
        glColor3f(0.05f, 0.05f, 0.22f); glVertex2f(1, -0.05f);
        glColor3f(0.05f, 0.05f, 0.22f); glVertex2f(-1, -0.05f);
        glEnd();
    } else {
        glBegin(GL_QUADS);
        glColor3f(0.4f, 0.7f, 1.0f); glVertex2f(-1, 1);
        glColor3f(0.4f, 0.7f, 1.0f); glVertex2f(1, 1);
        glColor3f(0.7f, 0.88f, 1.0f); glVertex2f(1, -0.05f);
        glColor3f(0.7f, 0.88f, 1.0f); glVertex2f(-1, -0.05f);
        glEnd();
    }
}

void drawStars() {
    if (!nightMode) return;
    static float twinkle = 0.0f;
    twinkle += 0.03f;
    for (int i = 0; i < 200; i++) {
        float b = stars[i].brightness * (0.6f + 0.4f * sin(twinkle + i * 0.5f));
        glColor3f(b, b, b * 1.1f);
        glPointSize(stars[i].brightness > 0.7f ? 2.0f : 1.0f);
        glBegin(GL_POINTS);
        glVertex2f(stars[i].x, stars[i].y);
        glEnd();
    }
}

void drawMoon() {
    if (!nightMode) {
        // Sun
        float glow = 0.05f + moonGlow * 0.05f;
        setColor(1.0f, 0.95f, 0.5f + glow);
        drawCircle(0.75f, 0.82f, 0.075f + glow * 0.5f, 40);
        setColor(1.0f, 0.98f, 0.8f);
        drawCircle(0.75f, 0.82f, 0.065f, 40);
        return;
    }
    // Glow halo
    float glow = moonGlow;
    setColor(0.15f + glow * 0.3f, 0.15f + glow * 0.3f, 0.25f + glow * 0.3f);
    drawCircle(-0.75f, 0.80f, 0.09f + glow * 0.02f, 40);
    // Moon body
    setColor(0.95f, 0.95f, 0.80f);
    drawCircle(-0.75f, 0.80f, 0.07f, 40);
    // Crescent shadow
    setColor(0.05f, 0.05f, 0.15f);
    drawCircle(-0.72f, 0.80f, 0.055f, 40);
    // Craters
    setColor(0.80f, 0.80f, 0.65f);
    drawCircle(-0.78f, 0.77f, 0.012f, 20);
    drawCircle(-0.80f, 0.83f, 0.008f, 20);
}

void drawCloud(float ox, float oy, float scale, float alpha) {
    if (nightMode)
        glColor4f(0.25f, 0.25f, 0.35f, alpha);
    else
        glColor4f(0.95f, 0.95f, 1.0f, alpha);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawCircle(ox,        oy,        0.06f * scale, 30);
    drawCircle(ox+0.07f*scale, oy+0.02f*scale, 0.07f * scale, 30);
    drawCircle(ox+0.14f*scale, oy,        0.055f * scale, 30);
    drawCircle(ox+0.21f*scale, oy+0.01f*scale, 0.065f * scale, 30);
    drawCircle(ox+0.28f*scale, oy,        0.05f * scale, 30);
    drawRect(ox - 0.01f*scale, oy - 0.04f*scale, ox + 0.33f*scale, oy + 0.02f*scale);
    glDisable(GL_BLEND);
}

void drawClouds() {
    drawCloud(-0.9f + cloudX, 0.78f, 1.0f, 0.85f);
    drawCloud(-0.2f + cloudX * 0.7f, 0.70f, 0.8f, 0.7f);
    drawCloud(0.35f + cloudX * 0.5f, 0.75f, 0.9f, 0.75f);
    if (-0.9f + cloudX > 1.4f) cloudX = cloudX - 2.3f;
}

void drawBuilding(float x, float w, float h, float r, float g, float b) {
    float top = -0.05f + h;
    // Body
    setColor(r, g, b);
    drawRect(x, -0.05f, x + w, top);
    // Roof detail
    setColor(r * 0.7f, g * 0.7f, b * 0.7f);
    drawRect(x + w * 0.1f, top, x + w * 0.9f, top + 0.02f);
    // Antenna
    setColor(0.5f, 0.5f, 0.5f);
    drawRect(x + w * 0.48f, top + 0.02f, x + w * 0.52f, top + 0.06f);
    // Windows
    int cols = (int)(w / 0.04f);
    int rows = (int)(h / 0.06f);
    if (cols < 1) cols = 1;
    if (rows < 1) rows = 1;
    float ww = w / (cols + 1);
    float wh = h / (rows + 1);
    for (int c = 0; c < cols; c++) {
        for (int rr = 0; rr < rows; rr++) {
            float wx = x + ww * (c + 0.5f);
            float wy = -0.05f + wh * (rr + 0.5f);
            // Some windows lit
            int lit = (c * 7 + rr * 3 + (int)(x * 10)) % 5;
            if (nightMode) {
                if (lit < 3) setColor(1.0f, 0.95f, 0.5f);
                else setColor(0.1f, 0.1f, 0.15f);
            } else {
                if (lit < 3) setColor(0.6f, 0.8f, 1.0f);
                else setColor(0.3f, 0.4f, 0.55f);
            }
            drawRect(wx - 0.013f, wy - 0.018f, wx + 0.013f, wy + 0.018f);
        }
    }
    // Dark edge (shadow)
    setColor(r * 0.4f, g * 0.4f, b * 0.4f);
    drawRect(x + w - 0.005f, -0.05f, x + w, top);
}

void drawBuildings() {
    // Far background buildings (darker, smaller)
    drawBuilding(-1.00f, 0.14f, 0.45f, 0.18f, 0.18f, 0.26f);
    drawBuilding(-0.85f, 0.10f, 0.38f, 0.15f, 0.15f, 0.22f);
    drawBuilding(-0.74f, 0.12f, 0.50f, 0.20f, 0.20f, 0.30f);
    drawBuilding(-0.60f, 0.09f, 0.42f, 0.16f, 0.16f, 0.24f);
    drawBuilding(-0.50f, 0.13f, 0.35f, 0.17f, 0.17f, 0.25f);
    drawBuilding(-0.35f, 0.11f, 0.55f, 0.20f, 0.20f, 0.30f);
    drawBuilding(-0.23f, 0.10f, 0.40f, 0.16f, 0.16f, 0.24f);
    drawBuilding(-0.12f, 0.15f, 0.48f, 0.20f, 0.20f, 0.28f);
    drawBuilding( 0.04f, 0.11f, 0.36f, 0.18f, 0.18f, 0.26f);
    drawBuilding( 0.16f, 0.13f, 0.52f, 0.22f, 0.22f, 0.32f);
    drawBuilding( 0.30f, 0.12f, 0.44f, 0.18f, 0.18f, 0.26f);
    drawBuilding( 0.43f, 0.10f, 0.38f, 0.15f, 0.15f, 0.22f);
    drawBuilding( 0.54f, 0.14f, 0.56f, 0.22f, 0.22f, 0.32f);
    drawBuilding( 0.69f, 0.12f, 0.46f, 0.18f, 0.18f, 0.26f);
    drawBuilding( 0.82f, 0.10f, 0.40f, 0.16f, 0.16f, 0.24f);
    drawBuilding( 0.93f, 0.08f, 0.35f, 0.14f, 0.14f, 0.20f);

    // Foreground buildings (brighter)
    drawBuilding(-1.00f, 0.18f, 0.55f, 0.28f, 0.28f, 0.38f);
    drawBuilding(-0.80f, 0.22f, 0.72f, 0.25f, 0.30f, 0.42f);
    drawBuilding(-0.56f, 0.16f, 0.48f, 0.30f, 0.28f, 0.40f);
    drawBuilding(-0.38f, 0.20f, 0.80f, 0.22f, 0.26f, 0.38f);  // Tall skyscraper
    drawBuilding(-0.17f, 0.18f, 0.58f, 0.28f, 0.32f, 0.44f);
    drawBuilding( 0.02f, 0.25f, 0.68f, 0.24f, 0.28f, 0.40f);
    drawBuilding( 0.28f, 0.16f, 0.52f, 0.28f, 0.30f, 0.42f);
    drawBuilding( 0.45f, 0.22f, 0.76f, 0.22f, 0.26f, 0.38f);  // Tall skyscraper
    drawBuilding( 0.68f, 0.18f, 0.60f, 0.26f, 0.30f, 0.42f);
    drawBuilding( 0.87f, 0.14f, 0.50f, 0.28f, 0.28f, 0.38f);
}

void drawGround() {
    // Road base
    setColor(0.18f, 0.18f, 0.20f);
    drawRect(-1.0f, -0.35f, 1.0f, -0.05f);
    // Sidewalks
    setColor(0.35f, 0.33f, 0.30f);
    drawRect(-1.0f, -0.07f, 1.0f, -0.05f);
    drawRect(-1.0f, -0.35f, 1.0f, -0.33f);
    // Lane markings
    setColor(0.85f, 0.82f, 0.20f);
    // Center dashed line
    for (int i = -10; i < 11; i++) {
        float lx = i * 0.18f;
        drawRect(lx, -0.205f, lx + 0.1f, -0.195f);
    }
    // Solid white lines
    setColor(0.9f, 0.9f, 0.9f);
    drawRect(-1.0f, -0.14f, 1.0f, -0.135f);
    drawRect(-1.0f, -0.265f, 1.0f, -0.26f);

    // Foreground ground
    setColor(0.12f, 0.14f, 0.10f);
    drawRect(-1.0f, -1.0f, 1.0f, -0.35f);
    // Pavement edge
    setColor(0.22f, 0.22f, 0.22f);
    drawRect(-1.0f, -0.40f, 1.0f, -0.35f);
}

void drawTree(float x, float y) {
    // Trunk
    setColor(0.35f, 0.22f, 0.10f);
    drawRect(x - 0.012f, y, x + 0.012f, y + 0.08f);
    // Foliage layers
    setColor(0.10f, 0.45f, 0.15f);
    drawCircle(x, y + 0.12f, 0.05f, 20);
    setColor(0.12f, 0.55f, 0.18f);
    drawCircle(x, y + 0.09f, 0.04f, 20);
    setColor(0.08f, 0.38f, 0.12f);
    drawCircle(x - 0.02f, y + 0.10f, 0.035f, 20);
    drawCircle(x + 0.02f, y + 0.10f, 0.035f, 20);
}

void drawStreetLight(float x) {
    // Pole
    setColor(0.5f, 0.5f, 0.5f);
    drawRect(x - 0.006f, -0.33f, x + 0.006f, -0.08f);
    // Arm
    drawRect(x - 0.006f, -0.09f, x + 0.04f, -0.083f);
    // Lamp housing
    setColor(0.4f, 0.4f, 0.4f);
    drawRect(x + 0.025f, -0.105f, x + 0.055f, -0.082f);
    // Light glow
    if (nightMode) {
        setColor(1.0f, 0.95f, 0.6f);
        drawCircle(x + 0.04f, -0.093f, 0.012f, 20);
        // Cone of light
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.95f, 0.5f, 0.12f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x + 0.04f, -0.10f);
        glVertex2f(x - 0.06f, -0.33f);
        glVertex2f(x + 0.14f, -0.33f);
        glEnd();
        glDisable(GL_BLEND);
    } else {
        setColor(0.9f, 0.9f, 0.7f);
        drawCircle(x + 0.04f, -0.093f, 0.010f, 20);
    }
}

void drawTrees() {
    drawTree(-0.90f, -0.33f);
    drawTree(-0.65f, -0.33f);
    drawTree(-0.42f, -0.33f);
    drawTree(-0.18f, -0.33f);
    drawTree( 0.08f, -0.33f);
    drawTree( 0.32f, -0.33f);
    drawTree( 0.56f, -0.33f);
    drawTree( 0.80f, -0.33f);
}

void drawStreetLights() {
    drawStreetLight(-0.75f);
    drawStreetLight(-0.30f);
    drawStreetLight( 0.18f);
    drawStreetLight( 0.65f);
}

void drawCar(float x, float y, float r, float g, float b, bool facingRight) {
    float d = facingRight ? 1.0f : -1.0f;
    // Body
    setColor(r, g, b);
    drawRect(x - 0.09f, y, x + 0.09f, y + 0.035f);
    // Roof
    setColor(r * 0.8f, g * 0.8f, b * 0.8f);
    drawRect(x - 0.055f, y + 0.035f, x + 0.055f, y + 0.062f);
    // Windows
    setColor(0.5f, 0.75f, 0.95f);
    drawRect(x - 0.048f, y + 0.038f, x - 0.008f, y + 0.058f);
    drawRect(x + 0.008f, y + 0.038f, x + 0.048f, y + 0.058f);
    // Wheels
    setColor(0.15f, 0.15f, 0.15f);
    drawCircle(x - 0.055f, y, 0.020f, 16);
    drawCircle(x + 0.055f, y, 0.020f, 16);
    setColor(0.55f, 0.55f, 0.55f);
    drawCircle(x - 0.055f, y, 0.011f, 16);
    drawCircle(x + 0.055f, y, 0.011f, 16);
    // Headlights / Taillights
    if (facingRight) {
        // headlights
        if (nightMode) {
            setColor(1.0f, 1.0f, 0.8f);
            drawRect(x + 0.085f, y + 0.012f, x + 0.092f, y + 0.022f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1.0f, 1.0f, 0.6f, 0.15f);
            glBegin(GL_TRIANGLES);
            glVertex2f(x + 0.09f, y + 0.017f);
            glVertex2f(x + 0.35f, y);
            glVertex2f(x + 0.35f, y + 0.04f);
            glEnd();
            glDisable(GL_BLEND);
        }
        // Taillights
        setColor(0.9f, 0.1f, 0.1f);
        drawRect(x - 0.092f, y + 0.012f, x - 0.085f, y + 0.022f);
    } else {
        if (nightMode) {
            setColor(1.0f, 1.0f, 0.8f);
            drawRect(x - 0.092f, y + 0.012f, x - 0.085f, y + 0.022f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1.0f, 1.0f, 0.6f, 0.15f);
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 0.09f, y + 0.017f);
            glVertex2f(x - 0.35f, y);
            glVertex2f(x - 0.35f, y + 0.04f);
            glEnd();
            glDisable(GL_BLEND);
        }
        setColor(0.9f, 0.1f, 0.1f);
        drawRect(x + 0.085f, y + 0.012f, x + 0.092f, y + 0.022f);
    }
}

void drawCars() {
    // Upper lane (going right)
    drawCar(carX1, -0.165f, 0.85f, 0.15f, 0.15f, true);
    drawCar(carX2, -0.165f, 0.20f, 0.45f, 0.85f, true);
    // Lower lane (going left)
    drawCar(carX3, -0.255f, 0.20f, 0.20f, 0.80f, false);
    drawCar(carX4, -0.255f, 0.85f, 0.50f, 0.10f, false);
}

void drawReflections() {
    if (!nightMode) return;
    // Wet road reflections
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Street light reflections on road
    float lxArr[] = {-0.71f, -0.26f, 0.22f, 0.69f};
    for (int li = 0; li < 4; li++) {
        float lx = lxArr[li];
        glColor4f(1.0f, 0.95f, 0.5f, 0.08f);
        drawRect(lx - 0.03f, -0.34f, lx + 0.11f, -0.06f);
    }
    glDisable(GL_BLEND);
}

void drawHUD() {
    // Mode indicator
    glColor3f(1, 1, 1);
    glRasterPos2f(-0.98f, 0.92f);
    const char* msg = nightMode ? "Night Mode (Press N)" : "Day Mode (Press N)";
    for (int i = 0; msg[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);

    // Title
    glRasterPos2f(-0.18f, 0.92f);
    const char* title = "CITY SCENERY";
    for (int i = 0; title[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);

    // Controls
    glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2f(0.60f, 0.92f);
    const char* ctrl = "ESC: Quit";
    for (int i = 0; ctrl[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ctrl[i]);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawSky();
    drawStars();
    drawMoon();
    drawClouds();
    drawBuildings();
    drawGround();
    drawReflections();
    drawTrees();
    drawStreetLights();
    drawCars();
    drawHUD();

    glutSwapBuffers();
}

void update(int value) {
    // Move clouds
    cloudX += 0.0004f;
    if (cloudX > 2.5f) cloudX = -2.5f;

    // Move cars (upper lane right, lower lane left)
    carX1 += 0.004f; if (carX1 > 1.3f) carX1 = -1.3f;
    carX2 += 0.003f; if (carX2 > 1.3f) carX2 = -1.3f;
    carX3 -= 0.005f; if (carX3 < -1.3f) carX3 =  1.3f;
    carX4 -= 0.003f; if (carX4 < -1.3f) carX4 =  1.3f;

    // Moon glow pulse
    moonGlow += moonDir;
    if (moonGlow > 1.0f || moonGlow < 0.0f) moonDir = -moonDir;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC
    if (key == 'n' || key == 'N') nightMode = !nightMode;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    W = w; H = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / h;
    if (aspect > 1.0f)
        gluOrtho2D(-aspect, aspect, -1.0, 1.0);
    else
        gluOrtho2D(-1.0, 1.0, -1.0/aspect, 1.0/aspect);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("City Scenery - OpenGL");
    glClearColor(0.02f, 0.02f, 0.1f, 1.0f);
    initStars();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
