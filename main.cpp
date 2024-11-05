#include <GL/glut.h>
#include <string>
#include <iostream>
using namespace std;

// Define screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int winner = 0;

// Define paddle properties
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 80;
const int PADDLE_SPEED = 8; // Increase paddle speed

// Define ball properties
const int BALL_SIZE = 10;
const float BALL_SPEED_X = 2.0f; // Decrease ball speed in X direction
const float BALL_SPEED_Y = 1.5f; // Decrease ball speed in Y direction
const float BALL_SPEED_X_EASY = 2.0f;
const float BALL_SPEED_Y_EASY = 1.5f;
const float BALL_SPEED_X_MEDIUM = 3.0f; // Increase ball speed for medium level
const float BALL_SPEED_Y_MEDIUM = 2.0f; // Increase ball speed for medium level
const float BALL_SPEED_X_HARD = 4.0f;   // Increase ball speed for hard level
const float BALL_SPEED_Y_HARD = 2.5f;   // Increase ball speed for hard level


// Global variables
int paddle1Y = SCREEN_HEIGHT / 2;
int paddle2Y = SCREEN_HEIGHT / 2;
float ballX = SCREEN_WIDTH / 2;
float ballY = SCREEN_HEIGHT / 2;
float ballSpeedX = BALL_SPEED_X;
float ballSpeedY = BALL_SPEED_Y;

int score1 = 0;
int score2 = 0;

bool gameRunning = false;

enum class GameLevel { EASY, MEDIUM, HARD };
GameLevel currentLevel = GameLevel::EASY;

// Function to draw a rectangle
void drawRectangle(int x, int y, int width, int height) {
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();
}

// Function to draw the paddles
void drawPaddles() {
    glColor3f(0.0f, 0.0f, 0.0f); // White color
    drawRectangle(20, paddle1Y - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);
    drawRectangle(SCREEN_WIDTH - PADDLE_WIDTH - 20, paddle2Y - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);
}

// Function to draw the ball
void drawBall() {
    glColor3f(0.0f, 0.0f, 0.0f); // White color
    drawRectangle(static_cast<int>(ballX - BALL_SIZE / 2), static_cast<int>(ballY - BALL_SIZE / 2), BALL_SIZE, BALL_SIZE);
}

