// GameView.cpp
#include "../include/GameView.h"

// Hàm khởi tạo
GameView::GameView(Board& board) : m_board(board) {
    // Thiết lập các biến vẽ
    m_padding = 40.f;
    m_cellSize = 60.f; // Mỗi ô rộng 60px
    
    // Lấy kích thước TỪ đối tượng board của bạn (là 9)
    int size = m_board.BOARD_SIZE; 
    
    // Bàn cờ 9x9 có 8 khoảng, nên (size - 1)
    m_boardSizePixels = m_cellSize * (size - 1); 
    
    m_boardTopLeft = sf::Vector2f(m_padding, m_padding); 
}

// Hàm vẽ chính
void GameView::draw(sf::RenderWindow& window) {
    int size = m_board.BOARD_SIZE; // Lấy kích thước (9)

    // 1. Vẽ các đường kẻ
    sf::RectangleShape line;
    line.setFillColor(sf::Color::Black);

    for (int i = 0; i < size; ++i) {
        // Vẽ đường ngang
        line.setSize(sf::Vector2f(m_boardSizePixels, 2.f)); // Dày 2px
        line.setPosition(m_boardTopLeft.x, m_boardTopLeft.y + i * m_cellSize);
        window.draw(line);

        // Vẽ đường dọc
        line.setSize(sf::Vector2f(2.f, m_boardSizePixels));
        line.setPosition(m_boardTopLeft.x + i * m_cellSize, m_boardTopLeft.y);
        window.draw(line);
    }

    // 2. Vẽ các quân cờ
    sf::CircleShape stoneShape(m_cellSize / 2.f - 4.f); // Bán kính
    
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            
            // Dùng hàm get() chính xác từ file Board.h của bạn
            StoneColor stone = m_board.get(x, y); 
            
            if (stone != StoneColor::EMPTY) {
                if (stone == StoneColor::BLACK) {
                    stoneShape.setFillColor(sf::Color::Black);
                } else {
                    stoneShape.setFillColor(sf::Color::White);
                }
                
                // Tính vị trí pixel để vẽ
                stoneShape.setPosition(
                    m_boardTopLeft.x + x * m_cellSize - stoneShape.getRadius(),
                    m_boardTopLeft.y + y * m_cellSize - stoneShape.getRadius()
                );
                window.draw(stoneShape);
            }
        }
    }
}

// Hàm chuyển đổi
bool GameView::mapPixelToBoardCoords(sf::Vector2i pixelPos, int& boardX, int& boardY) {
    int size = m_board.BOARD_SIZE;
    
    // Tính toán tọa độ (x, y)
    // Dùng 0.5f để làm tròn
    boardX = static_cast<int>((pixelPos.x - m_boardTopLeft.x + (m_cellSize / 2.f)) / m_cellSize);
    boardY = static_cast<int>((pixelPos.y - m_boardTopLeft.y + (m_cellSize / 2.f)) / m_cellSize);

    // Kiểm tra xem có nằm trong bàn cờ không
    if (boardX >= 0 && boardX < size && boardY >= 0 && boardY < size) {
        return true;
    }
    return false;
}