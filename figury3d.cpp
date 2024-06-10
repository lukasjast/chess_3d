#include "raylib.h"
#include "rlgl.h"
#include "cmath"
#include <vector>

void DrawSign(int sign, Vector3 position, Color color);

class piece {
public:
    piece(int x, int y);
    ~piece();
    Vector2 position;
    Vector2 targetPosition;
    bool isMoving;
    float moveSpeed;
    int pieceType;
    bool isWhite;
    bool selected;
    float heightOffset;
    float targetHeightOffset;
    int moveStage;

    void DrawPiece(Model model);
    void MoveForward();
    void MoveTo(int x, int y);
    void UpdatePosition();
};

enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

//dodane funkcje
void ResetCameraPosition(Camera3D& camera);

int main()
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Szachy 3D");

    Camera3D camera = { 0 };
    ResetCameraPosition(camera);
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model pawnModel = LoadModel("obj/pawn.obj");
    Model knightModel = LoadModel("obj/knight.obj");
    Model bishopModel = LoadModel("obj/bishop.obj");
    Model rookModel = LoadModel("obj/rook.obj");
    Model queenModel = LoadModel("obj/queen.obj");
    Model kingModel = LoadModel("obj/king.obj");

    bool spot = true;
    bool start = false;
    bool cameraLock = false;
    bool info = false;
    bool dot = false;
    bool whiteTurn = true;

    Color whiteSpot = { 249,232,213,255 };
    Color blackSpot = { 86,50,8,255 };
    Vector3 spotSize = { 1.0f,0.1f,1.0f };
    Vector3 borderSize1 = { 10.0f, 0.1f, 1.0f };
    Vector3 borderSize2 = { 1.0f, 0.1f, 10.0f };
    Vector3 borderPosition1 = { 0.0f,-0.05f,4.5f };
    Vector3 borderPosition2 = { 0.0f,-0.05f,-4.5f };
    Vector3 borderPosition3 = { -4.5f,-0.05f,0.0f };
    Vector3 borderPosition4 = { 4.5f,-0.05f,0.0f };
    Vector3 dotCenter = { 0.0f,0.0f,0.0f };
    Color borderColor = { 120,177,124,255 };
    Color backgroundColor = { 140,226,238,255 };
    Color infoColor = { 195,190,238,255 };
    Color signColor = { 22,72,22,255 };

    std::vector<piece> whites;
    std::vector<piece> blacks;


    //tworzenie figur

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 8; ++j) {
            piece p = piece(1 + j, 1 + i);
            whites.push_back(p);
            p = piece(1 + j, 7 + i);
            blacks.push_back(p);
        }
    }

    DisableCursor();
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        /// aktualizacja pozycja kazdej figury przed rysowaniem
        for (int i = 0; i < whites.size(); ++i) {
            whites[i].UpdatePosition();
        }
        for (int i = 0; i < blacks.size(); ++i) {
            blacks[i].UpdatePosition();
        }

        BeginDrawing();
        ClearBackground(backgroundColor);
        BeginMode3D(camera);

        DrawCubeV(borderPosition1, borderSize1, borderColor);
        DrawCubeV(borderPosition2, borderSize1, borderColor);
        DrawCubeV(borderPosition3, borderSize2, borderColor);
        DrawCubeV(borderPosition4, borderSize2, borderColor);
        DrawCubeWiresV({ 0.0f,-0.05f,0.0f }, { 10.0f,0.1f,10.0f }, BLACK);

        for (float k = 0.0; k < 8.0f; k += 1.0) {
            DrawSign('A' + k, { -3.75f + k, 0.01f, 4.1f }, signColor);
        }

        for (float g = 0.0; g < 8.0f; g += 1.0) {
            DrawSign('1' + g, { -4.7f, 0.01f, 3.1f - g }, signColor);
        }

        for (float i = 0.0; i < 8.0f; i += 1.0) {
            spot = !spot;
            for (float j = 0.0; j < 8.0f; j += 1.0) {
                Vector3 spotPosition = { -3.5f + i,-0.05f, -3.5f + j };
                if (spot)
                    DrawCubeV(spotPosition, spotSize, blackSpot);
                else
                    DrawCubeV(spotPosition, spotSize, whiteSpot);
                DrawCubeWiresV(spotPosition, spotSize, BLACK);
                spot = !spot;
            }
        }

        //rysowanie figur
        for (int i = 0; i < whites.size(); ++i) {
            switch (whites[i].pieceType) {
            case PAWN:
                whites[i].DrawPiece(pawnModel);
                break;
            case KNIGHT:
                whites[i].DrawPiece(knightModel);
                break;
            case BISHOP:
                whites[i].DrawPiece(bishopModel);
                break;
            case ROOK:
                whites[i].DrawPiece(rookModel);
                break;
            case QUEEN:
                whites[i].DrawPiece(queenModel);
                break;
            default:
                whites[i].DrawPiece(kingModel);
            }
        }
        for (int i = 0; i < blacks.size(); ++i) {
            switch (blacks[i].pieceType) {
            case PAWN:
                blacks[i].DrawPiece(pawnModel);
                break;
            case KNIGHT:
                blacks[i].DrawPiece(knightModel);
                break;
            case BISHOP:
                blacks[i].DrawPiece(bishopModel);
                break;
            case ROOK:
                blacks[i].DrawPiece(rookModel);
                break;
            case QUEEN:
                blacks[i].DrawPiece(queenModel);
                break;
            default:
                blacks[i].DrawPiece(kingModel);
            }
        }

        if (dot) {
            DrawCylinder(dotCenter, 0.1f, 0.1f, 0.05f, 8, RED);
        }
        EndMode3D();
        if (IsKeyPressed('S'))
            start = true;
        if (!start) {
            UpdateCamera(&camera, CAMERA_ORBITAL);
            DrawText("Nacisnij 'S' aby zaczac", screenWidth / 2 - MeasureText("Nacisnij 'S' aby zaczac", 100) / 2, screenHeight / 2, 100, RED);
        }
        else if (!cameraLock)
            UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed('B')) {
            ResetCameraPosition(camera);
        }

        SetMouseCursor(3);

        if (IsKeyPressed(' ') and start) {
            if (cameraLock)
                DisableCursor();
            else
                EnableCursor();
            cameraLock = !cameraLock;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT and cameraLock)) {
            Ray ray = GetMouseRay(GetMousePosition(), camera);
            bool boardClicked = false;
            for (float a = 0.0; a < 8.0f; a += 1.0) {
                for (float b = 0.0; b < 8.0f; b += 1.0) {
                    Vector3 p1 = { -4.0f + a, 0.0f, -4.0f + b };
                    Vector3 p2 = { -3.0f + a, 0.0f, -4.0f + b };
                    Vector3 p3 = { -3.0f + a, 0.0f, -3.0f + b };
                    Vector3 p4 = { -4.0f + a, 0.0f, -3.0f + b };
                    RayCollision collision = GetRayCollisionQuad(ray, p1, p2, p3, p4);
                    if (collision.hit) {
                        dotCenter = { p1.x + 0.5f,p1.y,p1.z + 0.5f };
                        dot = true;
                        boardClicked = true;
                        if (whiteTurn) {
                            for (int i = 0; i < whites.size(); ++i) {
                                if (whites[i].selected) {
                                    whites[i].MoveTo(a + 1, 8 - b);
                                    for (int j = 0; j < blacks.size(); ++j) {
                                        if (blacks[j].position.x == a + 1 and blacks[j].position.y == 8 - b) {
                                            std::vector<piece>::iterator it = blacks.begin() + j;
                                            blacks.erase(it);
                                        }
                                    }
                                    whites[i].selected = false;
                                    whiteTurn = false;
                                }
                            }
                            for (int i = 0; i < whites.size(); ++i) {
                                if (whites[i].position.x - 1 == a and whites[i].position.y == 8 - b) {
                                    for (int j = 0; j < whites.size(); ++j) {
                                        whites[j].selected = false;
                                    }
                                    whites[i].selected = true;
                                }
                            }
                        }
                        else {
                            for (int i = 0; i < blacks.size(); ++i) {
                                if (blacks[i].selected) {
                                    blacks[i].MoveTo(a + 1, 8 - b);
                                    for (int j = 0; j < whites.size(); ++j) {
                                        if (whites[j].position.x == a + 1 and whites[j].position.y == 8 - b) {
                                            std::vector<piece>::iterator it = whites.begin() + j;
                                            whites.erase(it);
                                        }
                                    }
                                    blacks[i].selected = false;
                                    whiteTurn = true;
                                }
                            }
                            for (int i = 0; i < blacks.size(); ++i) {
                                if (blacks[i].position.x - 1 == a and blacks[i].position.y == 8 - b) {
                                    for (int j = 0; j < blacks.size(); ++j) {
                                        blacks[j].selected = false;
                                    }
                                    blacks[i].selected = true;
                                }
                            }
                        }
                    }
                }
            }
            if (!boardClicked)
                dot = false;
        }

        DrawText("Sterowanie - 'I'", 20, 20, 30, RAYWHITE);
        if (IsKeyPressed('I'))
            info = !info;
        if (info) {
            DrawRectangle(20, 60, screenWidth - 40, 300, Fade(infoColor, 0.5f));
            DrawRectangleLines(20, 60, screenWidth - 40, 300, BLACK);
            DrawText("Spacja - odblokuj/zablokuj kamere", 25, 65, 40, BLACK);
            DrawText("W, A, S, D - sterowanie pozycja kamery", 25, 105, 40, BLACK);
            DrawText("Strzalkami operujemy katem kamery", 25, 145, 40, BLACK);
            DrawText("B reset polozenia kamery", 25, 185, 40, BLACK);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void DrawSign(int sign, Vector3 position, Color color)
{
    Font font = GetFontDefault();
    float fontSize = 8.0f;
    int index = GetGlyphIndex(font, sign);
    float scale = fontSize / (float)font.baseSize;
    position.x += (float)(font.glyphs[index].offsetX - font.glyphPadding) / (float)font.baseSize * scale;
    position.z += (float)(font.glyphs[index].offsetY - font.glyphPadding) / (float)font.baseSize * scale;
    Rectangle srcRec = { font.recs[index].x - (float)font.glyphPadding, font.recs[index].y - (float)font.glyphPadding,
                         font.recs[index].width + 2.0f * font.glyphPadding, font.recs[index].height + 2.0f * font.glyphPadding };

    float width = (float)(font.recs[index].width + 2.0f * font.glyphPadding) / (float)font.baseSize * scale;
    float height = (float)(font.recs[index].height + 2.0f * font.glyphPadding) / (float)font.baseSize * scale;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float tx = srcRec.x / font.texture.width;
    float ty = srcRec.y / font.texture.height;
    float tw = (srcRec.x + srcRec.width) / font.texture.width;
    float th = (srcRec.y + srcRec.height) / font.texture.height;

    rlCheckRenderBatchLimit(4);
    rlSetTexture(font.texture.id);

    rlPushMatrix();
    rlTranslatef(position.x, position.y, position.z);

    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);

    rlNormal3f(0.0f, 1.0f, 0.0f);
    rlTexCoord2f(tx, ty); rlVertex3f(x, y, z);
    rlTexCoord2f(tx, th); rlVertex3f(x, y, z + height);
    rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);
    rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);

    rlEnd();
    rlPopMatrix();

    rlSetTexture(0);
}