// Function to draw the dashboard
void drawDashboard() {
    glColor3f(0.0f, 0.0f, 0.0f); // White color

    // Draw game title
    glRasterPos2i(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT - 30);
    std::string titleText = "Pong Game";
    for (char c : titleText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw instructions
    glRasterPos2i(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 60);
    std::string instructionText1 = "Player 1: Use 'W' and 'S' keys";
    for (char c : instructionText1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2i(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 80);
    std::string instructionText2 = "Player 2: Use 'I' and 'K' keys";
    for (char c : instructionText2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw scores
    glRasterPos2i(SCREEN_WIDTH / 4, SCREEN_HEIGHT - 110);
    std::string scoreText1 = "Player 1: " + std::to_string(score1);
    for (char c : scoreText1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2i(3 * SCREEN_WIDTH / 4, SCREEN_HEIGHT - 110);
    std::string scoreText2 = "Player 2: " + std::to_string(score2);
    for (char c : scoreText2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Function to display the user interface
void displayUI() {
    std::string levelString;
    switch (currentLevel) {
        case GameLevel::EASY: levelString = "Easy"; break;
        case GameLevel::MEDIUM: levelString = "Medium"; break;
        case GameLevel::HARD: levelString = "Hard"; break;
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2);
    std::string uiString = "Press 'P' to Start";
    for (char c : uiString) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    glRasterPos2i(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2+30);
    std::string uString = "Select Level: 'L' (Current: " + levelString + ")";
    for (char c : uString) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Function to display the credits
void displayCredits() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(SCREEN_WIDTH / 2 - 310, 20);
    std::string creditsText = "Pong Game OpenGL Project Created by 1804041, 1804055, 1804056";
    for (char c : creditsText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Function to update the ball's position
void updateBall() {
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Check collision with walls
    if (ballY - BALL_SIZE / 2 <= 0 || ballY + BALL_SIZE / 2 >= SCREEN_HEIGHT) {
        ballSpeedY = -ballSpeedY;
    }

    // Check collision with paddles
    if ((ballX - BALL_SIZE / 2 <= PADDLE_WIDTH + 20 && ballY >= paddle1Y - PADDLE_HEIGHT / 2 && ballY <= paddle1Y + PADDLE_HEIGHT / 2) ||
        (ballX + BALL_SIZE / 2 >= SCREEN_WIDTH - PADDLE_WIDTH - 20 && ballY >= paddle2Y - PADDLE_HEIGHT / 2 && ballY <= paddle2Y + PADDLE_HEIGHT / 2)) {
        ballSpeedX = -ballSpeedX;

        // Modify the ball's speed based on the current level
        switch (currentLevel) {
            case GameLevel::EASY:
                ballSpeedX = BALL_SPEED_X_EASY;
                ballSpeedY = BALL_SPEED_Y_EASY;
                break;
            case GameLevel::MEDIUM:
                ballSpeedX = BALL_SPEED_X_MEDIUM;
                ballSpeedY = BALL_SPEED_Y_MEDIUM;
                break;
            case GameLevel::HARD:
                ballSpeedX = BALL_SPEED_X_HARD;
                ballSpeedY = BALL_SPEED_Y_HARD;
                break;
        }
    }

    // Check if ball goes out of bounds (score points for the opponent)
    if (ballX - BALL_SIZE / 2 <= 0) {
        ballX = SCREEN_WIDTH / 2;
        ballY = SCREEN_HEIGHT / 2;
        ballSpeedX = BALL_SPEED_X;
        ballSpeedY = BALL_SPEED_Y;
        ++score2; // Player 2 scores
    }
    else if (ballX + BALL_SIZE / 2 >= SCREEN_WIDTH) {
        ballX = SCREEN_WIDTH / 2;
        ballY = SCREEN_HEIGHT / 2;
        ballSpeedX = BALL_SPEED_X;
        ballSpeedY = BALL_SPEED_Y;
        ++score1; // Player 1 scores
    }
}



// Function to handle keyboard input for the paddles and level selection
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        paddle1Y += PADDLE_SPEED;
        break;
    case 's':
        paddle1Y -= PADDLE_SPEED;
        break;
    case 'i':
        paddle2Y += PADDLE_SPEED;
        break;
    case 'k':
        paddle2Y -= PADDLE_SPEED;
        break;
    case 'p': // Start game with 'P' key
        gameRunning = true;
        ballX = SCREEN_WIDTH / 2;
        ballY = SCREEN_HEIGHT / 2;
        ballSpeedX = BALL_SPEED_X;
        ballSpeedY = BALL_SPEED_Y;
        score1 = 0;
        score2 = 0;
        break;
    case 'l': // Change level with 'L' key
        if (gameRunning) break; // Disable level selection during the game
        switch (currentLevel) {
            case GameLevel::EASY: currentLevel = GameLevel::MEDIUM; break;
            case GameLevel::MEDIUM: currentLevel = GameLevel::HARD; break;
            case GameLevel::HARD: currentLevel = GameLevel::EASY; break;
        }
        break;
    }
}

// Function to display the winner in a separate window
void displayWinnerWindow() {
    glutInitWindowSize(400, 100);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 400) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 100) / 2);
    int winnerWindow = glutCreateWindow("Winner");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color of the winner window to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 400, 0, 100);
    glutDisplayFunc([]() {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        glRasterPos2i(150, 50);
        std::string winnerText = "Player " + std::to_string(winner) + " Winner!";
        for (char c : winnerText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        glutSwapBuffers();
    });
    glutMainLoop(); // Start the main loop for the winner window
}


// Function to check for a winner
void checkWinner() {
    if (score1 >= 1) {
        winner = 1; // Player 1 wins
    } else if (score2 >= 1) {
        winner = 2; // Player 2 wins
    }

    if (winner != 0) {
        // Call function to display winner in a separate window
        displayWinnerWindow();
        gameRunning = false;
    }
}

// Function to draw the dotted line
void drawDottedLine() {
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glLineStipple(10, 0xCCCC); // Set the stipple pattern for the dotted line
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2i(SCREEN_WIDTH / 2, 30);
    glVertex2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT-130);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}


// Function to display the game screen
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameRunning) {
        drawDottedLine(); // Draw the dotted line
        drawPaddles();
        drawBall();
        drawDashboard(); // Draw dashboard on the screen
        checkWinner(); // Check for a winner
    } else {
        displayUI(); // Display user interface when the game is not running
    }
    displayCredits(); // Display credits
    glutSwapBuffers();
}

// ... (existing code below)


/*
// Function to display the game screen
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameRunning) {
        drawPaddles();
        drawBall();
        drawDashboard(); // Draw dashboard on the screen
    }
    else {
        displayUI(); // Display user interface when the game is not running
    }
    displayCredits(); // Display credits
    glutSwapBuffers();
}*/

// Function to update the game state and redraw the screen
void timer(int) {
    if (gameRunning) {
        updateBall(); // Update the ball's position
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Set the game loop to run at approximately 60 frames per second
}

// Function to set up the OpenGL environment
void init() {
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // Black background
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Pong Game");
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(handleKeypress);
    init();
    glutMainLoop();
    return 0;
}
