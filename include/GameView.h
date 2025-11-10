// GameView.h
#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Board.h" // <<< Dùng đường dẫn đến Board.h của bạn

class GameView {
public:
    // Hàm khởi tạo, nhận vào Board logic để nó biết phải vẽ gì
    GameView(Board& board);

    // Hàm vẽ tất cả mọi thứ (bàn cờ, quân cờ)
    void draw(sf::RenderWindow& window);

    // Hàm tiện ích: Chuyển đổi tọa độ pixel (click chuột)
    // sang tọa độ bàn cờ (x, y)
    bool mapPixelToBoardCoords(sf::Vector2i pixelPos, int& boardX, int& boardY);

private:
    Board& m_board; // Chỉ lưu tham chiếu tới logic Board

    // Các biến dùng để căn chỉnh và vẽ
    float m_padding;       // Lề xung quanh bàn cờ
    float m_cellSize;      // Kích thước (pixel) của 1 ô
    float m_boardSizePixels; // Tổng kích thước (pixel) của bàn cờ
    sf::Vector2f m_boardTopLeft; // Tọa độ pixel của góc trên-trái bàn cờ
};