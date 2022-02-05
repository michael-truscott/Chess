#include "ChessGameScene.h"
#include "AssetLoader.h"
#include "Input.h"
#include <iostream>

constexpr int SQUARE_SIZE = 64;
constexpr int BOARD_X_OFFSET = 144;
constexpr int BOARD_Y_OFFSET = 44;

Piece::Piece(PieceType type, Color color, Rank rank, File file)
{
	this->type = type;
	this->color = color;
	this->rank = rank;
	this->file = file;
}

ChessGameScene::ChessGameScene(SDL_Renderer* renderer) :
	m_renderer(renderer),
	m_pieces(),
	m_selectedPiece(nullptr),
	m_currentTurn(Color::WHITE)
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
		Piece* targetPiece = GetPieceAt(rank, file);

		if (m_selectedPiece) {
			if (targetPiece == nullptr) {
				// Move the piece
				m_selectedPiece->rank = rank;
				m_selectedPiece->file = file;
				m_selectedPiece = nullptr;
				NextTurn();
			}
			else if (targetPiece == m_selectedPiece) {
				// de-select
				m_selectedPiece = nullptr;
			}
			else if (targetPiece->color != m_selectedPiece->color) {
				// capture the opposing piece
				m_pieces.erase(
					std::remove_if(m_pieces.begin(), m_pieces.end(),
						[targetPiece](const std::unique_ptr<Piece>& p) { return p.get() == targetPiece; })
				);
				m_selectedPiece->rank = rank;
				m_selectedPiece->file = file;
				m_selectedPiece = nullptr;
				NextTurn();
			}
			else if (targetPiece->color == m_selectedPiece->color) {
				m_selectedPiece = targetPiece;
			}
		}
		else {
			if (targetPiece != nullptr) {
				if (targetPiece->color == m_currentTurn) 
					m_selectedPiece = targetPiece;
			}
		}
	}
}

void ChessGameScene::ResetBoard()
{
	m_pieces.clear();

	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::WHITE, Rank::R1, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::WHITE, Rank::R1, File::B ));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::WHITE, Rank::R1, File::C ));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::QUEEN, Color::WHITE, Rank::R1, File::D ));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KING, Color::WHITE, Rank::R1, File::E ));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::WHITE, Rank::R1, File::F ));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::WHITE, Rank::R1, File::G ));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::WHITE, Rank::R1, File::H ));

	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::B));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::C));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::D));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::E));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::F));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::G));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::H));

	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::BLACK, Rank::R8, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::BLACK, Rank::R8, File::B));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::BLACK, Rank::R8, File::C));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::QUEEN, Color::BLACK, Rank::R8, File::D));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KING, Color::BLACK, Rank::R8, File::E));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::BLACK, Rank::R8, File::F));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::BLACK, Rank::R8, File::G));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::BLACK, Rank::R8, File::H));

	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::B));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::C));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::D));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::E));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::F));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::G));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::H));

	m_currentTurn = Color::WHITE;
}

void ChessGameScene::ScreenCoordsToRankAndFile(int screenX, int screenY, Rank* rank, File* file)
{
	*rank = (Rank)(7 - ((screenY - BOARD_Y_OFFSET) / SQUARE_SIZE));
	*file = (File)((screenX - BOARD_X_OFFSET) / SQUARE_SIZE);
}

Piece* ChessGameScene::GetPieceAt(Rank rank, File file)
{
	for (auto& piece : m_pieces) {
		if (piece->rank == rank && piece->file == file) {
			return piece.get();
		}
	}
	return nullptr;
}

void ChessGameScene::NextTurn()
{
	m_currentTurn = m_currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void ChessGameScene::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	SDL_Rect turnRect{ 0,0, 132, 132 };
	SDL_RenderCopy(m_renderer, m_turnImages[(int)m_currentTurn], NULL, &turnRect);

	DrawBoard();

	for (auto& piece : m_pieces)
		DrawPiece(piece.get());

	if (m_selectedPiece) {
		SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
		int x = BOARD_X_OFFSET + SQUARE_SIZE * (int)m_selectedPiece->file;
		int y = BOARD_Y_OFFSET + SQUARE_SIZE * (7 - (int)m_selectedPiece->rank);
		SDL_Rect rect{ x, y, 64, 64 };
		SDL_RenderDrawRect(m_renderer, &rect);
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
			if (isLightSquare) {
				SDL_SetRenderDrawColor(m_renderer, 240, 217, 181, 255);
				//SDL_SetRenderDrawColor(m_renderer, 255, 246, 0, 255);
			}
			else {
				SDL_SetRenderDrawColor(m_renderer, 181, 136, 99, 255);
				//SDL_SetRenderDrawColor(m_renderer, 0, 38, 255, 255);
			}
			
			SDL_Rect rect{ BOARD_X_OFFSET + x * SQUARE_SIZE, BOARD_Y_OFFSET + y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
			SDL_RenderFillRect(m_renderer, &rect);
			isLightSquare = !isLightSquare;
		}
	}
}

void ChessGameScene::DrawPiece(Piece* piece)
{
	int x = BOARD_X_OFFSET + SQUARE_SIZE * (int)piece->file;
	int y = BOARD_Y_OFFSET + SQUARE_SIZE * (7 - (int)piece->rank);
	SDL_Rect rect{ x, y, 64, 64 };
	SDL_Texture* texture = piece->color == Color::WHITE ? m_whitePieceImages[(int)piece->type] : m_blackPieceImages[(int)piece->type];
	SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}
