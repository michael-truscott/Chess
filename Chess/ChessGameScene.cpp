#include "ChessGameScene.h"
#include "AssetLoader.h"
#include "Input.h"
#include <iostream>

constexpr int SQUARE_SIZE = 64;
constexpr int BOARD_X_OFFSET = 144;
constexpr int BOARD_Y_OFFSET = 44;

ChessGameScene::ChessGameScene(SDL_Renderer* renderer) :
	m_renderer(renderer),
	m_selectedPiece(nullptr),
	m_flipView(false)
{
	m_whitePieceImages[(int)PieceType::PAWN] = AssetLoader::LoadTextureFile("assets/images/pawn_white.png");
	m_whitePieceImages[(int)PieceType::KNIGHT] = AssetLoader::LoadTextureFile("assets/images/knight_white.png");
	m_whitePieceImages[(int)PieceType::BISHOP] = AssetLoader::LoadTextureFile("assets/images/bishop_white.png");
	m_whitePieceImages[(int)PieceType::ROOK] = AssetLoader::LoadTextureFile("assets/images/rook_white.png");
	m_whitePieceImages[(int)PieceType::QUEEN] = AssetLoader::LoadTextureFile("assets/images/queen_white.png");
	m_whitePieceImages[(int)PieceType::KING] = AssetLoader::LoadTextureFile("assets/images/king_white.png");

	m_blackPieceImages[(int)PieceType::PAWN] = AssetLoader::LoadTextureFile("assets/images/pawn_black.png");
	m_blackPieceImages[(int)PieceType::KNIGHT] = AssetLoader::LoadTextureFile("assets/images/knight_black.png");
	m_blackPieceImages[(int)PieceType::BISHOP] = AssetLoader::LoadTextureFile("assets/images/bishop_black.png");
	m_blackPieceImages[(int)PieceType::ROOK] = AssetLoader::LoadTextureFile("assets/images/rook_black.png");
	m_blackPieceImages[(int)PieceType::QUEEN] = AssetLoader::LoadTextureFile("assets/images/queen_black.png");
	m_blackPieceImages[(int)PieceType::KING] = AssetLoader::LoadTextureFile("assets/images/king_black.png");

	m_turnImages[(int)Color::WHITE] = AssetLoader::LoadTextureFile("assets/images/white_turn.png");
	m_turnImages[(int)Color::BLACK] = AssetLoader::LoadTextureFile("assets/images/black_turn.png");

	ResetBoard();
}

void ChessGameScene::Update(float dt)
{
	HandleMouse();

	if (Input::KeyPressed(SDL_SCANCODE_R)) {
		ResetBoard();
		m_selectedPiece = nullptr;
	}
	if (Input::KeyPressed(SDL_SCANCODE_F)) {
		FlipView();
	}
	if (Input::KeyPressed(SDL_SCANCODE_LEFT)) {
		m_boardState.Rewind();
	}
}

void ChessGameScene::HandleMouse()
{
	SDL_Point mouse = Input::GetMousePos();

	if (Input::MouseButtonPressed(SDL_BUTTON_LEFT)) {
		SDL_Rect boardRect{ BOARD_X_OFFSET, BOARD_Y_OFFSET, SQUARE_SIZE * 8, SQUARE_SIZE * 8 };
		if (!SDL_PointInRect(&mouse, &boardRect)) {
			m_selectedPiece = nullptr;
			return;
		}
		Rank rank;
		File file;
		ScreenCoordsToRankAndFile(mouse.x, mouse.y, &rank, &file);
		Piece* targetPiece = m_boardState.GetPieceAt(rank, file);

		if (m_selectedPiece) {
			if (targetPiece == m_selectedPiece) {
				m_selectedPiece = nullptr;
			}
			else if (targetPiece && targetPiece->color == m_selectedPiece->color) {
				m_selectedPiece = targetPiece;
			}
			else {
				if (m_boardState.MovePiece(m_selectedPiece, rank, file)) {
					m_selectedPiece = nullptr;
				}
			}
		}
		else {
			if (targetPiece != nullptr) {
				if (targetPiece->color == m_boardState.CurrentTurn()) 
					m_selectedPiece = targetPiece;
			}
		}
	}
}

