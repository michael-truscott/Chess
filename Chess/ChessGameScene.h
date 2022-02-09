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
    void DrawMoveDot(ChessMove* move);

    void HandleMouse();
    void SelectPiece(Piece* piece);

    void ResetBoard();
    void FlipView();

    void ScreenCoordsToRankAndFile(int screenX, int screenY, Rank* rank, File* file);
    void RankAndFileToScreenCoords(Rank rank, File file, int* screenX, int* screenY);

    BoardState m_boardState;
    
    Piece* m_selectedPiece;
    std::vector<std::unique_ptr<ChessMove>> m_legalMoves;
    bool m_flipView;

    std::array<SDL_Texture*, (int)PieceType::MAX> m_whitePieceImages;
    std::array<SDL_Texture*, (int)PieceType::MAX> m_blackPieceImages;
    std::array<SDL_Texture*, 2> m_turnImages;

    SDL_Texture* m_moveDot;
};

