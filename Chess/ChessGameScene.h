#pragma once
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include <array>
#include "Piece.h"
#include "BoardState.h"

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
    void DrawSquareHighlight(Rank rank, File file);
    void HandleMouse();

    void ResetBoard();
    void FlipView();

    void ScreenCoordsToRankAndFile(int screenX, int screenY, Rank* rank, File* file);

    BoardState m_boardState;
    
    Piece* m_selectedPiece;
    bool m_flipView;

    std::array<SDL_Texture*, (int)PieceType::MAX> m_whitePieceImages;
    std::array<SDL_Texture*, (int)PieceType::MAX> m_blackPieceImages;
    std::array<SDL_Texture*, 2> m_turnImages;
};