void ChessGameScene::ResetBoard()
{
	m_boardState.Reset();
}

void ChessGameScene::FlipView()
{
	m_flipView = !m_flipView;
}

void ChessGameScene::ScreenCoordsToRankAndFile(int screenX, int screenY, Rank* rank, File* file)
{
	if (m_flipView) {
		*rank = (Rank)((screenY - BOARD_Y_OFFSET) / SQUARE_SIZE);
		*file = (File)(7 - (screenX - BOARD_X_OFFSET) / SQUARE_SIZE);
	}
	else {
		*rank = (Rank)(7 - ((screenY - BOARD_Y_OFFSET) / SQUARE_SIZE));
		*file = (File)((screenX - BOARD_X_OFFSET) / SQUARE_SIZE);
	}
}

void ChessGameScene::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	SDL_Rect turnRect{ 0,0, 132, 132 };
	SDL_RenderCopy(m_renderer, m_turnImages[(int)m_boardState.CurrentTurn()], NULL, &turnRect);

	DrawBoard();

	auto lastMove = m_boardState.LastMove();
	if (lastMove) {
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 128);
		DrawSquareHighlight(lastMove->oldRank, lastMove->oldFile);
		DrawSquareHighlight(lastMove->newRank, lastMove->newFile);
	}

	bool whiteInCheck = m_boardState.IsPositionInCheck(Color::WHITE);
	bool blackInCheck = m_boardState.IsPositionInCheck(Color::BLACK);
	for (auto& piece : m_boardState.GetAllPieces()) {
		if (piece->captured)
			continue;

		// Draw highlight depending on state priority
		if (piece.get() == m_selectedPiece) {
			SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 192);
			DrawSquareHighlight(piece->rank, piece->file);
		}
		else if (piece->type == PieceType::KING &&
				(piece->color == Color::WHITE && whiteInCheck ||
				piece->color == Color::BLACK && blackInCheck))
		{
			SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
			DrawSquareHighlight(piece->rank, piece->file);
		}
		
		DrawPiece(piece.get());
	}
}

void ChessGameScene::DrawBoard()
{
	int xOffset = 144;
	int yOffset = 44;
	bool isLightSquare;
	for (int y = 0; y < 8; y++) {
		isLightSquare = y % 2 == 0;
		for (int x = 0; x < 8; x++) {
			if (isLightSquare)
				SDL_SetRenderDrawColor(m_renderer, 240, 217, 181, 255);
			else
				SDL_SetRenderDrawColor(m_renderer, 181, 136, 99, 255);
			
			int rectX = BOARD_X_OFFSET + x * SQUARE_SIZE;
			int rectY = BOARD_Y_OFFSET + y * SQUARE_SIZE;
			SDL_Rect rect{ rectX, rectY, SQUARE_SIZE, SQUARE_SIZE };
			SDL_RenderFillRect(m_renderer, &rect);
			isLightSquare = !isLightSquare;
		}
	}
}

void ChessGameScene::DrawPiece(Piece* piece)
{
	int translatedFile = m_flipView ? (7 - (int)piece->file) : (int)piece->file;
	int translatedRank = m_flipView ? (int)piece->rank : (7 - (int)piece->rank);
	int x = BOARD_X_OFFSET + SQUARE_SIZE * translatedFile;
	int y = BOARD_Y_OFFSET + SQUARE_SIZE * translatedRank;

	SDL_Rect rect{ x, y, 64, 64 };
	SDL_Texture* texture = piece->color == Color::WHITE ? m_whitePieceImages[(int)piece->type] : m_blackPieceImages[(int)piece->type];
	SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}

void ChessGameScene::DrawSquareHighlight(Rank rank, File file)
{
	int rankInt = m_flipView ? (int)rank : (7 - (int)rank);
	int fileInt = m_flipView ? (7 - (int)file) : (int)file;
	int x = BOARD_X_OFFSET + SQUARE_SIZE * fileInt;
	int y = BOARD_Y_OFFSET + SQUARE_SIZE * rankInt;
	SDL_Rect rect{ x, y, 64, 64 };
	SDL_RenderFillRect(m_renderer, &rect);
}