piece::piece(int x, int y)
{
    selected = false;
    position.x = x;
    position.y = y;
    targetPosition = position;
    isMoving = false;
    moveSpeed = 0.05f;
    heightOffset = 0.0f;
    targetHeightOffset = 0.75f;
    moveStage = 0;
    if (y == 1 or y == 2)
        isWhite = true;
    else
        isWhite = false;

    if (y == 2 or y == 7)
        pieceType = PAWN;
    else {
        switch (x) {
        case 1:
        case 8:
            pieceType = ROOK;
            break;
        case 2:
        case 7:
            pieceType = KNIGHT;
            break;
        case 3:
        case 6:
            pieceType = BISHOP;
            break;
        case 4:
            pieceType = QUEEN;
            break;
        default:
            pieceType = KING;
        }
    }
}

piece::~piece()
{
}

void piece::DrawPiece(Model model)
{
    Color color;
    Color whitePiece = { 215,194,185,255 };
    Color blackPiece = { 70,51,43,255 };
    if (isWhite and !selected)
        color = whitePiece;
    else if (!isWhite and !selected)
        color = blackPiece;
    else if (isWhite and selected)
        color = YELLOW;
    else
        color = RED;

    Vector3 pos = { -4.45f + position.x, 0.0f + heightOffset, 4.5f - position.y };
    if (pieceType == ROOK)
        DrawModel(model, pos, 0.3f, color);
    else
        DrawModel(model, pos, 0.2f, color);
}

