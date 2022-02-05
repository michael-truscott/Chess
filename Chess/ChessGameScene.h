#pragma once
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include <array>

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    MAX,
};

enum class Color {
    WHITE,
    BLACK,
};

enum class Rank {
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
};

enum class File {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
};

struct Piece {
    Piece(PieceType type, Color color, Rank rank, File file);

    PieceType type;
    Color color;
    Rank rank;
    File file;
};

class ChessGameScene : public Scene
{
public:
    ChessGameScene(SDL_Renderer* renderer);
    virtual void Update(float dt) override;
    virtual void Render() override;

private:
    SDL_Renderer* m_renderer;
    void DrawBoard();
    void DrawPiece(Piece* piece);
    void HandleMouse();

    void ResetBoard();
    void NextTurn();

    Piece* GetPieceAt(Rank rank, File file);
    void ScreenCoordsToRankAndFile(int screenX, int screenY, Rank* rank, File* file);

    std::vector<std::unique_ptr<Piece>> m_pieces;
    std::array<SDL_Texture*, (int)PieceType::MAX> m_whitePieceImages;
    std::array<SDL_Texture*, (int)PieceType::MAX> m_blackPieceImages;
    Piece* m_selectedPiece;
    
    Color m_currentTurn;
    std::array<SDL_Texture*, 2> m_turnImages;
};

