game-m1:
	clang++ main.cpp AssetLoader.cpp BoardState.cpp ChessGameScene.cpp ChessMove.cpp Events.cpp Game.cpp \
	 GamePacket.cpp Input.cpp NetClientChessGameScene.cpp NetServerChessGameScene.cpp PacketBuffer.cpp \
	 Piece.cpp Scene.cpp SceneManager.cpp TcpClient.cpp TcpServer.cpp \
	 -o Chess \
	 -std=c++17 \
	 -I/opt/homebrew/Cellar/sdl2/2.0.20/include/SDL2 \
	 -I/opt/homebrew/Cellar/sdl2_image/2.0.5/include/SDL2 \
	 -I/opt/homebrew/Cellar/sdl2_net/2.0.1/include/SDL2 \
	 -L/opt/homebrew/Cellar/sdl2/2.0.20/lib \
	 -L/opt/homebrew/Cellar/sdl2_image/2.0.5/lib \
	 -L/opt/homebrew/Cellar/sdl2_net/2.0.1/lib \
	 -lSDL2-2.0.0 -lSDL2_image-2.0.0 -lSDL2_net-2.0.0

game-x64:
	clang++ main.cpp AssetLoader.cpp BoardState.cpp ChessGameScene.cpp ChessMove.cpp Events.cpp Game.cpp \
	 GamePacket.cpp Input.cpp NetClientChessGameScene.cpp NetServerChessGameScene.cpp PacketBuffer.cpp \
	 Piece.cpp Scene.cpp SceneManager.cpp TcpClient.cpp TcpServer.cpp \
	 -o Chess \
	 -target x86_64-apple-darwin \
	 -std=c++17 \
	 -I/usr/local/Cellar/sdl2/2.0.20/include/SDL2 \
	 -I/usr/local/Cellar/sdl2_image/2.0.5/include/SDL2 \
	 -I/usr/local/Cellar/sdl2_net/2.0.1/include/SDL2 \
	 -L/usr/local/Cellar/sdl2/2.0.20/lib \
	 -L/usr/local/Cellar/sdl2_image/2.0.5/lib \
	 -L/usr/local/Cellar/sdl2_net/2.0.1/lib \
	 -lSDL2-2.0.0 -lSDL2_image-2.0.0 -lSDL2_net-2.0.0