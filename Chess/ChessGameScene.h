#pragma once
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include <array>
#include "Piece.h"
#include "BoardState.h"

enum class GameState
{
    NORMAL,
    GAME_OVER,
    PROMOTION_PROMPT,
};

class ChessGameScene : public Scene
{
public:
    ChessGameScene(SDL_Renderer* renderer);
    virtual void Update(float dt) override;
    virtual void Render() override;

protected:
    void FlipView();
    virtual bool TryMovePiece(Piece* piece, Rank rank, File file);
    virtual void ApplyPromoteMove(PieceType promoteType);
    virtual void SelectPiece(Piece* piece);

    SDL_Renderer* m_renderer;
    BoardState m_boardState;
    bool m_flipView;
    bool m_canUndoMoves;
    std::unique_ptr<ChessMove> m_promotionMove;

private:
    void DrawBoard();
    void DrawPiece(Piece* piece);
    void DrawSquareHighlight(Rank rank, File file);
    void DrawMoveDot(ChessMove* move);

    void HandleMouse();
    void ListLegalMoves();
    
    void ResetBoard();

    void ScreenCoordsToRankAndFile(int screenX, int screenY, Rank* rank, File* file);
    void RankAndFileToScreenCoords(Rank rank, File file, int* screenX, int* screenY);

    GameState m_gameState;
    
    
    Piece* m_selectedPiece;
    std::vector<std::unique_ptr<ChessMove>> m_legalMoves;
    
    std::array<SDL_Texture*, (int)PieceType::MAX> m_whitePieceImages;
    std::array<SDL_Texture*, (int)PieceType::MAX> m_blackPieceImages;
    std::array<SDL_Texture*, 2> m_turnImages;
    SDL_Texture* m_promoteDialog;
    SDL_Texture* m_moveDot;
};