void piece::MoveForward()
{
    if (isWhite) {
        position.y += 1;
    }
    else {
        position.y -= 1;
    }
}
void piece::MoveTo(int x, int y) {
    targetPosition.x = x;
    targetPosition.y = y;
    isMoving = true;
    moveStage = 0;
}

void piece::UpdatePosition() {
    if (!isMoving) return;

    // Ruch składa się z trzech etapów
    if (moveStage == 0) { // Podnoszenie
        if (heightOffset < targetHeightOffset) {
            heightOffset += moveSpeed;
            if (heightOffset >= targetHeightOffset) {
                heightOffset = targetHeightOffset;
                moveStage = 1; // Przejście do etapu 2: Ruch poziomy
            }
        }
    }
    else if (moveStage == 1) { // Ruch poziomy
        float deltaX = targetPosition.x - position.x;
        float deltaY = targetPosition.y - position.y;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

        if (distance < moveSpeed) {
            position = targetPosition;
            moveStage = 2; // Przejście do etapu 3: Opuszczanie
        }
        else {
            position.x += (deltaX / distance) * moveSpeed;
            position.y += (deltaY / distance) * moveSpeed;
        }
    }
    else if (moveStage == 2) { // Opuszczanie
        if (heightOffset > 0.0f) {
            heightOffset -= moveSpeed;
            if (heightOffset <= 0.0f) {
                heightOffset = 0.0f;
                isMoving = false; // Ruch zakończony
                moveStage = 0; // Zresetowanie etapu ruchu
            }
        }
    }
}

void ResetCameraPosition(Camera3D& camera)
{
    camera.position = Vector3{ 10.0f, 10.0f, 10.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
}